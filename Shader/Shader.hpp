#ifndef GOSUEX_SHADER_HPP
#define GOSUEX_SHADER_HPP

#include <Gosu/Math.hpp>
#include <Gosu/IO.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/Directories.hpp>

// Include glew and add the lib
#include <GL/glew.h>
#ifdef _MSC_VER
	#pragma comment(lib, "glew32.lib")
#endif

#include <vector>
#include <map>
#include <iostream>

namespace GosuEx
{
	// Gets real screen width from the graphics
	inline unsigned int realWidth(Gosu::Graphics &graphics)
	{
		return Gosu::round(graphics.width() * graphics.factorX());
	}

	// Gets real screen height from the graphics 
	inline unsigned int realHeight(Gosu::Graphics &graphics)
	{
		return Gosu::round(graphics.height() * graphics.factorY());
	}

	// Holds opengl ids to shader program, vertex and fragment shader
	struct ShaderProgram
	{
		GLint program, vertex, fragment;
	};

	struct Kernel3x3
	{
		float texCoordOffsets[18];
	};

	/* Handles custom fragment shader and multiple-pass rendering 
	 * PostProcessing only uses fragment shaders, vertex shaders are not much use 
	 * The class can handle to run multiple shaders per draw(), but for each shader-run the frame buffer gets copied to texture */
	// Not true anymore, this class can handly vertex shader aswell, if the user wishes to do so.
	class Shader
	{
		private:
			Gosu::Graphics *graphics;

		public:
			Shader(Gosu::Graphics &graphics)
			{
				// Set our local graphics reference to passed argument
				this->graphics = &graphics;

				static bool glewInitialized = false;

				// Is glew already initialized?
				if(!glewInitialized)
				{
					glewInit();

					glewInitialized = true;
				}

				// Check if shaders are supported by GPU
				if (!Shader::available())
					return;

				// Init the texture 
				internalTexture(graphics);
			}

			static GLuint internalTexture(Gosu::Graphics& graphics)
			{
				static bool initialized = false;
				static GLuint texture = -1;
				static int textureWidth, textureHeight;

				int width = realWidth(graphics);
				int height = realHeight(graphics);

				// Is the texture uninitialized or did the screen size change?
				if (!initialized || textureWidth != width || textureHeight != height)
				{
					// If texture already exists, delete it first
					if(texture != -1)
						glDeleteTextures(1, &texture);

					// Create a texture of screen size
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, 1);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
					glBindTexture(GL_TEXTURE_2D, 0);

					// Save the width and height
					textureWidth = width;
					textureHeight = height;

					initialized = true;
				}

				return texture;
			}

			// TODO: this is ugly, rethink this approach
			void kernelTextureCoordOffsets(ShaderProgram program, char *uniform)
			{
				Kernel3x3 texKernel;
				GLint i, j;
				GLfloat xInc, yInc;

				xInc = 1.0f / (float)realWidth(*this->graphics);
				yInc = 1.0f / (float)realHeight(*this->graphics);

				for (i = 0; i < 3; i++)
				{
					for (j = 0; j < 3; j++)
					{
						texKernel.texCoordOffsets[(((i*3)+j)*2)+0] = (-1.0f * xInc) + ((GLfloat)i * xInc);
						texKernel.texCoordOffsets[(((i*3)+j)*2)+1] = (-1.0f * yInc) + ((GLfloat)j * yInc);
					}
				}

				glUseProgram(program.program);
				glUniform2fv(glGetUniformLocation(program.program, uniform), 9, texKernel.texCoordOffsets);
				glUseProgram(0);
			}

			// Check if shaders are supported by GPU
			static bool available()
			{
				if(glewIsSupported("GL_VERSION_2_0")) return true;
				return false;
			}

			std::string loadShaderFile(std::wstring file)
			{
				Gosu::Buffer buf;
				Gosu::loadFile(buf, file);
				std::string data(reinterpret_cast<char*>(buf.data()), buf.size());
				return data;
			}

			// Check if shader is already compiled and returns pointer to compiled ShaderProgram
			ShaderProgram compile(std::wstring fsfile, std::wstring vsfile = L"")
			{
				// Map that stores compiled shader programs
				static std::map<std::wstring, ShaderProgram> shaders;
				if(shaders.find(fsfile) == shaders.end())
				{
					// Load the fragment shader file
					std::string temp = this->loadShaderFile(fsfile);
					const GLchar *fragmentShader = temp.c_str();

					// Create shader program
					ShaderProgram program;
					program.program = glCreateProgram();

					// 'Fixed functionality' vertex shader, this is hard coded for easy use
					const GLchar* ffVertexShader = "void main(void)\r\n{\r\ngl_Position = ftransform();\r\ngl_TexCoord[0] = gl_MultiTexCoord0;\r\n}\r\n";

					int success;
					// Attach vertex shader
					program.vertex = glCreateShader(GL_VERTEX_SHADER);
					if(vsfile != L"")
					{
						temp = this->loadShaderFile(vsfile);
						const GLchar* vertexShader = temp.c_str();
						glShaderSource(program.vertex, 1, &vertexShader, NULL);
					}else{
						glShaderSource(program.vertex, 1, &ffVertexShader, NULL);
					}
					glCompileShader(program.vertex);
					glGetShaderiv(program.vertex, GL_COMPILE_STATUS, &success);
					if(!success)
					{
						std::cout << "vertex shader error" << std::endl;
						GLchar log[32768];
						glGetShaderInfoLog(program.vertex, 32768, NULL, log);
						std::cout << log << std::endl;
						//throw std::runtime_error(log);
					}
					glAttachShader(program.program, program.vertex);

					// Attach fragment shader
					program.fragment = glCreateShader(GL_FRAGMENT_SHADER);
					glShaderSource(program.fragment, 1, &fragmentShader , NULL);
					glCompileShader(program.fragment);					
					glGetShaderiv(program.fragment, GL_COMPILE_STATUS, &success);
					if(!success)
					{
						std::cout << "fragment shader error" << std::endl;
						GLchar log[32768];
						glGetShaderInfoLog(program.fragment, 32768, NULL, log);
						std::cout << log << std::endl;
						//throw std::runtime_error(log);
					}
					glAttachShader(program.program, program.fragment);

					// Compile shader program
					glLinkProgram(program.program);

					// Add compiled program to our shader map
					shaders[fsfile] = program;
				}
				return shaders[fsfile];
			}
			
			// Think about passing stuff to the shader, maybe we should not wrap around glUniformXY
			// (And even if we do, implement stuff like uniform2f/3f for vector stuff, as they are quite common)
			// Sets integer uniform
			void uniform1i(ShaderProgram program, char *uniform, int value)
			{
				glUseProgram(program.program);
				glUniform1i(glGetUniformLocation(program.program, uniform), value);
				glUseProgram(0);
			}

			// Sets float uniform
			void uniform1f(ShaderProgram program, char *uniform, float value)
			{
				glUseProgram(program.program);
				glUniform1f(glGetUniformLocation(program.program, uniform), value);
				glUseProgram(0);
			}

			void uniform2f(ShaderProgram program, char *uniform, float value1, float value2)
			{
				glUseProgram(program.program);
				glUniform2f(glGetUniformLocation(program.program, uniform), value1, value2);
				glUseProgram(0);
			}

			void uniform3f(ShaderProgram program, char *uniform, float value1, float value2, float value3)
			{
				glUseProgram(program.program);
				glUniform3f(glGetUniformLocation(program.program, uniform), value1, value2, value3);
				glUseProgram(0);
			}

			// Applies shader to frame buffer contents
			// Think about stuff like texture units. Maybe the user wants to do something more "special"
			// like multitexturing. He should be able to do so without replicating all the gl stuff
			void run(ShaderProgram program)
			{
				int width = realWidth(*this->graphics);
				int height = realHeight(*this->graphics);

				// If we don't have shader support don't do anything
				if(!Shader::available()) return;

				// Force the Gosu drawOpQueue to flush
				graphics->beginGL();

				// Copy frame buffer contents to internal texture
				glBindTexture(GL_TEXTURE_2D, GosuEx::Shader::internalTexture(*this->graphics));
				glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, width, height, 0);

				// Activate shader program
				glUseProgram(program.program);

				// Clears screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadIdentity();
				glViewport(0, 0, width, height);
				glOrtho(0, width, height, 0, -1, 1);

				// Enable texturing and bind the internal texture
				// Why are we not activating texturing nor the texture unit here?
				//glEnable(GL_TEXTURE_2D);
				//glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, GosuEx::Shader::internalTexture(*this->graphics));

				// Draws screen-sized quad
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f);
					glVertex2f(0, 0);
					glTexCoord2f(1.0f, 1.0f);
					glVertex2f((GLfloat)width, 0);
					glTexCoord2f(1.0f, 0.0f);
					glVertex2f((GLfloat)width, (GLfloat)height);
					glTexCoord2f(0.0f, 0.0f);
					glVertex2f(0, (GLfloat)height);
				glEnd();

				// Disable shader program
				glUseProgram(0);

				glMatrixMode(GL_PROJECTION);
				glPopMatrix();

				graphics->endGL();
			}
	};
}

#endif