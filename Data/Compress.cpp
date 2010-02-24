#include "Compress.hpp"
#include <vector>
#include <Gosu/IO.hpp>
#include "Packet.hpp"

void GosuEx::Data::deflate(const Gosu::Buffer& source, Gosu::Buffer& dest, int level, const unsigned int CHUNK) {
		// Vars
		int ret, flush;
		unsigned int have;
		unsigned int sourceOffset = 0;
		z_stream strm;
		std::vector<Bytef> in(CHUNK);
		std::vector<Bytef> out(CHUNK);
		//unsigned char* in = new unsigned char[CHUNK];
		//unsigned char* out = new unsigned char[CHUNK];
		// Resetting strm
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		// Start deflating
		ret = deflateInit(&strm, level);

		// Something went wrong? Oops.
		if (ret != Z_OK)
				throw ZException("deflateInit failed", ret);
		// Outer "feed our data" loop.
		do {
				strm.avail_in = source.size()-sourceOffset < CHUNK ? source.size()-sourceOffset : CHUNK;
				flush = strm.avail_in == CHUNK ? Z_NO_FLUSH : Z_FINISH;
				source.read(sourceOffset, strm.avail_in, &in[0]);
				strm.next_in = &in[0];
				sourceOffset += CHUNK;
				// inner compress data loop.
				do {
						strm.avail_out = CHUNK;
						strm.next_out = &out[0];
						ret = deflate(&strm, flush);
						if (ret == Z_STREAM_ERROR)
								throw ZException("deflate failed", ret);
						have = CHUNK - strm.avail_out;
						dest.resize(dest.size()+have);
						dest.write(dest.size()-have, have, &out[0]);
				} while (strm.avail_out == 0);
				if (strm.avail_in != 0)
						throw std::runtime_error("Not all input was proceeded");
		} while (flush != Z_FINISH);
		if (ret != Z_STREAM_END)
				throw ZException("Expected Z_STREAM_END", ret);
		deflateEnd(&strm);
}

void GosuEx::Data::deflatePacket(const Packet& source, Gosu::Buffer& dest, int level, const unsigned int CHUNK) {
		Gosu::Buffer sbuf;
		sbuf.resize(source.size());
		sbuf.write(0, source.size(), source.data());
		deflate(sbuf, dest, level, CHUNK);
}

void GosuEx::Data::inflate(const Gosu::Buffer& source, Gosu::Buffer& dest, const unsigned int CHUNK) {
		int ret;
		unsigned int have;
		unsigned int sourceOffset = 0;
		z_stream strm;
		std::vector<Bytef> in(CHUNK);
		std::vector<Bytef> out(CHUNK);
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit(&strm);
		if (ret != Z_OK)
				throw ZException("inflateInit failed", ret);
		do {
				strm.avail_in = source.size()-sourceOffset < CHUNK ? source.size()-sourceOffset : CHUNK;
				if (strm.avail_in == 0) break;
				source.read(sourceOffset, strm.avail_in, &in[0]);
				strm.next_in = &in[0];
				sourceOffset += CHUNK;
				
				do {
						strm.avail_out = CHUNK;
						strm.next_out = &out[0];
						ret = inflate(&strm, Z_NO_FLUSH);
						
						switch (ret) {
								case Z_NEED_DICT:
										ret = Z_DATA_ERROR;
								case Z_DATA_ERROR:
								case Z_MEM_ERROR:
										inflateEnd(&strm);
										throw ZException("inflate failed", ret);
								default:
										break;
						}
						have = CHUNK - strm.avail_out;
						dest.resize(dest.size()+have);
						dest.write(dest.size()-have, have, &out[0]);
				} while (strm.avail_out == 0);
		} while (ret != Z_STREAM_END);
		inflateEnd(&strm);
		if (ret != Z_STREAM_END)
				throw ZException("Expected Z_STREAM_END", ret);
}

void GosuEx::Data::inflatePacket(const Gosu::Buffer& source, Packet& dest, const unsigned int CHUNK) {
		Gosu::Buffer dbuf;
		inflate(source, dbuf, CHUNK);
		dest = Packet(dbuf.data(), dbuf.size());
}

GosuEx::Data::ZException::ZException(const std::string& message, int code):
std::runtime_error(message) {
		this->_Code = code;
}

int GosuEx::Data::ZException::code() const { return _Code; }