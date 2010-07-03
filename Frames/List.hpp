#ifndef GOSUEX_FRAMES_LIST_HPP
#define GOSUEX_FRAMES_LIST_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/FrameManager.hpp>
#include <Gosu/Math.hpp>
#include <cmath>

/**
* Lists are a special kind of widget.
* Do not attach any children to them.
*/

namespace GosuEx {
	namespace Frames {
		template<typename TKey, typename TElement> class BasicList : public Widget {
			struct Impl {
				typename std::map<TKey, TElement*> elements;
				typename std::map<TKey, TElement*>::iterator iterator;
				std::size_t visibleElements;
				Unit index;
			} pimpl;
		public:
			BasicList(Unit x, Unit y, Unit z, Unit width, Unit height):
				Widget(x, y, z, width, height)
			{
				pimpl.index = pimpl.visibleElements = 0;
				pimpl.iterator = pimpl.elements.begin();
			}

			virtual void draw() {
				if (!this->shouldDraw())
					return;

				if (this->dispX() >= 0 && this->dispY() >= 0)
					FrameManager::singleton().graphics().beginClipping(static_cast<unsigned int>(this->dispX()), static_cast<unsigned int>(this->dispY()), static_cast<unsigned int>(this->dispWidth()), static_cast<unsigned int>(this->dispHeight()));
				
				Widget::draw();

				if (this->dispX() >= 0 && this->dispY() >= 0)
					FrameManager::singleton().graphics().endClipping();
			}

			TElement& createElement(const TKey& key, TElement* element, const std::wstring& name) {
				this->createElement(key, element);
				element->setName(name);
				FrameManager::singleton().addNamedWidget(element);
				return *element;
			}

			TElement& createElement(const TKey& key, TElement* element) {
				FrameManager::singleton().addWidget(element);
				this->addElement(key, element);
				return *element;
			}

			void addElement(const TKey& key, TElement* element) {
				pimpl.elements.insert(std::pair<TKey, TElement*>(key, element));
				element->setParent(this);
				element->setX(dispX()+dispWidth()/2);
				element->setRelX(0.5);
				element->setWidth(dispWidth());
				element->hide();
				setIndex(index());
			}

			void removeElement(const TKey& key) {
				pimpl.elements.erase(key);
			}

			void deleteElement(const TKey& key) {
				TElement* el = element(key);
				this->removeElement(key);
				FrameManager::singleton().deleteWidget(el);
			}

			TElement& element(const TKey& key) {
				return *pimpl.elements.find(key)->second;
			}

			const std::map<TKey, TElement*>& elements() const { return pimpl.elements; }

			Unit index() const { return pimpl.index; }

			std::size_t visibleElements() const { return pimpl.visibleElements; }

			// Setting a new offset/index for the list
			// 0.0 will show the first element,
			// 0.5 will show 1/2 of the first element.
			// and so on.
			void setIndex(Unit newIndex) {
				if (newIndex > static_cast<Unit>(pimpl.elements.size())) { newIndex = pimpl.elements.size()-1; }
				// Reset our iterator
				pimpl.iterator = pimpl.elements.begin();
				pimpl.index = newIndex;

				// Set the iterator to our new position, floor-ed
				for (std::size_t i = static_cast<std::size_t>(newIndex); i && pimpl.iterator != pimpl.elements.end(); i--)
					pimpl.iterator++;

				if (pimpl.iterator == pimpl.elements.end())
					return;

				pimpl.visibleElements = 0;
				
				Unit i;
				Unit h = -std::modf(newIndex, &i)*pimpl.iterator->second->dispHeight();
				
				for (std::map<TKey, TElement*>::iterator it = pimpl.iterator; it != pimpl.elements.end(); ++it) {
					pimpl.visibleElements++;
					if ((h += it->second->dispHeight()) > dispHeight()) break;
				}

				reset();
			}

			void scrollBy(Unit offset) {
				setIndex(index()+std::max(-index(), offset));
			}

			template<class Compare> void sort(Compare comp) {
				pimpl.elements.swap(std::map<TKey, TElement*>(pimpl.elements.begin(), pimpl.elements().end(), comp));
			}

public:
			void reset() {
				if (pimpl.iterator == pimpl.elements.end())
					return;

				Unit i;
				Unit oy = this->dispY()-std::modf(index(), &i)*pimpl.iterator->second->dispHeight();
				
				for (std::map<TKey, TElement*>::iterator it = pimpl.elements.begin(); it != pimpl.elements.end(); ++it)
					it->second->hide();

				std::map<TKey, TElement*>::iterator it = pimpl.iterator;
				for (std::size_t i = visibleElements(); i && it != pimpl.elements.end(); i--, ++it) {
					it->second->show();
					it->second->setY(oy);
					oy += it->second->dispHeight();
				}
			}
		};
	}
}
#endif