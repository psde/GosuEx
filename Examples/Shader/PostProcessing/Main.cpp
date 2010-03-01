#include <Gosu/AutoLink.hpp>
#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>
#include <list>
#include <math.h>

#include <GosuEx/Shader/Shader.hpp>

using namespace std;

class GosuWindow : public Gosu::Window
{
	private:
		Gosu::Font *font;

		boost::shared_ptr<Gosu::Image> sky, gem;
		vector<Gosu::Image*> tileset;
		vector<Gosu::Image*> playerset;

		bool useShader;

	public:
		GosuWindow()
		 :	Gosu::Window(800, 600, false)
		{
			graphics().setResolution(800, 600);
			font = new Gosu::Font(graphics(), Gosu::defaultFontName(), 20);

			Gosu::imagesFromTiledBitmap(graphics(), Gosu::resourcePrefix() + L"media/Tileset.png", 60, 60, true, this->tileset);
			Gosu::imagesFromTiledBitmap(graphics(), Gosu::resourcePrefix() + L"media/CptnRuby.png", 50, 50, false, this->playerset);

			sky.reset(new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"media/Sky.png", true));
			gem.reset(new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"media/Gem.png", true));

			this->useShader = true;
		}

		void draw()
		{
			// Just drawing a quick test scene here
			this->font->draw(L"CptnCpp saz: \"Hy there!\"", 260, 260, 1, 1, 1, Gosu::Color::BLACK);
			
			this->sky->draw(0, 0, 0);

			this->gem->drawRot(410, 320, 1, 25 * sin((float)Gosu::milliseconds() / 1337));
			this->gem->drawRot(490, 340, 1, 25 * sin((float)Gosu::milliseconds() / 1337));

			this->playerset[0]->draw(315, 300, 1);
			this->tileset[0]->draw(300, 350, 1);
			this->tileset[0]->draw(350, 350, 1);
			this->tileset[0]->draw(400, 350, 1);
			this->tileset[1]->draw(300, 400, 1);
			this->tileset[1]->draw(350, 400, 1);
			this->tileset[1]->draw(400, 400, 1);
			this->tileset[1]->draw(450, 400, 1);

			if(this->useShader)
			{
				this->pixelate(graphics(), 4);
				this->sepia(graphics());
			}
			
			// This is not going to be "post processed" as it is drawn after the shaders are applied
			this->font->draw(L"Press space to toggle shaders", 20, 550, 1);

			if(GosuEx::Shader::available())
				this->font->draw(L"Shader are supported.", 20, 20, 1);
			else
				this->font->draw(L"Shader not supported! :(", 20, 20, 1);

		}

		void update()
		{
			
		}

		void buttonDown(Gosu::Button button)
		{
			if(button == Gosu::kbSpace) this->useShader = !this->useShader;
			if(button == Gosu::kbEscape) close();
		}

		void sepia(Gosu::Graphics &graphics)
		{
			GosuEx::Shader local(graphics);
			static GosuEx::ShaderProgram program = local.compile(Gosu::resourcePrefix() + L"shader/sepia.frag");
			local.run(program);
		};

		void pixelate(Gosu::Graphics &graphics, int pixelsize = 10)
		{
			GosuEx::Shader local(graphics);
			static GosuEx::ShaderProgram program = local.compile(Gosu::resourcePrefix() + L"shader/pixelate.frag");
			local.uniform1i(program, "pixelSize", pixelsize);           
			local.uniform1i(program, "width", GosuEx::realWidth(graphics));
			local.uniform1i(program, "height", GosuEx::realHeight(graphics));
			local.run(program);
		};
};

int main(int argc, char* argv[])
{
    GosuWindow win;
    win.show();

	return 0;
}