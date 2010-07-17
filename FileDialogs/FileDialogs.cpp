#include "FileDialogs.hpp"

using namespace GosuEx;

FileDialogs::Filter::Filter(const std::wstring& name, const std::wstring& pattern) { this->name = name; this->pattern = pattern; }

FileDialogs::FilterList& FileDialogs::FilterList::operator%(Filter& filter) { filters.push_back(filter); return *this; }