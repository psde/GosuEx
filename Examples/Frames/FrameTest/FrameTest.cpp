#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <GosuEx/Frames/Frames.hpp>
#include <GosuEx/Frames/AutoLink.hpp>
#include <GosuEx/FpsCounter/FpsCounter.hpp>
#include <boost/shared_ptr.hpp>

using namespace GosuEx::Frames;

typedef BasicList<Frame, int, FramedStaticButton> MyList;

namespace z {
	enum zOrder {
		zBackground,
		zSomeElement,
		zSomeOtherElement,
		zSomeOtherImbaElement,
		zCursor
	};
}
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
		FrameManager::root().createChild(new Frame(2, 2, z::zBackground, 796, 596, Colors::white), L"bg");
		dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"bg")).setBorderColor(Color(42, 87, 230));
		dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"bg")).setBorderWidth(2);
		FrameManager::root().createChild(new Frame(400, 0, z::zSomeElement, 400, 200, Gosu::Colors::cyan));
		FrameManager::root().createChild(f = new ExtFrame(50, 50, z::zSomeOtherElement, 150, 300, Gosu::Colors::cyan, Gosu::Colors::black, 5.0));
		f->setName(L"Johnny");
		f->setHighlightedBackgroundColor(Gosu::Colors::red);
		FrameManager::root().createChild(f = new ExtFrame(100, 100, z::zSomeOtherImbaElement, 150, 150, Gosu::Colors::yellow, Gosu::Colors::black, 2.0));
		f->setBorderColor(Gosu::Colors::green);
		f->setHighlightedBackgroundColor(Gosu::Colors::red);
		FrameManager::root().createChild(new TriangleCursor(z::zCursor, 15, std::numeric_limits<Unit>::infinity()-99, Colors::white, Colors::black, 3.0));
		fc = new GosuEx::FpsCounter(500, 40, 100, 260, 80, &graphics(), myFont);
		StaticText* st;
		FrameManager::root().createChild(st = new StaticText(100, 100, z::zSomeOtherElement, myFont, Colors::aqua));
		st->setText(L"FUN FUN FUN FUN");
		
		ExtFontText* ft;
		FrameManager::root().createChild(ft = new ExtFontText(400, 250, z::zSomeElement, myFont, Colors::white));
		ft->setHighlightedTextColor(Colors::red);
		ft->setText(L"A wild Pikachu appears!");
		ExtImage* i;
		FrameManager::root().createChild(i = new ExtImage(800, 600, z::zSomeOtherImbaElement, Gosu::resourcePrefix()+L"FramesLogo.png"));
		i->setHighlightedImage(new Gosu::Image(graphics(), Gosu::resourcePrefix()+L"FramesLogoH.png"));
		i->setRelX(1.0);
		i->setRelY(1.0);
		Unit x, y, width, height, rw, rh;
		x = i->dispX();
		y = i->dispY();
		width = i->dispWidth();
		height = i->dispHeight();
		rw = i->width();
		rh = i->height();
		FramedStaticButton* b;
		FrameManager::root().createChild(b = new FramedStaticButton(200, 200, z::zSomeOtherImbaElement, 250, 80, myFont, L"This is an annoying button.", Colors::black, Colors::white, Colors::blue, 2.0));
		MyList* l;
		FrameManager::root().createChild(l = new MyList(300, 300, z::zSomeElement, 300, 200), L"list");
		l->addElement(0, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Null", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(1, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Un", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(2, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Two", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(3, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Three", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(4, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Quattre", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(5, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Cinq", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(6, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Sechs", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(7, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Sieben", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(8, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Acht", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(9, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Neun", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(10, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Zehn", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(11, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Eleven", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(12, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Douze", Colors::black, Colors::white, Colors::black, 2.0));
		l->addElement(13, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Drölf", Colors::black, Colors::white, Colors::black, 2.0));
		l->setBackgroundColor(Gosu::Colors::none);
		l->reset();
		l->setIndex(0);
		l->setBorderColor(Colors::fuchsia);
		l->setBorderWidth(2.0);
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
		else if (btn == Gosu::kbUp) {
			MyList& l = dynamic_cast<MyList& >(FrameManager::singleton().namedWidget(L"list"));
			l.scrollBy(-0.1);
			l.reset();
		}
		else if (btn == Gosu::kbDown) {
			MyList& l = dynamic_cast<MyList& >(FrameManager::singleton().namedWidget(L"list"));
			l.scrollBy(0.1);
			l.reset();
		}
	}

	~FrameTestWindow() {
		FrameManager::despawn();
	}
};

int main(int argc, char* argv[]) {
	FrameTestWindow win;
	win.show();
}