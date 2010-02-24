#ifndef GOSUEX_DATA_COMPRESS_HPP
#define GOSUEX_DATA_COMPRESS_HPP

/**
* Provides functions to compress (deflate) and decompress (inflate) Gosu::Buffers
* and GosuEx::Data::Packet.
*/

#include <Gosu/Fwd.hpp>
#include <GosuEx/Data/Fwd.hpp>
// This path should be usable if you have a checkout of Gosu's SVN
#include <dependencies/zlib/zlib.h>
#include <stdexcept>

namespace GosuEx {
		namespace Data {
				
				/**
				* Compresses source into dest. CHUNK is the buffer size for reading/writing - if you can 
				* provide more, use 128K or 256K. More buffer = more efficiency
				*/

				void deflate(const Gosu::Buffer& source, Gosu::Buffer& dest, int level = Z_DEFAULT_COMPRESSION, const unsigned int CHUNK = 16384);


				/**
				* Compresses the packet source into dest. CHUNK is the buffer for reading/writing - if you can
				* provide more, use 128K or 256K. More buffer - more efficiency.
				*/

				void deflatePacket(const Packet& source, Gosu::Buffer& dest, int level = Z_DEFAULT_COMPRESSION, const unsigned int CHUNK = 16384);

				/**
				* Decompresses source into dest. CHUNK is the buffer size for reading/writing - if you can
				* provide more, use 128K or 256K. More buffer = more efficiency.
				*/
				
				void inflate(const Gosu::Buffer& source, Gosu::Buffer& dest, const unsigned int CHUNK = 16384);

				/**
				* Decompresses the packet source into dest. CHUNK is the buffer for reading/writing - if you can
				* provide more, use 128K or 256K. More buffer - more efficiency.
				*/

				void inflatePacket(const Gosu::Buffer& source, Packet& dest, const unsigned int CHUNK = 16384);

				/**
				* EXCEPTION!
				*/

				class ZException : public std::runtime_error {
						int _Code;
				public:
						ZException(const std::string& message, int code);
						
						int code() const;
				};
		}
}

#endif