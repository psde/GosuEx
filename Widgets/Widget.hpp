#ifndef GOSUEX_WIDGETS_WIDGET_HPP
#define GOSUEX_WIDGETS_WIDGET_HPP

/**
 * GosuEx::Widgets is an 'GosuExtension' to create userinterfaces quick and easy.
	* It's based on a Widget System which means that everything you do
	* is a widget; you have a root widget and all your UI elements are a 'child' of
	* another widget. You can put as many widgets as you want in another*, you could
	* even put one widget in two containers - although I do not recommend that.
	* You _have_ to store Widgets as a WidgetP. Everything else is just wrong.
	* 
	* All widgets have at least two attributes which may be important for you
	* 'enabled' - an enabled widget will fire callbacks (and allow children to do that, too)
	* 'visible' - a visible widget, well, it's visible. A hidden widget is like a disabled widget,
	*             it won't fire any callbacks neither will its children.
	*
	*
	* Okay, I would like to speak with you about birds and bees
	* The birds and the bees are lying in the trees
	*
	* ABOUT "CALLBACKS"
	* Callbacks are a nice way to let the user interact with the interface.
	* A normal widget 
	* However, the basic widgets you see in this lib are nothing more than rects
	* So, onHover/onBlur will be called when the user moves over this rect
	* Let's have this edge as an image with x as some nice colors:
	*     
	*  P /-----------|
	*  C/XXXXXXXXXXXX|
	*  /XXXXXXXXXXXXX|
	*  |-------------E
	* Even if C is not part of the image (its transparency is 100%), a click on C will call
	* on[Right|Left]Click. Also, if the mouse enters P it will call onHover. Because the widget thinks
	* it's a rect, beginning with P and ending with E. Any point between this rect will be in the widget
	* every point could trigger onHover or on[Right|Left]Click.
	* Okay, now some tricks.
	* - I recommend to use onLeftRelease/onRightRelease if you plan to do a button.
	*   Because I often click a button, think "O noes" and move the mouse out of any button (or on the right button)
	*   even when the "clicked" button will have another color (that's fine) or something like that, it's nice
	*   if the event is called when the user releases the mouse. But, just as a tip, of course
	*   is it possible to use onLeftClick.
	* - If you unset the internal "Is the mouse aiming at me?" variable (setEnabled, setVisibility, setGosuInput(NULL), setSize(0,0))
	*   you won't get any onBlur/on[Right|Left][Click|Release] events!
	* 
	* Short:
	* - on[Left|Right]Release will be called (if even) from the widget the mouse is NOW over.
	* It's really late, I think this should be informative enough. brb, getting something to eat.
	*
	* * May there are some memory restrictions.
*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/Input.hpp>

namespace GosuEx {
		namespace Widgets {

				class Widget : boost::noncopyable {
						// The 'data'
						struct Impl;
						// Pointer to this data
						boost::scoped_ptr<Impl> pimpl;
				public:
						/// Constructors, Destructors, Cows
						// Initializing just as a container
						Widget();
						// Initializing as a rect
						Widget(double x, double y, double z, double width, double height);
						// Destructor
						virtual ~Widget();

						/// foo
						// Enable this widget. See setEnabled.
						void enable() { setEnabled(true); }
						// Disable this widget. See setEnabled.
						void disable() { setEnabled(false); }
						// Show this widget. See setVisibility
						void show() { setVisibility(true); }
						// Hide this widget. See setVisibility.
						void hide() { setVisibility(false); }
						// Attachs an effect
						void attachEffect(Effects::EffectP effect);
						// Detachs the effect
						void detachEffect();
						// This function _can_ help you with memory leaks
						// It destroys all functions so you're able to use sharedPtr.
						// Call this function on the rootWidget on ~Window().
						virtual void cleanUp();

						// Should be called with Window::update.
						virtual void update();
						// Should be called with Window::draw
						virtual void draw();
						// Should be called with Window::buttonDown
						virtual void buttonDown(Gosu::Button btn);
						// Should be called with Window::buttonUp
						virtual void buttonUp(Gosu::Button btn);

						/// setFoo
						// Sets new x position.
						virtual void setX(double newX) { setPosition(newX, y(), z()); }
						// Sets new y position.
						virtual void setY(double newY) { setPosition(x(), newY, z()); }
						// Sets new z position.
						void setZ(double newZ) { setPosition(x(), y(), newZ); }
						// Sets new position.
						virtual void setPosition(double newX, double newY, double newZ);
						// Sets new x relation
						void setRelX(double newX) { setRelations(newX, relY()); }
						// Sets new y relation
						void setRelY(double newY) { setRelations(relX(), newY); }
						// Sets new relations
						void setRelations(double newX, double newY);
						// Sets new x factor
						void setFactorX(double newX) { setFactors(newX, factorY()); }
						// Sets new y factor
						void setFactorY(double newY) { setFactors(factorX(), newY); }
						// Sets factors
						void setFactors(double newX, double newY);
						// Sets new width.
						void setWidth(double newWidth) { setSize(newWidth, height()); }
						// Sets new height.
						void setHeight(double newHeight) { setSize(width(), newHeight); }
						// Sets new size.
						void setSize(double newWidth, double newHeight);
						// Enabled/Disabled.
						// A disabled widget won't redirect any buttonDown, buttonUp or update events to its children
						// nor will it call any callbacks.
						// Setting it to 'false' will reset aimed.
						virtual void setEnabled(bool enabled);
						// Visible/Hidden
						// A hidden widget acts like a disabled widget - except it's not visible either.
						// See setEnabled.
						void setVisibility(bool visible);
						// In order for some widgets to work, it's required to set this.
						// When you add a new widget it will inherit this values from its parent.
						virtual void setGosuGraphics(Gosu::Graphics* graphics);
						// See setGraphics, just for Input.
						virtual void setGosuInput(Gosu::Input* input);
						// Do not call this. It's reserved for "factory"-functions.
						void setPtr(WidgetP ptr);

						/// "get"Foo
						// Returns x position. Throws an std::logic_error if used with a normal layer or container.
						double x() const;
						// Returns y position. Throws an std::logic_error if used with a normal layer or container.
						double y() const;
						// Returns z position. Throws an std::logic_error if used with a normal layer or container.
						double z() const;
						// Returns x relation.
						double relX() const;
						// Returns y relation
						double relY() const;
						// Returns x factor (das unfassbare)
						double factorX() const;
						// Returns y factor (das fassbare)
						double factorY() const;
						// Returns width. Throws an std::logic_error if used with a normal layer or container.
						double width() const;
						// Returns height. Throws an std::logic_error if used with a normal layer or container.
						double height() const;
						// Returns if this widget is enabled
						bool enabled() const;
						// Returns if this widget is visible
						bool visible() const;
						// Returns the childrencount.
						std::size_t size() const;
						// Returns if the mouse is "above" this widget
						// <serious!!111>2do: if you leftclick call headshot()</serious!!111>
						// When this function WILL return false, EITHER:
						// - no width
						// - no height
						// - no gosuInput
						// - is disabled
						// - the mouse was not above the widget when last update()d
						bool aimed() const;
						// if we have a possible left/rightClick
						bool leftClick() const;
						bool rightClick() const;

						// Returns the pointer to this object.
						WidgetP ptr();

						// Returns the weak pointer.
						WidgetWP wptr();

						// Returns a casted pointer to this object
						template<class T> boost::shared_ptr<T> cptr() {
								return boost::shared_dynamic_cast<T, Widget>(ptr());
						}

						template<class T> boost::weak_ptr<T> cwptr() {
								return boost::weak_ptr<T>(cptr<T>());
						}

						// Returns the saved Gosu::Graphics*
						Gosu::Graphics* gosuGraphics() const;
						// Returns the saved Gosu::Input*
						Gosu::Input* gosuInput() const;
						// Returns the pointer to the index-th child. Will throw an exception if index is... 'wrong'
						WidgetP child(std::size_t index);
						// Returns the effect, if there is one
						Effects::EffectP effect();

						// Returns the index-th child and converts it to the right class. Will throw an exception if this is not possible.
						// The 'c' stands for 'converted' or 'casted', whatever you want.
						// whatever you like.
						// whatever you 
						template<class T> boost::shared_ptr<T> cChild(std::size_t index) {
								return boost::shared_dynamic_cast<T, Widget>(this->child(index));
						}

						/// Operators
						// You can do 
						// *parent << child1 << child2 << child3 
						// to add as many childs to one parent as you want.
						// The first widget in this row is always the parent.
						Widget& operator<<(WidgetP child);
						Widget& operator<<(Widget* child);

						// Same like ptr(). You should not have access to the "real" pointer.
						// Because this could things really mess up.
						WidgetP operator&();
						
						/// CALLBACKS
						// All callbacks will (normally) return the shared_ptr to the widget.
						// You are welcome to overload that.
						// For example:
						// <code where="inheritance of Gosu::Window">
						// GosuEx::Widgets::ColorableP ourWidget;
						// GosuEx::Widgets::Widget anotherWidget = ...; // it doesn't matter where/how it's defined
						// *anotherWidget
						//   << GosuEx::Widgets::createWidget(new GosuEx::Widgets::Colorable(...), ourWidget)
						// ;
						// ourWidget->onHover = boost::bind(&Window::weAreHappy, this, ourWidget);
						// // If we want a WidgetP instead of ColorableP (why ever ...), we would have to use
						// // ourWidget->onHover = boost::bind(&Window::weAreHappy, this, _1);
						// //
						// // The weAreHappy function looks like this, please see that we could use
						// // a WidgetP instead of ColorableP if we use the code above (with _1)
						// // void Window::weAreHappy(GosuEx::Widgets::ColorableP happyWidget) { ... }
						// </code>

						// This is called when the mouse wents over the widget
						// won't be called for disabled, invisible, no-height, no-width,
						// no-gosu-input widgets.
						boost::function<void(WidgetP)> onHover;
						
						// As soon as the mouse leaves our widget
						// Note: If you "harddisable" hovering with setEnabled, setVisibility, or
						// setGosuInput(NULL), this event WON'T be called!
						// Same condition like onHover
						boost::function<void(WidgetP)> onBlur;

						// As soon as the mouse is above this widget and Gosu::msLeft is clicked
						// Same conditions like onHover
						boost::function<void(WidgetP)> onLeftDown;
						
						// See onLeftClick, just for the other mousething
						boost::function<void(WidgetP)> onRightDown;
						
						// If the mouse is released above us.
						boost::function<void(WidgetP)> onLeftUp;
						
						// If the mouse is released above us.
						boost::function<void(WidgetP)> onRightUp;
						
						// If we were pressed and released on this widget.
						boost::function<void(WidgetP)> onLeftClick;
						boost::function<void(WidgetP)> onRightClick;

						// JOIN THE PROTECTED SIDE
				protected:
						// sometimes, widgets just have to reset themselves.
						// for example if they have changed majorly. (HAH! FASTERLY!)
						// This function does the trick. It's empty by default.
						virtual void reset();
						// This function is called when update()d. It returns true if the mouse is "above" this widget,
						// otherwise false. usually widgets will save the result which is available with aimed()
						// if no width or height is specified or if there is no gosuInput, it is _not_ called
						virtual bool checkAiming();
						// Should we handle draw?
						virtual bool shouldHandleDraw();
						// Should we handle update?
						virtual bool shouldHandleUpdate();
						// Should we handle buttonDown?
						virtual bool shouldHandleButtonDown();
						// Should we handle buttonUp?
						virtual bool shouldHandleButtonUp();
				};

				/**
				* This function creates a rootWidget for you.
				* Usage: rootWidget = GosuEx::Widgets::createRootWidget(&graphics(), &input());
				* this is _not_ a superior class management thing or something like that.
				* You should pass graphics() and input() so every child of this rootWidget has it.
				*/

				inline WidgetP createRootWidget(Gosu::Graphics* graphics, Gosu::Input* input) {
						Widget* rw = new Widget();
						rw->setGosuGraphics(graphics);
						rw->setGosuInput(input);
						WidgetP nptr(rw);
						rw->setPtr(nptr);
						return rw->ptr();
				}

				/**
				* This function is useful(l?) if you have to add a widget
				* and get its sharedptr instantly.
				* Usage: << createWidget(new {YourWidgetTypeHere}({ConstructorDataHere}), {YourWidgetP-PointerHere})
				* or: << createWidget(new {YourWidgetTypeHere}({ConstructorDataHere}))
				*/
				template<class T> inline T* createWidget(T* widget, boost::shared_ptr<T>& ptr) {
						if (!widget->ptr()) {
								boost::shared_ptr<T> nptr(widget);
								widget->setPtr(nptr);
								ptr = boost::shared_dynamic_cast<T, Widget>(widget->ptr());
								return widget;
						}
						ptr = boost::shared_dynamic_cast<T, Widget>(widget->ptr());
						return widget;
				}

				template<class T> inline WidgetP createWidget(T* widget) {
						if (!widget->ptr()) {
								boost::shared_ptr<T> nptr(widget);
								widget->setPtr(nptr);
								return widget->ptr();
						}
						return widget->ptr();
				}

		}
}
#endif