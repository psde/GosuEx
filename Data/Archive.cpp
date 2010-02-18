#include "Archive.hpp"
#include "Packet.hpp"
#include "Compress.hpp"

using namespace GosuEx;

struct Data::Archive::Impl {
		std::map<std::wstring, Gosu::Buffer> files;
};


Data::Archive::Archive():
pimpl(new Impl) {
		// nothing to doo!
}

Data::Archive::Archive(const std::wstring &filename, const unsigned int CHUNK):
pimpl(new Impl) {
		load(filename, CHUNK);
}

Data::Archive::Archive(Gosu::Buffer& buffer, const unsigned int CHUNK):
pimpl(new Impl) {
		load(buffer, CHUNK);
}

Data::Archive::~Archive() {}

void Data::Archive::save(const std::wstring& filename, int level, const unsigned int CHUNK) {
		// Serialisieren wir das ganze...
		Packet p;
		// "Magic bytes"
		p << std::string("GXDA");
		// Version
		p << VERSION;
		// Anzahl Einträge
		p << (std::size_t)pimpl->files.size();
		for (std::map<std::wstring, Gosu::Buffer>::iterator it = pimpl->files.begin(); it != pimpl->files.end(); ++it) {
				p << it->first << it->second;
		}
		Gosu::Buffer compressed;
	 deflatePacket(p, compressed, level, CHUNK);
		Gosu::saveFile(compressed, filename);
}

void Data::Archive::load(const std::wstring& filename, const unsigned int CHUNK) {
		Gosu::Buffer compressed;
		Gosu::loadFile(compressed, filename);
		load(compressed, CHUNK);
}

void Data::Archive::load(Gosu::Buffer& buffer, const unsigned int CHUNK) {
		Packet p;
		inflatePacket(buffer, p, CHUNK);
		std::string mb;
		p >> mb;
		if (mb != "GXDA")
				throw std::runtime_error("Invalid magic bytes");
		unsigned int av;
		p >> av;
		if (av > VERSION)
				throw std::runtime_error("Archive was made with a more recent version");
		std::size_t files;
		p >> files;
		for (std::size_t i = 0; i < files; i++) {
				std::wstring filename;
				p >> filename;
				p >> get(filename);
		}
}

Gosu::Buffer& Data::Archive::get(const std::wstring& filename) const {
		return pimpl->files[filename];
}

void Data::Archive::saveFile(const std::wstring& archiveName, const std::wstring& filename) const {
		Gosu::saveFile(get(archiveName), filename);
}

void Data::Archive::add(const std::wstring& filename, Gosu::Buffer& buffer) {
		pimpl->files[filename] = buffer;
}

void Data::Archive::addFile(const std::wstring& archiveName, const std::wstring& filename) {
		Gosu::loadFile(get(archiveName), filename);
}

void Data::Archive::remove(const std::wstring& archiveName) {
		pimpl->files.erase(archiveName);
}

bool Data::Archive::exists(const std::wstring& archiveName) const {
		return pimpl->files.find(archiveName) != pimpl->files.end();
}

std::size_t Data::Archive::size() const { return pimpl->files.size(); }

Data::Archive::iterator::iterator(const iterator& it):
cursor(it.cursor) {}

Data::Archive::iterator::iterator(std::map<std::wstring, Gosu::Buffer>::iterator it):
cursor(it) {}

Data::Archive::iterator& Data::Archive::iterator::operator++() {
		cursor++;
		return *this;
}

Data::Archive::iterator& Data::Archive::iterator::operator--() {
		cursor--;
		return *this;
}

std::wstring Data::Archive::iterator::operator *() const {
		return cursor->first;
}

const std::wstring* Data::Archive::iterator::operator->() const {
		return &cursor->first;
}

bool Data::Archive::iterator::operator==(const iterator& b) {
		return b.cursor == cursor;
}

bool Data::Archive::iterator::operator!=(const iterator& b) {
		return !(*this == b);
}

Data::Archive::iterator Data::Archive::begin() {
		return iterator(pimpl->files.begin());
}

Data::Archive::iterator Data::Archive::end() {
		return iterator(pimpl->files.end());
}