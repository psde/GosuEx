#ifndef GOSUEX_FRAMES_IMAGE_HPP
#define GOSUEX_FRAMES_IMAGE_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <boost/shared_ptr.hpp>
#include <Gosu/Image.hpp>

namespace GosuEx {
	namespace Frames {
		class Image : public Widget {
			struct Impl {
				boost::shared_ptr<Gosu::Image> image;
				Color imageColor;
			} pimpl;
		public:
			Image(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color = Colors::white):
				Widget(x, y, z, image->width(), image->height())
			{
				pimpl.imageColor = color;
				//pimpl.image = image;
				setImage(image);
			}

			Image(Unit x, Unit y, Unit z, Gosu::Image* image, Color color = Colors::white):
				Widget(x, y, z, image->width(), image->height())
			{
				pimpl.imageColor = color;
				setImage(image);
				//pimpl.image.reset(image);
			}

			Image(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable = false, Color color = Colors::white):
				Widget(x, y, z, 0, 0)
			{
				pimpl.imageColor = color;
				setImage(filename, tileable);
				//pimpl.image.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable));
			}

			void setImageColor(Color newColor) { pimpl.imageColor = newColor; }
			void setAllImageColors(Color newColor) { setImageColor(newColor); }

			Color defaultImageColor() const { return pimpl.imageColor; }
			virtual Color imageColor() const { return defaultImageColor(); }
			virtual boost::shared_ptr<Gosu::Image> image() const { return defaultImage(); }
			boost::shared_ptr<Gosu::Image> defaultImage() const { return pimpl.image; }

			void setImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl.image = newImage; }
			void setImage(Gosu::Image* newImage) { pimpl.image.reset(newImage); }
			void setImage(const std::wstring& filename, bool tileable = false) { pimpl.image.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }

			void setAllImages(boost::shared_ptr<Gosu::Image> newImage) { setImage(newImage); }
			void setAllImages(Gosu::Image* newImage) { setImage(newImage); }
			void setAllImages(const std::wstring& filename, bool tileable = false) { setImage(filename, tileable); }

			virtual void draw() {
				if (!shouldDraw()) 
					return;
				image()->draw(dispX(), dispY(), z(), factorX(), factorY(), imageColor());
			}

			virtual Unit width() const { return image()->width(); }
			virtual Unit height() const { return image()->height(); }
		};

		class ExtImage : public Image {
			struct Impl {
				Color disabledImageColor, highlightedImageColor, disabledHighlightImageColor;
				boost::shared_ptr<Gosu::Image> disabledImage, highlightedImage, disabledHighlightImage;
			} pimpl;
		public:
			ExtImage(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none):
				Image(x, y, z, image, color)
			{
				if (highlightedImageColor != Colors::none)
					pimpl.highlightedImageColor = highlightedImageColor;
				else
					pimpl.highlightedImageColor = color;
				if (disabledImageColor != Colors::none)
					pimpl.disabledImageColor = disabledImageColor;
				else
					pimpl.disabledImageColor = color;
				if (disabledHighlightImageColor != Colors::none)
					pimpl.disabledHighlightImageColor = disabledHighlightImageColor;
				else
					pimpl.disabledHighlightImageColor = color;
				setAllImages(defaultImage());
			}

			ExtImage(Unit x, Unit y, Unit z, Gosu::Image* image, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none):
				Image(x, y, z, image, color)
			{				
				if (highlightedImageColor != Colors::none)
					pimpl.highlightedImageColor = highlightedImageColor;
				else
					pimpl.highlightedImageColor = color;
				if (disabledImageColor != Colors::none)
					pimpl.disabledImageColor = disabledImageColor;
				else
					pimpl.disabledImageColor = color;
				if (disabledHighlightImageColor != Colors::none)
					pimpl.disabledHighlightImageColor = disabledHighlightImageColor;
				else
					pimpl.disabledHighlightImageColor = color;
				setAllImages(defaultImage());
			}

			ExtImage(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable = false, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none):
				Image(x, y, z, filename, tileable, color)
			{
				if (highlightedImageColor != Colors::none)
					pimpl.highlightedImageColor = highlightedImageColor;
				else
					pimpl.highlightedImageColor = color;
				if (disabledImageColor != Colors::none)
					pimpl.disabledImageColor = disabledImageColor;
				else
					pimpl.disabledImageColor = color;
				if (disabledHighlightImageColor != Colors::none)
					pimpl.disabledHighlightImageColor = disabledHighlightImageColor;
				else
					pimpl.disabledHighlightImageColor = color;
				setAllImages(defaultImage());
			}

			void setDisabledImageColor(Color newColor) { pimpl.disabledImageColor = newColor; }
			void setHighlightedImageColor(Color newColor) { pimpl.highlightedImageColor = newColor; }
			void setDisabledHighlightImageColor(Color newColor) { pimpl.disabledHighlightImageColor = newColor; }

			Color disabledImageColor() const { return pimpl.disabledImageColor; }
			Color highlightedImageColor() const { return pimpl.highlightedImageColor; }
			Color disabledHighlightImageColor() const { return pimpl.disabledHighlightImageColor; }

			boost::shared_ptr<Gosu::Image> disabledImage() const { return pimpl.disabledImage; }
			boost::shared_ptr<Gosu::Image> highlightedImage() const { return pimpl.highlightedImage; }
			boost::shared_ptr<Gosu::Image> disabledHighlightImage() const { return pimpl.disabledHighlightImage; }

			void setDisabledImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl.disabledImage = newImage; }
			void setDisabledImage(Gosu::Image* newImage) { pimpl.disabledImage.reset(newImage); }
			void setDisabledImage(const std::wstring& filename, bool tileable = false) { pimpl.disabledImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }
			void setHighlightedImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl.highlightedImage = newImage; }
			void setHighlightedImage(Gosu::Image* newImage) { pimpl.highlightedImage.reset(newImage); }
			void setHighlightImage(const std::wstring& filename, bool tileable = false) { pimpl.highlightedImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }
			void setDisabledHighlightImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl.disabledHighlightImage = newImage; }
			void setDisabledHighlightImage(Gosu::Image* newImage) { pimpl.disabledHighlightImage.reset(newImage); }
			void setDisabledHighlightImage(const std::wstring& filename, bool tileable = false) { pimpl.disabledHighlightImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }

			virtual void setAllImages(boost::shared_ptr<Gosu::Image> newImage) {
				Image::setAllImages(newImage);
				setDisabledImage(newImage);
				setHighlightedImage(newImage);
				setDisabledHighlightImage(newImage);
			}

			virtual void setAllImages(Gosu::Image* newImage) {
				Image::setAllImages(newImage);
				setDisabledImage(newImage);
				setHighlightedImage(newImage);
				setDisabledHighlightImage(newImage);
			}

			virtual void setAllImages(const std::wstring& filename, bool tileable = false) {
				Image::setAllImages(filename, tileable);
				// WATCH OUT!
				// We loaded the image now. We can use defaultImage().
				setAllImages(defaultImage());
			}

			virtual boost::shared_ptr<Gosu::Image> image() const {
				if (enabled()) {
					if (highlighted())
						return highlightedImage();
					else
						return defaultImage();
				}
				else {
					if (highlighted())
						return disabledHighlightImage();
					else
						return disabledImage();
				}
			}

			virtual void setAllImageColors(Color newColor) {
				Image::setAllImageColors(newColor);
				setDisabledImageColor(newColor);
				setHighlightedImageColor(newColor);
				setDisabledHighlightImageColor(newColor);
			}

			Color imageColor() const {
				if (enabled()) {
					if (highlighted())
						return highlightedImageColor();
					else
						return defaultImageColor();
				}
				else {
					if (highlighted())
						return disabledHighlightImageColor();
					else
						return disabledImageColor();
				}
			}
		};
	}
}
#endif