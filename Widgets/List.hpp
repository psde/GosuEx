#ifndef GOSUEX_WIDGETS_LIST_HPP
#define GOSUEX_WIDGETS_LIST_HPP

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>
#include <map>

namespace GosuEx {
		namespace Widgets {

				template<typename Key> class List : public Widget {
						struct Impl {
								typename std::map<Key, WidgetP> elements;
								typename std::map<Key, WidgetP>::iterator it;
								std::size_t index, visElements;
						};
						boost::scoped_ptr<Impl> pimpl;
				public:
						typedef boost::shared_ptr<List<Key> > P;
						List(double x, double y, double z, double width, double maxHeight):
						pimpl(new Impl), Widget(x, y, z, width, maxHeight) {
								pimpl->index = pimpl->visElements = 0;
								pimpl->it = pimpl->elements.begin();
						}
						~List() {}

						virtual void update() {
								if (!shouldHandleUpdate()) return;
								if (!visibleElements() || !elements()) return;

								std::map<Key, WidgetP>::iterator it = pimpl->it;
								for (std::size_t i = visibleElements(); i && it != pimpl->elements.end(); i--, ++it) {
										it->second->update();
								}
						}

						virtual void draw() {
								if (!shouldHandleDraw()) return;
								if (!visibleElements() || !elements()) return;
								std::map<Key, WidgetP>::iterator it = pimpl->it;
								for (std::size_t i = visibleElements(); i && it != pimpl->elements.end(); i--, ++it) {
										it->second->draw();
								}
						}
						
						virtual void buttonUp(Gosu::Button btn) {
								if (!shouldHandleButtonUp()) return;
								if (!visibleElements() || !elements()) return;
								std::map<Key, WidgetP>::iterator it = pimpl->it;
								for (std::size_t i = visibleElements(); i && it != pimpl->elements.end(); i--, ++it) {
										it->second->buttonUp(btn);
								}
						}

						virtual void buttonDown(Gosu::Button btn) {
								if (!shouldHandleButtonDown()) return;
								if (!visibleElements() || !elements()) return;
								std::map<Key, WidgetP>::iterator it = pimpl->it;
								for (std::size_t i = visibleElements(); i && it != pimpl->elements.end(); i--, ++it) {
										it->second->buttonDown(btn);
								}
						}

						void addElement(const Key& key, Widget* element) {
								if (!element->ptr()) {
										WidgetP nptr(element);
										element->setPtr(nptr);
										addElement(key, element->ptr());
										return;
								}
								addElement(key, element->ptr());
						}

						void addElement(const Key& key, WidgetP element) { 
								pimpl->elements.insert(std::pair<Key, WidgetP>(key, element));
								element->setGosuGraphics(gosuGraphics());
								element->setGosuInput(gosuInput());
								element->setX(x()-relX()*width()*factorX()+width()*factorX()/2);
								element->setRelations(0.5, 0);
								setIndex(index());
						}

						void removeElement(WidgetP element) {
								for (std::map<Key, WidgetP>::iterator it = pimpl->elements.begin();	it != pimpl->elements.end(); ++it) {
												if (it->second == element) {
														pimpl->elements.erase(it);
														setIndex(index());
														return;
												}
								}
						}

						void removeElement(const Key& key) {
								pimpl->elements.erase(pimpl->elements.find(key));
								setIndex(index());
						}

						WidgetP element(Key index) const {
								return pimpl->elements.find(index)->second;
						}

						std::size_t elements() const { return pimpl->elements.size(); }

						std::size_t index() const { return pimpl->index; }
						
						std::size_t visibleElements() const { return pimpl->visElements; }

						// Returns true if the list is full.
						void setIndex(std::size_t newIndex) {
								if (newIndex > elements()) { newIndex = elements()-1; }
								// Reset our iterator
								pimpl->it = pimpl->elements.begin();
								pimpl->index = newIndex;
								for (std::size_t i = newIndex; i; i--)
										pimpl->it++;
								// Von dort aus ausgehend, schaue wieviele Elemente angezeigt werden sollen.
								pimpl->visElements = 0;
								double h = 0;
								for (std::map<Key, WidgetP>::iterator it = pimpl->it; it != pimpl->elements.end(); ++it) {
										if ((h += it->second->height()) > height()) break;
										pimpl->visElements++;
								}
								reset();
						}

						// HAACKS
						// Returns true if the list is full.
						void scrollBy(int offset) {
								setIndex(index()+max(-static_cast<int>(index()), offset));
						}

						template<class Compare> void sort(Compare comp) {
								pimpl->elements.swap(std::map<Key, WidgetP>(pimpl->elements.begin(), pimpl->elements.end(), comp));
						}
				protected:
						void reset() {
								Widget::reset();
								double oy = y();
								std::map<Key, WidgetP>::iterator it = pimpl->it;
								for (std::size_t i = visibleElements(); i && it != pimpl->elements.end(); i--, ++it) {
										it->second->setY(oy);
										oy += it->second->height();
								}
						}
				};
		}
}
#endif