#ifndef GOSUEX_FILEDIALOGS_FILEDIALOGS_HPP
#define GOSUEX_FILEDIALOGS_FILEDIALOGS_HPP

#include <Gosu/Fwd.hpp>

#include <string>
#include <vector>

namespace GosuEx {
	namespace FileDialogs {

		// Filters are used to specify allowed file types.
		struct Filter {
			std::wstring name, pattern;
			Filter(const std::wstring& name, const std::wstring& pattern);
		};


		// A filterlist is a list of filters that can be passed to open/save dialogs.
		// You can use it like that: FilterList() % Filter(L"All Files", L"*.*") % Filter(L"Images", L"*.png;*.jpg;*.jpeg").
		struct FilterList {
			std::vector<Filter> filters;
			FilterList& operator%(Filter& filter);
		};

		// This is the "parent" of all dialogs.
		class FileDialog {
		public:
			virtual void setFilename(const std::wstring& name) = 0;
		};

		// Parent of all "Open File..." dialogs.
		class OpenFileDialog : public FileDialog {
		public:
			virtual std::wstring open() = 0;
		};

		// Parent of all "Save File..." dialogs.
		class SaveFileDialog : public FileDialog {
		public:
			virtual std::wstring save() = 0;
		};

		// Just call these.
		OpenFileDialog* getOpenFileDialog(Gosu::Window& win, const std::wstring& initialdir = L"", const std::wstring& title = L"", FilterList& filters = FilterList());
		SaveFileDialog* getSaveFileDialog(Gosu::Window& win, const std::wstring& initialdir = L"", const std::wstring& title = L"", FilterList& filters = FilterList());
	}
}
#endif