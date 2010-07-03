#include <GosuEx/Frames/Fwd.hpp>
#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <GosuEx/Frames/Frames.hpp>
#include <GosuEx/Frames/AutoLink.hpp>
#include <GosuEx/FPSCounter/FPSCounter.hpp>

using namespace GosuEx::Frames;

typedef BasicList<int, FramedStaticButton> MyList;

namespace z {
	enum zOrder {
		zBackground,
		zSomeElement,
		zSomeOtherElement,
		zSomeOtherImbaElement,
		zFPS,
		zCursor
	};
}
class FrameTestWindow : public Gosu::Window {
	GosuEx::FPSCounter* fc;
	boost::shared_ptr<Gosu::Font> myFont;
public:
	FrameTestWindow():
			Gosu::Window(800, 600, 0), myFont(new Gosu::Font(graphics(), Gosu::defaultFontName(), 20))
	{
		// Spawn the FrameManager
		if (!FrameManager::spawn(graphics(), input()))
			throw std::runtime_error("Unable to initialize FrameManager");
		// Assert it exists. (it does.)
		assert(&FrameManager::singleton());

		// Create a ExtFrame
		// Things with 'Ext' as prefix are always extended.
		// At least they support highlighted (cursor-is-over-Widget), disabled and disabledHighlighted methods
		// and/or colors.
		// ExtFrame can change colors on highlighting and disabling.
		ExtFrame* f;
		// It's important to know the difference between add and create, delete and remove.
		// create* tells the FrameManager "New widget here" - this is necessary for many reasons
		// (highlighting etc.). add* does just tell the widget is has a new child.
		// delete* does REALLY delete the widget, giving its memory free. remove just removes it.
		// To move widgets around, use remove* and add*.
		// Here, we create a new one.
		// create* always returns T& (the thing created), so we could save it. Here, we demonstrate names and
		// how to use the singleton.
		// Create a Frame, set its border and background.
		FrameManager::root().createChild(new Frame(2, 2, z::zBackground, 796, 596, Colors::white), L"bg");
		dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"bg")).setBorderColor(Color(42, 87, 230));
		dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"bg")).setBorderWidth(2);

		// Create another box, on top of the other.
		FrameManager::root().createChild(new Frame(400, 0, z::zSomeElement, 400, 200, Gosu::Colors::cyan));
		// And another
		FrameManager::root().createChild(f = new ExtFrame(50, 50, z::zSomeOtherElement, 150, 300, Gosu::Colors::cyan, Gosu::Colors::black, 5.0));
		// Name it Johnny.
		f->setName(L"Johnny");
		f->setHighlightedBackgroundColor(Gosu::Colors::red);

		// And an IMBA element. It's not that imba though.
		FrameManager::root().createChild(f = new ExtFrame(100, 100, z::zSomeOtherImbaElement, 150, 150, Gosu::Colors::yellow, Gosu::Colors::black, 2.0));
		f->setBorderColor(Gosu::Colors::green);
		f->setHighlightedBackgroundColor(Gosu::Colors::red);
		FrameManager::root().createChild(new TriangleCursor(z::zCursor, 15, std::numeric_limits<Unit>::infinity()-99, Colors::white, Colors::black, 3.0));

		// For debugging-and-fps-reasons
		fc = new GosuEx::FPSCounter(500, 0, z::zFPS, 100, 80, graphics(), myFont, Gosu::Color::BLUE, Gosu::Color::GRAY, Gosu::Color::RED, true);

		// StaticText
		// There is StaticText and FontText. StaticText draws its text to a bitmap, then it's like an Image.
		// FontText is using Gosu::Font.
		// As a "rule": Text changes often: Use FontText.
		// Otherwise, StaticText.
		// There's also ExtStaticText and ExtFontText.

		StaticText* st;
		FrameManager::root().createChild(st = new StaticText(100, 100, z::zSomeOtherElement, myFont, Colors::aqua));
		st->setText(L"FUN FUN FUN FUN");
		
		ExtFontText* ft;
		FrameManager::root().createChild(ft = new ExtFontText(400, 250, z::zSomeElement, myFont, Colors::white));
		ft->setHighlightedTextColor(Colors::red);
		ft->setText(L"A wild Pikachu appears!");

		// Images.
		ExtImage* i;
		FrameManager::root().createChild(i = new ExtImage(800, 600, z::zSomeOtherImbaElement, Gosu::resourcePrefix()+L"FramesLogo.png"));
		i->setHighlightedImage(new Gosu::Image(graphics(), Gosu::resourcePrefix()+L"FramesLogoH.png"));
		i->setRelX(1.0);
		i->setRelY(1.0);
		
		// Buttonz.
		// Buttons are a background (mostly a frame/image) with something put on it (mostly Text/Image).
		// They are also available as Ext versions.
		FramedStaticButton* b;
		FrameManager::root().createChild(b = new FramedStaticButton(200, 200, z::zSomeOtherImbaElement, 250, 80, myFont, L"This is an annoying button.", Colors::black, Colors::white, Colors::blue, 2.0));

		// Lists are only administrative. They do not draw anything. You mustn't add children to it.
		// You can, however, add elements. They will be sorted correctly by key. Scrolling s a bit weird though.
		MyList* l;
		FrameManager::root().createChild(l = new MyList(300, 300, z::zSomeElement, 300, 200), L"list");
		
		l->createElement(0, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Null", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(1, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Un", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(2, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Two", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(3, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Three", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(4, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Quattre", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(5, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Cinq", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(6, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Sechs", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(7, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Sieben", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(8, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Acht", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(9, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Neun", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(10, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Zehn", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(11, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Eleven", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(12, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Douze", Colors::black, Colors::white, Colors::black, 2.0));
		l->createElement(13, new FramedStaticButton(0, 0, z::zSomeOtherImbaElement, 0, 0, myFont, L"Drölf", Colors::black, Colors::white, Colors::black, 2.0));

		// Show people the magic of setBtnDownHandler!
		l->setButtonUpHandler(Gosu::msWheelUp, boost::bind(&MyList::scrollBy, l, -0.5));
		l->setButtonUpHandler(Gosu::msWheelDown, boost::bind(&MyList::scrollBy, l, -0.5));

		l->reset();
		l->setIndex(0);
	}

	~FrameTestWindow() {
		delete fc;
		myFont.reset();
		// Important to despawn it.
		FrameManager::despawn();
	}

	void update() {
		FrameManager::singleton().update();
		fc->update();
	}

	void draw() {
		FrameManager::singleton().draw();
		fc->draw();
	}

	void buttonUp(Gosu::Button btn) {
		// kbNumpadAdd makes Johnny fat.
		if (btn == Gosu::kbNumpadAdd) {
			Frame& johnny = dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"Johnny"));
			johnny.setBorderWidth(johnny.borderWidth()+2);
		}
		// kbNumpadSubtract makes Johnny thin.
		else if (btn == Gosu::kbNumpadSubtract) {
			Frame& johnny = dynamic_cast<Frame&>(FrameManager::singleton().namedWidget(L"Johnny"));
			johnny.setBorderWidth(johnny.borderWidth()-2);
		}

		// Scroll in the list by key arrows.
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

		FrameManager::singleton().buttonUp(btn);
	}

	void buttonDown(Gosu::Button btn) {
		FrameManager::singleton().buttonDown(btn);
	}
};

int main(int argc, char* argv[]) {
#if _MSC_VER >= 1600
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	FrameTestWindow* win = new FrameTestWindow();
	win->show();
	delete win;
}