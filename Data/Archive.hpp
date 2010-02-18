#ifndef GOSUEX_DATA_ARCHIVE_HPP
#define GOSUEX_DATA_ARCHIVE_HPP

/**
 * GosuEx::Data::Archive allows you to compress one or more
	* files into one (and extract them as buffer OR as a file.)
	* Compresses its files.
*/

#include <GosuEx/Data/Fwd.hpp>
#include <GosuEx/Data/Compress.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <Gosu/IO.hpp>

namespace GosuEx {
		namespace Data {

				class Archive {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						// Default constructor for new archives
						Archive();
						// Loads an existing archive
						Archive(const std::wstring& filename, const unsigned int CHUNK = 16384);
						// Loads an archive out of an buffer
						Archive(Gosu::Buffer& buffer, const unsigned int CHUNK = 16384);

						// dtor
						~Archive();

						// Saves the archive
						void save(const std::wstring& filename, int level = Z_DEFAULT_COMPRESSION, const unsigned int CHUNK = 16384);
						// Loads the archive (either from a buffer or from a file)
						void load(const std::wstring& filename, const unsigned int CHUNK = 16384);
						void load(Gosu::Buffer& buffer, const unsigned int CHUNK = 16384);

						// Gets a buffer
						Gosu::Buffer& get(const std::wstring& archiveName) const;

						// Gets and saves a file
						void saveFile(const std::wstring& archiveName, const std::wstring& filename) const;

						// Adds/overwrites a buffer/file
						void add(const std::wstring& archiveName, Gosu::Buffer& buffer);
						void addFile(const std::wstring& archiveName, const std::wstring& filename);
						
						// Removes a buffer
						void remove(const std::wstring& archiveName);
						
						// Does a buffer exist?
						bool exists(const std::wstring& archiveName) const;

						// Filecount
						std::size_t size() const;
						
						// Iterating through the stored file/buffernames.

						class iterator {
								std::map<std::wstring, Gosu::Buffer>::iterator cursor;
						public:
								// Constructor, only used by the archive. Do not use.
								iterator(std::map<std::wstring, Gosu::Buffer>::iterator it);
								// Copyctor
								iterator(const iterator& it);
								
								// increment. tested and works.
								iterator& operator++();
								// decrement. not tested.
								iterator& operator--();
								// Dereferencing
								std::wstring operator*() const;
								const std::wstring* operator->() const;

								// 
								bool operator==(const iterator& b);
								bool operator!=(const iterator& b);
						};
						
						iterator begin();
						iterator end();

						static const unsigned int VERSION = 268435456;
				};
		}
}
#endif