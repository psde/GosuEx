#ifndef GOSUEX_FPSCOUNTER_HPP
#define GOSUEX_FPSCOUNTER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <Gosu/Font.hpp>
#include <Gosu/Timing.hpp>
#include <Gosu/Graphics.hpp>
#include <deque>

namespace GosuEx {
	class FPSCounter {
		struct Impl {
			// FPS, FPS-as-of-now, maxfps-in-this-query
			int fps, accum, maxFPS;
			// average fps over the last WIDTH units.
			double avg;
			// Gosu::milliseconds of the last collection
			unsigned long lastUpdate;
			// Coordinates, dimension
			double x, y, z, width, height;
			// Font
			boost::shared_ptr<Gosu::Font> font;
			// Color; average line color
			Gosu::Color fpsColor, textColor, avgColor;
			// Graphics to draw graphics
			Gosu::Graphics* graphics;
			// History. Each unit is one entry.
			std::deque<int> fpsHistory;
			// Should we even draw the fps needles?
			bool drawFPSPlot;
		} pimpl;
	public:
		// Constructor.
		FPSCounter(double x, double y, double z, double width, double height, Gosu::Graphics& graphics, boost::shared_ptr<Gosu::Font> font, Gosu::Color textColor, Gosu::Color fpsColor, Gosu::Color avgColor, bool drawFPSPlot = true) {
			pimpl.x = x;
			pimpl.y = y;
			pimpl.z = z;
			pimpl.width = width;
			pimpl.height = height;
			pimpl.font = font;
			pimpl.textColor = textColor;
			pimpl.fpsColor = fpsColor;
			pimpl.avgColor = avgColor;
			pimpl.graphics = &graphics;
			pimpl.fps = pimpl.accum = pimpl.maxFPS = 1;
			pimpl.avg = 0;
			pimpl.lastUpdate = Gosu::milliseconds();
			pimpl.drawFPSPlot = drawFPSPlot;
		}

		~FPSCounter() {}

		void update() {
			pimpl.accum++;
			if (Gosu::milliseconds() - pimpl.lastUpdate > 1000) {
				// Stuff it back. Except if it's the first one.
				if (pimpl.fps != 0 || pimpl.fpsHistory.size()) {
					pimpl.fpsHistory.push_back(pimpl.fps);
					if (pimpl.fpsHistory.size() > static_cast<std::size_t>(pimpl.width)-2)
						pimpl.fpsHistory.pop_front();
					double avg = 0;
					pimpl.maxFPS = 0;
					BOOST_FOREACH(int t, pimpl.fpsHistory) {
						avg += t;
						pimpl.maxFPS = std::max(pimpl.maxFPS, t);
					}
					pimpl.avg = avg/static_cast<double>(pimpl.fpsHistory.size());
				}
				// Because secondly based 
				pimpl.fps = pimpl.accum;
				pimpl.accum = 0;
				pimpl.lastUpdate = Gosu::milliseconds();
			}
		}

		void draw() {
			// Draw the borders.
			pimpl.graphics->drawLine(pimpl.x, pimpl.y, pimpl.fpsColor, pimpl.x+pimpl.width, pimpl.y, pimpl.fpsColor, pimpl.z);
			pimpl.graphics->drawLine(pimpl.x, pimpl.y+pimpl.height, pimpl.fpsColor, pimpl.x+pimpl.width, pimpl.y+pimpl.height, pimpl.fpsColor, pimpl.z);
			pimpl.graphics->drawLine(pimpl.x, pimpl.y, pimpl.fpsColor, pimpl.x, pimpl.y+pimpl.height, pimpl.fpsColor, pimpl.z);
			pimpl.graphics->drawLine(pimpl.x+pimpl.width, pimpl.y, pimpl.fpsColor, pimpl.x+pimpl.width, pimpl.y+pimpl.height, pimpl.fpsColor, pimpl.z);
			

			if (pimpl.drawFPSPlot) {
				// Draw the "spikes"
				double x = pimpl.x+pimpl.width-1;
				BOOST_FOREACH(int t, pimpl.fpsHistory) {
					pimpl.graphics->drawLine(x, pimpl.y+pimpl.height*(1.0-static_cast<double>(t)/static_cast<double>(pimpl.maxFPS)), pimpl.fpsColor, x, pimpl.y+pimpl.height, pimpl.fpsColor, pimpl.z);
					--x;
				}

				// And the average line.
				pimpl.graphics->drawLine(pimpl.x, pimpl.y+pimpl.height*(1.0-pimpl.avg/static_cast<double>(pimpl.maxFPS)), pimpl.avgColor, pimpl.x+pimpl.width, pimpl.y+pimpl.height*(1.0-pimpl.avg/static_cast<double>(pimpl.maxFPS)), pimpl.avgColor, pimpl.z);
			}

			// The font
			pimpl.font->drawRel((boost::wformat(L"Cur: %2.1f") % pimpl.fps).str(), pimpl.x+pimpl.width/2.0, pimpl.y+pimpl.height/2.0, pimpl.z, 0.5, 1.0, 1.0, 1.0, pimpl.textColor);
			pimpl.font->drawRel((boost::wformat(L"Avg: %2.1f") % pimpl.avg).str(), pimpl.x+pimpl.width/2.0, pimpl.y+pimpl.height/2.0, pimpl.z, 0.5, 0, 1.0, 1.0, pimpl.textColor);
		}
		
		int fps() const { return pimpl.fps; }
		double avg() const { return pimpl.avg; }
		int maxFPS() const { return pimpl.maxFPS; }
	};
}
#endif