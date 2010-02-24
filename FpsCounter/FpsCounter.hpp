#ifndef GOSU_EXT_FPSCOUNTER_HPP
#define GOSU_EXT_FPSCOUNTER_HPP

#include <Gosu/Gosu.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <deque>
#include <math.h>

namespace
{
	struct Point
	{
		Point(int x = 0, int y = 0) {
			this->x = x;
			this->y = y;
		}
		int x,y;
	};

	struct FpsPoint
	{
		FpsPoint(double fps, bool landmark = false)
		{
			this->fps = fps;
			this->landmark = landmark;
		}

		double fps;
		bool landmark;
	};
}

namespace GosuEx
{
	class FpsCounter
	{
	private:
		Gosu::Graphics *graphics;

		int x, y, width, height;

		int logtime;

		std::deque<FpsPoint> fpslist;

		boost::shared_ptr<Gosu::Font> font;

		
		Gosu::Color boxColor, lineColor, lineLandmarkColor;

		double updateInterval, accum, timeleft, fps;
		int frames, lastFrameCompleted;

	public:	
		FpsCounter(Gosu::Graphics *graphics, int x, int y, int width, int height, float updateInterval = 0.03f)
			: graphics(graphics), x(x), y(y), width(width), height(height), updateInterval(updateInterval)
		{
			font.reset(new Gosu::Font(*graphics, Gosu::defaultFontName(), 14));

			this->boxColor = Gosu::Color(255, 255, 255, 255);
			this->lineColor = Gosu::Color(220, 255, 255, 255);
			this->lineLandmarkColor = Gosu::Color(65, 255, 255, 255);

			this->timeleft = this->updateInterval;
			this->accum = 0.0;
			this->frames = 0;
			this->lastFrameCompleted = Gosu::milliseconds();
			this->logtime = Gosu::milliseconds();
			this->fps = 0;
		}
        
        void setUpdateInterval(float interval)
        {
            this->updateInterval = interval;
        }

		// this yields wrong results if not called on the end of the draw() method, so maybe we can just 
		// do the calulation in FpsCounter::draw (user has only one function to call every time, not two)
		// on the other side: what about user that want to calculate but not want to show them? Hmm.. think about that
		void updateFPS()
		{
			// based on http://www.unifycommunity.com/wiki/index.php?title=FramesPerSecond
			double deltaTime = Gosu::milliseconds() - this->lastFrameCompleted;
			this->timeleft -= deltaTime/1000.0;
			this->accum += 1000.0/deltaTime;
			this->frames++;
			this->fps = Gosu::round(accum/frames);

			if( timeleft <= 0.0 )
			{
				bool landmark = false;
				if(Gosu::milliseconds() > static_cast<unsigned int>(logtime + 1000))
				{
					logtime = Gosu::milliseconds();
					landmark = true;
				}
				this->fpslist.push_back(FpsPoint(fps, landmark));
				this->frames = 0;
				this->accum = 0;
				this->timeleft = this->updateInterval;
			}

			while(fpslist.size() > static_cast<unsigned int>(this->width+1))
				fpslist.pop_front();
		}

		void draw()
		{
			this->lastFrameCompleted = Gosu::milliseconds();

			Point topLeft(x, y);
			Point downRight(x + width, y + height);

			// draw box 
			this->graphics->drawLine(topLeft.x, topLeft.y, boxColor, downRight.x, topLeft.y, boxColor, Gosu::zImmediate); // Top 
			this->graphics->drawLine(topLeft.x - 1, downRight.y, boxColor, downRight.x, downRight.y, boxColor, Gosu::zImmediate); // Bottom 
			this->graphics->drawLine(topLeft.x, topLeft.y, boxColor, topLeft.x, downRight.y, boxColor, Gosu::zImmediate); // Left 
			this->graphics->drawLine(downRight.x, topLeft.y, boxColor, downRight.x, downRight.y, boxColor, Gosu::zImmediate); // Left

			// draw fps 
			font->draw(L"Cur: " + boost::lexical_cast<std::wstring>(fps), topLeft.x+4, topLeft.y+2, Gosu::zImmediate, 1, 1, this->boxColor);

			// draw graph, if we are on it, calculate avg fps 
			double avg = 0;
			int avgCount = 0;
			if(fpslist.size() > 2)
			{
				for(unsigned int i=0; i < fpslist.size() - 1; i++)
				{
					int x = topLeft.x + (width - i) - 1;
					double val = fpslist.at(i).fps * -1 + height;
					double val2 = fpslist.at(i+1).fps * -1 + height;

					val = Gosu::clamp(val, 0.0, static_cast<double>(height));
					val2 = Gosu::clamp(val2, 0.0, static_cast<double>(height));
					
					this->graphics->drawLine(x, topLeft.y + val2, lineColor, x+1, topLeft.y + val, lineColor, Gosu::zImmediate);
					
					if(fpslist.at(i).landmark)
					{
						this->graphics->drawLine(x, topLeft.y + height, lineLandmarkColor, x+1, topLeft.y + val, lineLandmarkColor, Gosu::zImmediate);
					}

					// ignore negative fps, only happens a few times after init
					if(fpslist.at(i).fps > 0)
					{
						avg += fpslist.at(i).fps;
						avgCount++;
					}
				}
				avg = avg / avgCount;

				// maybe we should round the avg fps instead of just taking the first 4 chars
				font->draw(L"Avg: " + boost::lexical_cast<std::wstring>(avg).substr(0,4), topLeft.x+4, topLeft.y+14, Gosu::zImmediate, 1, 1, this->boxColor);
			}
		}
	};
}

#endif