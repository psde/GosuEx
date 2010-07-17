#include "FileDialogsWin.hpp"

using namespace GosuEx;

// Statics for W7
template<typename T> HMODULE GosuEx::FileDialogs::FileDialog7<T>::ShellDLL = NULL;
template<typename T> HRESULT(WINAPI* GosuEx::FileDialogs::FileDialog7<T>::SHCIFPN)(PCWSTR, IBindCtx*, REFIID, void**);

// XP
FileDialogs::OpenFileDialogXP::OpenFileDialogXP(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters): FileDialogXP<OpenFileDialog>(win, initialdir, title, filters) {}

std::wstring FileDialogs::OpenFileDialogXP::open() {
	if (GetOpenFileName(&this->p))
		return this->filename;
	return L"";
}

FileDialogs::SaveFileDialogXP::SaveFileDialogXP(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters): FileDialogXP<SaveFileDialog>(win, initialdir, title, filters) {}

std::wstring FileDialogs::SaveFileDialogXP::save() {
	if (GetSaveFileName(&this->p))
		return this->filename;
	return L"";
}

// Win7
FileDialogs::OpenFileDialog7::OpenFileDialog7(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters): FileDialog7<OpenFileDialog>(CLSID_FileOpenDialog, win, initialdir, title, filters) {}

std::wstring FileDialogs::OpenFileDialog7::open() {
	return _result();
}

FileDialogs::SaveFileDialog7::SaveFileDialog7(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters): FileDialog7<SaveFileDialog>(CLSID_FileSaveDialog, win, initialdir, title, filters) {}

std::wstring FileDialogs::SaveFileDialog7::save() {
	return _result();
}

// Determinate whether or not to use the new dialogs
bool FileDialogs::useNewDialogs() {
	return GetProcAddress(LoadLibrary(L"Shell32.dll"), "SHCreateItemFromParsingName") != NULL;
}

// And the functions to get the dialogs
FileDialogs::OpenFileDialog* FileDialogs::getOpenFileDialog(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters) {
	if (useNewDialogs())
		return new OpenFileDialog7(win, initialdir, title, filters);
	return new OpenFileDialogXP(win, initialdir, title, filters);
}

FileDialogs::SaveFileDialog* FileDialogs::getSaveFileDialog(Gosu::Window& win, const std::wstring& initialdir, const std::wstring& title, FilterList& filters) {
	if (useNewDialogs())
		return new SaveFileDialog7(win, initialdir, title, filters);
	else
		return new SaveFileDialogXP(win, initialdir, title, filters);
}