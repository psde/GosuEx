#define KERNEL_SIZE 9

uniform int pixelSize;
uniform sampler2D texture0; 
uniform int width;
uniform int height;

vec2 texCoords[KERNEL_SIZE]; 
                         
void main(void)
{
   vec4 avgColor; 
   vec2 texCoordsStep = 1.0/(vec2(float(width),float(height))/float(pixelSize));
   vec2 pixelBin = floor(gl_TexCoord[0].st/texCoordsStep);
   vec2 inPixelStep = texCoordsStep/3.0; 
   vec2 inPixelHalfStep = inPixelStep/2.0;


   texCoords[0] = vec2(inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[1] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[2] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[3] = vec2(inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[4] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[5] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[6] = vec2(inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[7] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;
   texCoords[8] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelHalfStep.y) + pixelBin * texCoordsStep;
   
   avgColor = texture2D(texture0, texCoords[0]) +
              texture2D(texture0, texCoords[1]) +
              texture2D(texture0, texCoords[2]) +
              texture2D(texture0, texCoords[3]) +
              texture2D(texture0, texCoords[4]) +
              texture2D(texture0, texCoords[5]) +
              texture2D(texture0, texCoords[6]) +
              texture2D(texture0, texCoords[7]) +
              texture2D(texture0, texCoords[8]);
   
   avgColor /= float(KERNEL_SIZE);
   
   gl_FragColor = avgColor;
}
