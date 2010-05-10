#ifndef GOSUEX_FRAMES_LIST_HPP
#define GOSUEX_FRAMES_LIST_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/FrameManager.hpp>

namespace GosuEx {
	namespace Frames {
		template<typename TBack, typename TKey, typename TElement> BasicList : public TBack {
			struct Impl {
				typename std::map<TKey, TElement*> elements;
				typename std::map<TKey, TElement*>::iterator iterator;
				Unit index, visibleElements;
			} pimpl;
		public:
			BasicList(Unit x, Unit y, Unit z, Unit width, Unit height):
				TBack(x, y, z, width, height)
			{
				pimpl.index = pimpl.visibleElements = 0;
				pimpl.iterator = pimpl.elements.begin();
			}
			
			/*virtual void update() {
				if (!shouldUpdate())
					return;
				if (!visibleElements() || !elements())
					return;
				std::map<TKey, TElement*>::iterator it = pimpl.it;
				for (std::size_t i = visibleElements(); i && it != pimpl.elements.end(); i--, ++it) {
					it->second->update();
				}
			}*/

			virtual void draw() {
				if (!shouldDraw())
					return;
				std::map<TKey, TElement*>::iterator it = pimpl.it;
				for (std::size_t i = visibleElements(); i && it != pimpl.elements.end(); i--, ++it) {
					it->second->update();
				}
			}


			void createElement(const TKey& key, TElement* element) {
				FrameManager::singleton().addWidget(element);
				addElement(key, element);
			}

			void addElement(const TKey& key, TElement* element) {
				pimpl.elements.insert(std::pair<TKey, TElement*>(key, element));
				element->setParent(this);
				element->setX(dispX());
				element->setRelX(0.5);
				setIndex(index());
			}
			
			void removeElement(const TKey& key) {
				pimpl.elements.erase(key);
			}

			void deleteElement(const TKey& key) {
				TElement* el = element(key);
				removeElement(key);
				FrameManager::singleton().deleteWidget(el);
			}

			TElement* element(const TKey& key) {
				return pimpl.elements.find(key)->second;
			}

			std::size_t elements() const { return pimpl.elements.size(); }

			std::size_t index() const { return pimpl.index; }

			std::size_t visibleElements() const { return pimpl.visibleElements; }

			void setIndex(std::size_t newIndex) {
				if (newIndex > elements()) { newIndex = elements()-1; }
				// Reset our iterator
				pimpl.it = pimpl.elements.begin();
				pimpl.index = newIndex;

				for (std::size_t i = newIndex; i; i--)
					pimpl.it++;

				pimpl.visibleElements = 0;
				Unit h = 0;
				for (std::map<TKey, TElement>::iterator it = pimpl.it; it != pimpl.elements.end(); ++it) {
					if ((h += it->second->dispHeight()) > dispHeight()) break;
					pimpl.visibleElements++;
				}
			}

			void scrollBy(int offset) {
				setIndex(index()+max(-static_cast<int>(index()), offset));
			}

			template<class Compare> void sort(Compare comp) {
				pimpl.elements.swap(std::map<TKey, TElement>(pimpl.elements.begin(), pimpl.elements().end(), comp));
			}

public:
			void reset() {
				Unit oy = y();
				std::map<TKey, TElement>::iterator it = pimpl.it;
				for (std::size_t i = visibleElements(); i && it != pimpl.elements.end(); i--, ++it) {
					it->second->setY(oy);
					oy += it->second->dispHeight();
				}
			}
		};

		typedef<
	}
}
#endif