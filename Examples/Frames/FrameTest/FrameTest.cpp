#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <GosuEx/Frames/Frames.hpp>
#include <GosuEx/Frames/AutoLink.hpp>
#include <GosuEx/FpsCounter/FpsCounter.hpp>
#include <boost/shared_ptr.hpp>

using namespace GosuEx::Frames;
class FrameTestWindow : public Gosu::Window {
	GosuEx::FpsCounter* fc;
	boost::shared_ptr<Gosu::Font> myFont;
public:
	FrameTestWindow():
			Gosu::Window(800, 600, 0), myFont(new Gosu::Font(graphics(), Gosu::defaultFontName(), 20))
	{
		if (!FrameManager::spawn(&graphics(), &input()))
			throw std::runtime_error("Unable to initialize FrameManager");
		assert(&FrameManager::singleton());
		ExtFrame* f;
		FrameManager::root().createChild(new Frame(0, 0, 0, 800, 600, Color(230, 230, 230)));
		FrameManager::root().createChild(new Frame(400, 0, 1, 400, 200, Gosu::Colors::cyan));
		FrameManager::root().createChild(f = new ExtFrame(50, 50, 1, 150, 300, Gosu::Colors::cyan, Gosu::Colors::black, 5.0));
		f->setName(L"Johnny");
		f->setHighlightedBackgroundColor(Gosu::Colors::red);
		FrameManager::root().createChild(f = new ExtFrame(100, 100, 2, 150, 150, Gosu::Colors::yellow, Gosu::Colors::black, 2.0));
		f->setBorderColor(Gosu::Colors::green);
		f->setHighlightedBackgroundColor(Gosu::Colors::red);
		FrameManager::root().createChild(new TriangleCursor(1000, 15, 10, Colors::white, Colors::black, 3.0));
		fc = new GosuEx::FpsCounter(500, 40, 100, 260, 80, &graphics(), myFont);
		StaticText* st;
		FrameManager::root().createChild(st = new StaticText(100, 100, 14128, myFont, Colors::aqua));
		st->setText(L"FUN FUN FUN FUN");

		Image* i;
		FrameManager::root().createChild(i = new Image(800, 600, 10, Gosu::resourcePrefix()+L"FramesLogo.png"));
		i->setRelX(1.0);
		i->setRelY(1.0);
		FramedStaticButton* b;
		FrameManager::root().createChild(b = new FramedStaticButton(200, 200, 58, 250, 80, myFont, L"This is an annoying button.", Colors::black, Colors::white, Colors::blue, 2.0));
		//f->setBorderColor(Gosu::Colors::black);
		//f->setBorderWidth(2.0);
		//f->setBackgroundColor(Gosu::Colors::blue);
	}

	void update() {
		FrameManager::singleton().update();
	}

	void draw() {
		FrameManager::singleton().draw();
		fc->updateFPS();
		fc->draw();
	}

	void buttonUp(Gosu::Button btn) {
		if (btn == Gosu::kbNumpadAdd) {
			Frame& johnny = dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"Johnny"));
			johnny.setBorderWidth(johnny.borderWidth()+2);
		}
		else if (btn == Gosu::kbNumpadSubtract) {
			Frame& johnny = dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"Johnny"));
			johnny.setBorderWidth(johnny.borderWidth()-2);
		}
	}
};

int main(int argc, char* argv[]) {
	FrameTestWindow win;
	win.show();
}