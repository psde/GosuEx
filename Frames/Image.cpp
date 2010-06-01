#include "Image.hpp"
#include "FrameManager.hpp"
#include <Gosu/Directories.hpp>

using namespace GosuEx::Frames;
struct Image::Impl {
	boost::shared_ptr<Gosu::Image> image;
	Color imageColor;
};

Image::Image(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color):
	Widget(x, y, z, image->width(), image->height()), pimpl(new Impl)
{
	pimpl->imageColor = color;
	setImage(image);
}

Image::Image(Unit x, Unit y, Unit z, Gosu::Image* image, Color color):
	Widget(x, y, z, image->width(), image->height()), pimpl(new Impl)
{
	pimpl->imageColor = color;
	setImage(image);
}


Image::Image(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable, Color color):
	Widget(x, y, z, 0, 0), pimpl(new Impl)
{
	pimpl->imageColor = color;
	setImage(filename, tileable);
}

Image::~Image() {}

void Image::setImageColor(Color newColor) { pimpl->imageColor = newColor; }
void Image::setAllImageColors(Color newColor) { setImageColor(newColor); }

Color Image::defaultImageColor() const { return pimpl->imageColor; }
Color Image::imageColor() const { return defaultImageColor(); }
boost::shared_ptr<Gosu::Image> Image::image() const { return defaultImage(); }
boost::shared_ptr<Gosu::Image> Image::defaultImage() const { return pimpl->image; }

void Image::setImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl->image = newImage; }
void Image::setImage(Gosu::Image* newImage) { pimpl->image.reset(newImage); }
void Image::setImage(const std::wstring& filename, bool tileable) { pimpl->image.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }

void Image::setAllImages(boost::shared_ptr<Gosu::Image> newImage) { setImage(newImage); }
void Image::setAllImages(Gosu::Image* newImage) { setImage(newImage); }
void Image::setAllImages(const std::wstring& filename, bool tileable) { setImage(filename, tileable); }

void Image::draw() {
	if (!shouldDraw()) 
		return;
	image()->draw(dispX(), dispY(), z(), factorX(), factorY(), imageColor());
	Widget::draw();
}

Unit Image::width() const { return image()->width(); }
Unit Image::height() const { return image()->height(); }

struct ExtImage::Impl {
	Color disabledImageColor, highlightedImageColor, disabledHighlightImageColor;
	boost::shared_ptr<Gosu::Image> disabledImage, highlightedImage, disabledHighlightImage;
};

ExtImage::ExtImage(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color, Color highlightedImageColor, Color disabledImageColor, Color disabledHighlightImageColor):
	Image(x, y, z, image, color), pimpl(new Impl)
{
	if (highlightedImageColor != Colors::none)
		pimpl->highlightedImageColor = highlightedImageColor;
	else
		pimpl->highlightedImageColor = color;

	if (disabledImageColor != Colors::none)
		pimpl->disabledImageColor = disabledImageColor;
	else
		pimpl->disabledImageColor = color;
	
	if (disabledHighlightImageColor != Colors::none)
		pimpl->disabledHighlightImageColor = disabledHighlightImageColor;
	else
		pimpl->disabledHighlightImageColor = color;
	
	setAllImages(defaultImage());
}

ExtImage::ExtImage(Unit x, Unit y, Unit z, Gosu::Image* image, Color color, Color highlightedImageColor, Color disabledImageColor, Color disabledHighlightImageColor):
	Image(x, y, z, image, color), pimpl(new Impl)
{				
	if (highlightedImageColor != Colors::none)
		pimpl->highlightedImageColor = highlightedImageColor;
	else
		pimpl->highlightedImageColor = color;
	
	if (disabledImageColor != Colors::none)
		pimpl->disabledImageColor = disabledImageColor;
	else
		pimpl->disabledImageColor = color;
	
	if (disabledHighlightImageColor != Colors::none)
		pimpl->disabledHighlightImageColor = disabledHighlightImageColor;
	else
		pimpl->disabledHighlightImageColor = color;
	
	setAllImages(defaultImage());
}

ExtImage::ExtImage(Unit x, Unit y, Unit z, const std::wstring& filename, bool tileable, Color color, Color highlightedImageColor, Color disabledImageColor, Color disabledHighlightImageColor):
	Image(x, y, z, filename, tileable, color), pimpl(new Impl)
{
	if (highlightedImageColor != Colors::none)
		pimpl->highlightedImageColor = highlightedImageColor;
	else
		pimpl->highlightedImageColor = color;

	if (disabledImageColor != Colors::none)
		pimpl->disabledImageColor = disabledImageColor;
	else
		pimpl->disabledImageColor = color;
	
	if (disabledHighlightImageColor != Colors::none)
		pimpl->disabledHighlightImageColor = disabledHighlightImageColor;
	else
		pimpl->disabledHighlightImageColor = color;
	
	setAllImages(defaultImage());
}

ExtImage::~ExtImage() {}

void ExtImage::setDisabledImageColor(Color newColor) { pimpl->disabledImageColor = newColor; }
void ExtImage::setHighlightedImageColor(Color newColor) { pimpl->highlightedImageColor = newColor; }
void ExtImage::setDisabledHighlightImageColor(Color newColor) { pimpl->disabledHighlightImageColor = newColor; }

Color ExtImage::disabledImageColor() const { return pimpl->disabledImageColor; }
Color ExtImage::highlightedImageColor() const { return pimpl->highlightedImageColor; }
Color ExtImage::disabledHighlightImageColor() const { return pimpl->disabledHighlightImageColor; }

boost::shared_ptr<Gosu::Image> ExtImage::disabledImage() const { return pimpl->disabledImage; }
boost::shared_ptr<Gosu::Image> ExtImage::highlightedImage() const { return pimpl->highlightedImage; }
boost::shared_ptr<Gosu::Image> ExtImage::disabledHighlightImage() const { return pimpl->disabledHighlightImage; }

void ExtImage::setDisabledImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl->disabledImage = newImage; }
void ExtImage::setDisabledImage(Gosu::Image* newImage) { pimpl->disabledImage.reset(newImage); }
void ExtImage::setDisabledImage(const std::wstring& filename, bool tileable) { pimpl->disabledImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }
void ExtImage::setHighlightedImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl->highlightedImage = newImage; }
void ExtImage::setHighlightedImage(Gosu::Image* newImage) { pimpl->highlightedImage.reset(newImage); }
void ExtImage::setHighlightImage(const std::wstring& filename, bool tileable) { pimpl->highlightedImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }
void ExtImage::setDisabledHighlightImage(boost::shared_ptr<Gosu::Image> newImage) { pimpl->disabledHighlightImage = newImage; }
void ExtImage::setDisabledHighlightImage(Gosu::Image* newImage) { pimpl->disabledHighlightImage.reset(newImage); }
void ExtImage::setDisabledHighlightImage(const std::wstring& filename, bool tileable) { pimpl->disabledHighlightImage.reset(new Gosu::Image(FrameManager::singleton().graphics(), filename, tileable)); }

void ExtImage::setAllImages(boost::shared_ptr<Gosu::Image> newImage) {
	Image::setAllImages(newImage);
	setDisabledImage(newImage);
	setHighlightedImage(newImage);
	setDisabledHighlightImage(newImage);
}

void ExtImage::setAllImages(Gosu::Image* newImage) {
	Image::setAllImages(newImage);
	setDisabledImage(newImage);
	setHighlightedImage(newImage);
	setDisabledHighlightImage(newImage);
}

void ExtImage::setAllImages(const std::wstring& filename, bool tileable) {
		Image::setAllImages(filename, tileable);
		// WATCH OUT!
		// We loaded the image now. We can use defaultImage().
		setAllImages(defaultImage());
}

boost::shared_ptr<Gosu::Image> ExtImage::image() const {
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

void ExtImage::setAllImageColors(Color newColor) {
	Image::setAllImageColors(newColor);
	setDisabledImageColor(newColor);
	setHighlightedImageColor(newColor);
	setDisabledHighlightImageColor(newColor);
}

Color ExtImage::imageColor() const {
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

boost::shared_ptr<Gosu::Image> GosuEx::loadImage(Gosu::Graphics& graphics, const std::wstring& filename, bool tileable) {
	boost::shared_ptr<Gosu::Image> ptr(new Gosu::Image(graphics, Gosu::resourcePrefix()+filename, tileable));
	return ptr;
}

	//! If you give it a Image* it won't do that automagic.
boost::shared_ptr<Gosu::Image> GosuEx::loadImage(Gosu::Image* image) {
	boost::shared_ptr<Gosu::Image> ptr(image);
	return ptr;
}