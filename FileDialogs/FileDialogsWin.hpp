#ifndef GOSUEX_FILEDIALOGS_FILEDIALOGSWIN_HPP
#define GOSUEX_FILEDIALOGS_FILEDIALOGSWIN_HPP

#include <GosuEx/FileDialogs/FileDialogs.hpp>

// Only in Kenya..  Err, I mean on Windows.
#ifdef _WIN32
#	include <Windows.h>
# include <stdio.h>
# include <ShObjIdl.h>
# include <Gosu/Window.hpp>
# include <boost/foreach.hpp>

namespace GosuEx {
	namespace FileDialogs {

		// File Dialogs for XP, maybe lower.

		// Parent.
		template<typename T> class FileDialogXP : public T {
		protected:
			OPENFILENAME p; // "Handle"
			std::wstring filename, initdir, title, filterStr; // Saved because.. I do not want any possible access violations.

			FileDialogXP(Gosu::Window& window, const std::wstring& initialdir, const std::wstring& title, FilterList& filters) {
				this->initdir = initialdir;
				this->title = title;
				this->filename.resize(MAX_PATH);
				ZeroMemory(&this->p, sizeof(OPENFILENAME));
				this->p.lStructSize = sizeof(OPENFILENAME);
				this->p.hwndOwner = window.handle();
				this->p.hInstance = NULL;
				this->p.lpstrCustomFilter = NULL;
				this->p.nMaxCustFilter = NULL;
				this->p.lpstrFile = &filename[0];
				this->p.nMaxFile = filename.size();
				this->p.lpstrFileTitle = NULL;
				this->p.nMaxFileTitle = 0;
				this->p.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				this->p.lpstrDefExt = 0;

				if (filters.filters.size()) {
					BOOST_FOREACH(const Filter& filter, filters.filters) {
						this->filterStr.append(filter.name);
						this->filterStr.append(L"\0", 1);
						this->filterStr.append(filter.pattern);
						this->filterStr.append(L"\0", 1);
					}
					this->filterStr.append(L"\0", 1);
					this->p.lpstrFilter = &filterStr[0]; // evil voodooo magic, but apparently, it works fine.
				}
				else
					this->p.lpstrFilter = NULL;
				
				if (this->title.length())
					this->p.lpstrTitle = &this->title[0];
				else
					this->p.lpstrTitle = NULL;
			}

			virtual void setFilename(const std::wstring& filename) {
				this->filename = filename;
				this->filename.resize(MAX_PATH);
				this->p.lpstrFile = &this->filename[0];
				this->p.nMaxFile = this->filename.size();
			}
		};
		
		// Open File under XP
		class OpenFileDialogXP : public FileDialogXP<OpenFileDialog> {
		public:
			OpenFileDialogXP(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters);

			virtual std::wstring open();
		};

		// Save a file under XP
		class SaveFileDialogXP : public FileDialogXP<SaveFileDialog> {
		public:
			SaveFileDialogXP(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters);

			virtual std::wstring save();
		};


		// File Dialogs for Windows Vista, 7, maybe newer.

		// Parent
		template<typename T> class FileDialog7 : public T {
		protected:
			IFileDialog* pfd;
			HRESULT hr;
			HWND handle;
			std::wstring title, filename;
			IShellItem* initialdir;
			std::vector<COMDLG_FILTERSPEC> rgSpec;
			FilterList filters;
			static HMODULE ShellDLL;
			static HRESULT(WINAPI* SHCIFPN)(PCWSTR, IBindCtx*, REFIID, void**);

			FileDialog7(const IID& id, Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters) {
				if (FileDialog7::ShellDLL == NULL) {
					CoInitialize(NULL);
					FileDialog7::ShellDLL = LoadLibrary(L"Shell32.dll");
					(FARPROC&)FileDialog7::SHCIFPN = GetProcAddress(FileDialog7::ShellDLL, "SHCreateItemFromParsingName");
				}

				this->handle = win.handle();
				this->title = title;
				this->filters = filters;
				this->hr = FileDialog7::SHCIFPN(initialdir.c_str(), NULL, IID_PPV_ARGS(&this->initialdir));
				if (!SUCCEEDED(hr))
					throw std::runtime_error("FileDialog7: cannot parse name");

				this->hr = CoCreateInstance(id, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->pfd));
				if (!SUCCEEDED(this->hr))
					throw std::runtime_error("FileDialog7: Unable to create CoInstance");

				this->pfd->SetTitle(&this->title[0]);
				this->pfd->SetFolder(this->initialdir);
				if (this->filters.filters.size()) {
					this->rgSpec.reserve(this->filters.filters.size());
					BOOST_FOREACH(const Filter& filter, this->filters.filters) {
						COMDLG_FILTERSPEC sp;
						sp.pszName = filter.name.c_str();
						sp.pszSpec = filter.pattern.c_str();
						this->rgSpec.push_back(sp);
					}
					pfd->SetFileTypes(this->rgSpec.size(), &this->rgSpec[0]);
				}
			}
		public:
			~FileDialog7() {
				this->pfd->Release();
				this->initialdir->Release();
			}

			virtual void setFilename(const std::wstring& filename) {
				this->filename = filename;
				this->pfd->SetFileName(this->filename.c_str());
			}

		protected:
			// Used to get the result
			// since this is the same on save and open
			std::wstring _result() {
				this->hr = pfd->Show(handle);
				if (!SUCCEEDED(this->hr)) // Either it did not work, or the user aborted
					return L"";

				IShellItem *psiResult;
				this->hr = pfd->GetResult(&psiResult);

				if (!SUCCEEDED(this->hr))
					return L"";

				wchar_t *filename;
				psiResult->GetDisplayName(SIGDN_FILESYSPATH, &filename);
				std::wstring result(filename);
				psiResult->Release();
				CoTaskMemFree(filename);
				return result;
			}
		};

		// Open
		class OpenFileDialog7 : public FileDialog7<OpenFileDialog> {
		public:
			OpenFileDialog7(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters);

			virtual std::wstring open();
		};

		class SaveFileDialog7 : public FileDialog7<SaveFileDialog> { 
		public:
			SaveFileDialog7(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters);
			
			virtual std::wstring save();
		};

		bool useNewDialogs();
	}
}
#	endif
#endif