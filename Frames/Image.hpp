#ifndef GOSUEX_FRAMES_IMAGE_HPP
#define GOSUEX_FRAMES_IMAGE_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <Gosu/Image.hpp>

namespace GosuEx {
	namespace Frames {
		class Image : public Widget {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			Image(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color = Colors::white);
			Image(Unit x, Unit y, Unit z, Gosu::Image* image, Color color = Colors::white);
			Image(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable = false, Color color = Colors::white);

			virtual ~Image();
			void setImageColor(Color newColor);
			void setAllImageColors(Color newColor);

			Color defaultImageColor() const;
			virtual Color imageColor() const;
			virtual boost::shared_ptr<Gosu::Image> image() const;
			boost::shared_ptr<Gosu::Image> defaultImage() const;

			void setImage(boost::shared_ptr<Gosu::Image> newImage);
			void setImage(Gosu::Image* newImage);
			void setImage(const std::wstring& filename, bool tileable = false);

			void setAllImages(boost::shared_ptr<Gosu::Image> newImage);
			void setAllImages(Gosu::Image* newImage);
			void setAllImages(const std::wstring& filename, bool tileable = false);

			virtual void draw();

			virtual Unit width() const;
			virtual Unit height() const;
		};

		class ExtImage : public Image {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			ExtImage(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none);
			ExtImage(Unit x, Unit y, Unit z, Gosu::Image* image, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none);
			ExtImage(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable = false, Color color = Colors::white, Color highlightedImageColor = Colors::none, Color disabledImageColor = Colors::none, Color disabledHighlightImageColor = Colors::none);
			virtual ~ExtImage();

			void setDisabledImageColor(Color newColor);
			void setHighlightedImageColor(Color newColor);
			void setDisabledHighlightImageColor(Color newColor);
			virtual void setAllImageColors(Color newColor);

			virtual Color imageColor() const;
			Color disabledImageColor() const;
			Color highlightedImageColor() const;
			Color disabledHighlightImageColor() const;

			boost::shared_ptr<Gosu::Image> disabledImage() const;
			boost::shared_ptr<Gosu::Image> highlightedImage() const;
			boost::shared_ptr<Gosu::Image> disabledHighlightImage() const;

			void setDisabledImage(boost::shared_ptr<Gosu::Image> newImage);
			void setDisabledImage(Gosu::Image* newImage);
			void setDisabledImage(const std::wstring& filename, bool tileable = false);
			void setHighlightedImage(boost::shared_ptr<Gosu::Image> newImage);
			void setHighlightedImage(Gosu::Image* newImage);
			void setHighlightImage(const std::wstring& filename, bool tileable = false);
			void setDisabledHighlightImage(boost::shared_ptr<Gosu::Image> newImage);
			void setDisabledHighlightImage(Gosu::Image* newImage);
			void setDisabledHighlightImage(const std::wstring& filename, bool tileable = false);

			virtual void setAllImages(boost::shared_ptr<Gosu::Image> newImage);
			virtual void setAllImages(Gosu::Image* newImage);
			virtual void setAllImages(const std::wstring& filename, bool tileable = false);

			virtual boost::shared_ptr<Gosu::Image> image() const;

		};
	}

	// Automagic
	boost::shared_ptr<Gosu::Image> loadImage(Gosu::Graphics& graphics, const std::wstring& filename, bool tileable = false);

	//! If you give it a Image* it won't do that automagic.
	boost::shared_ptr<Gosu::Image> loadImage(Gosu::Image* image);
}
#endif