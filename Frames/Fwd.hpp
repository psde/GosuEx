#ifndef GOSUEX_FRAMES_FWD_HPP
#define GOSUEX_FRAMES_FWD_HPP

#if _MSC_VER >= 1600
#	ifdef _DEBUG
#		define _CRTDBG_MAP_ALLOC
#		define _CRT_RAND_S
#		include <stdlib.h>
#		include <crtdbg.h>
#	endif
#endif

#include <set>
#include <vector>
#include <string>
#include <map>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#if _MSC_VER >= 1600
#	ifdef _DEBUG
#		define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	endif
#endif

namespace GosuEx {
	namespace Frames {
		class Widget;
		class Frame;
		class SquareCursor;
		class TriangleCursor;
		class ExtFrame;

		class FrameManager;
	}
}
#endif