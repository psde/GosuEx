#ifndef GOSUEX_FPSCOUNTER_HPP
#define GOSUEX_FPSCOUNTER_HPP

#include <Gosu/Graphics.hpp>
#include <Gosu/Math.hpp>
#include <Gosu/Color.hpp>
#include <Gosu/Timing.hpp>
#include <Gosu/Font.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <deque>

namespace GosuEx
{
	class FpsCounter {
	private:
		struct FpsPoint {
			FpsPoint(double fps, bool landmark = false) {
				this->fps = fps;
				this->landmark = landmark;
			}

			double fps;
			bool landmark;
		};

		Gosu::Graphics *graphics;

		int x, y, z, width, height;

		int logtime;

		std::deque<FpsPoint> fpslist;

		boost::shared_ptr<Gosu::Font> font;

		
		Gosu::Color boxColor, lineColor, lineLandmarkColor;

		double updateInterval, accum, timeleft;
		int frames, lastFrameCompleted;
	public:	
		double fps;

		FpsCounter(int x, int y, int z, int width, int height, Gosu::Graphics *graphics, boost::shared_ptr<Gosu::Font> font, float updateInterval = 0.03f) {
			this->font = font;
			this->boxColor = Gosu::Color(255, 255, 255, 255);
			this->lineColor = Gosu::Color(220, 255, 255, 255);
			this->lineLandmarkColor = Gosu::Color(65, 255, 255, 255);

			this->timeleft = updateInterval;
			this->accum = 0.0;
			this->frames = 0;
			this->lastFrameCompleted = Gosu::milliseconds();
			this->logtime = Gosu::milliseconds();
			this->fps = 0;
			this->graphics = graphics;
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->updateInterval = updateInterval;
		}
        
		// this yields wrong results if not called on the end of the draw() method, so maybe we can just 
		// do the calulation in FpsCounter::draw (user has only one function to call every time, not two)
		// on the other side: what about user that want to calculate but not want to show them? Hmm.. think about that
		void updateFPS() {
			// based on http://www.unifycommunity.com/wiki/index.php?title=FramesPerSecond
			double deltaTime = Gosu::milliseconds() - lastFrameCompleted;
			timeleft -= deltaTime/1000.0;
			accum += 1000.0/deltaTime;
			frames++;
			fps = Gosu::round(accum/frames);

			if(timeleft <= 0.0) {
				bool landmark = false;
				if(Gosu::milliseconds() > static_cast<unsigned long>(logtime + 1000)) {
					logtime = Gosu::milliseconds();
					landmark = true;
				}
				fpslist.push_back(FpsPoint(fps, landmark));
				frames = 0;
				accum = 0;
				timeleft = updateInterval;
			}

			while(fpslist.size() > static_cast<unsigned int>(width+1))
				fpslist.pop_front();
		}

		void draw() {
			lastFrameCompleted = Gosu::milliseconds();

			// draw box 
			graphics->drawLine(x, y, boxColor, x + width, y, boxColor, z); // ->
			graphics->drawLine(x + width, y, boxColor, x + width, y + height, boxColor, z); // v
			graphics->drawLine(x, y + height, boxColor, x + width, y + height, boxColor, z); // <-
			graphics->drawLine(x, y, boxColor, x, y + height, boxColor, z); // ^

			// draw fps 
			font->draw(L"Cur: " + boost::lexical_cast<std::wstring>(fps), x + 4, y + 2, z, 1, 1, boxColor);

			// draw graph, if we are on it, calculate avg fps 
			double avg = 0;
			int avgCount = 0;
			if (fpslist.size() > 2) {
				double oldVal = Gosu::clamp(fpslist.at(0).fps, 0.0, static_cast<double>(height));
				for(unsigned int i=1; i < fpslist.size() - 1; i++)
				{
					
					double newVal = Gosu::clamp(fpslist.at(i).fps * -1 + height, 0.0, static_cast<double>(height));
					
					graphics->drawLine(x + (width - i) - 1, y + oldVal, lineColor, x + (width - i), y + newVal, lineColor, z);
					
					if(fpslist.at(i).landmark) {
						graphics->drawLine(x, y + height, lineLandmarkColor, x+1, y + newVal, lineLandmarkColor, z);
					}

					// ignore negative fps, only happens a few times after init
					if(fpslist.at(i).fps > 0) {
						avg += fpslist.at(i).fps;
						avgCount++;
					}

					oldVal = newVal;
				}
				avg = avg / avgCount;

				// maybe we should round the avg fps instead of just taking the first 4 chars
				font->draw(L"Avg: " + boost::lexical_cast<std::wstring>(avg).substr(0,4), x+4, y+14, z, 1, 1, boxColor);
			}
		}
	};
}

#endif