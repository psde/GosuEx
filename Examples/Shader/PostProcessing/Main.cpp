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

	public:
		GosuWindow()
		 :	Gosu::Window(800, 600, false)
		{
			graphics().setResolution(800, 600);
			font = new Gosu::Font(graphics(), Gosu::defaultFontName(), 20);

			Gosu::imagesFromTiledBitmap(graphics(), L"media/Tileset.png", 60, 60, true, this->tileset);
			Gosu::imagesFromTiledBitmap(graphics(), L"media/CptnRuby.png", 50, 50, false, this->playerset);

			sky.reset(new Gosu::Image(graphics(), L"media/Sky.png", true));
			gem.reset(new Gosu::Image(graphics(), L"media/Gem.png", true));
		}

		void draw()
		{
			// Just drawing a quick test scene here
			this->font->draw(L"TEST!", 20, 20, 1);
			
			this->sky->draw(0, 0, 0);
			this->gem->draw(380, 305, 1);
			this->playerset[0]->draw(310, 300, 1);
			this->tileset[0]->draw(300, 350, 1);
			this->tileset[0]->draw(350, 350, 1);
			this->tileset[0]->draw(400, 350, 1);
			this->tileset[1]->draw(300, 400, 1);
			this->tileset[1]->draw(350, 400, 1);
			this->tileset[1]->draw(400, 400, 1);
			this->tileset[1]->draw(450, 400, 1);

			this->pixelate(graphics(), 4);
			this->sepia(graphics());
		}

		void update()
		{
			
		}

		void buttonDown(Gosu::Button button)
		{
			if(button == Gosu::kbEscape) close();
		}

		void sepia(Gosu::Graphics &graphics)
		{
			GosuEx::Shader local(graphics);
			static GosuEx::ShaderProgram program = local.compile(Gosu::sharedResourcePrefix() + L"shader/sepia.frag");
			local.run(program);
		};

		void pixelate(Gosu::Graphics &graphics, int pixelsize = 10)
		{
			GosuEx::Shader local(graphics);
			static GosuEx::ShaderProgram program = local.compile(Gosu::sharedResourcePrefix() + L"shader/pixelate.frag");
			local.uniform(program, "pixelSize", pixelsize);           

			local.uniform(program, "width", (int)GosuEx::realWidth(graphics));
			local.uniform(program, "height", (int)GosuEx::realHeight(graphics));
			local.run(program);
		};

};

int main(int argc, char* argv[])
{
    GosuWindow win;
    win.show();

	return 0;
}