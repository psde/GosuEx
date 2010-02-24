#ifndef GOSUEX_DATA_PACKET_HPP
#define GOSUEX_DATA_PACKET_HPP

#include <vector>
#include <string>

namespace GosuEx {
		namespace Data {
				class Packet {
						unsigned int index;
						std::vector<char> buf;
				public:
						// Constructor: "Put things in here"
						Packet() { 
								index = 0;
						}

						// Constructor: "Read things out here"
						Packet(const void* data, std::size_t size) {
								index = 0;
								const char* cdata = static_cast<const char*>(data);
								buf.insert(buf.end(), cdata, cdata+size);
						}

						// Adding something
						template<typename T> Packet& operator<<(const T& o);

						// Reading something
						template<typename T> Packet& operator>>(T& o);

						// Getting the buffer
						const void* data() const { return &buf[0]; }
						bool more() const { return index < size(); }
						std::size_t size() const { return buf.size(); }

						// do not use this.
						void seek(int pos) { index = pos; }

				};

				
				template<typename T> inline Packet& Packet::operator<<(const T& o) {
						//buf.insert(buf.end(), &o, &o+sizeof(T));
						const char* cdata = reinterpret_cast<const char*>(&o);
						buf.insert(buf.end(), cdata, cdata+sizeof(T));
						return *this;
				}

				template<typename T> inline Packet& Packet::operator>>(T& o) {
						index += sizeof(T);
						if (size() < index)
								throw std::out_of_range("Out of range: Packet::operator>> <T>");
						//memcpy(&o, &buf[index-sizeof(T)], sizeof(T));
						memcpy(&o, &buf[index-sizeof(T)], sizeof(T));
						return *this;
				}

				template<> inline Packet& Packet::operator<< <std::string> (const std::string& o) {
						*this << (std::size_t)o.size();
						buf.insert(buf.end(), o.c_str(), o.c_str()+o.size());
						return *this;
				}

				template<> inline Packet& Packet::operator>> <std::string> (std::string& o) {
						std::size_t len;
						*this >> len;
						index += len;
						if (size() < len)
								throw std::out_of_range("Out of range: Packet::operator>> <std::string>");
						o.resize(len);
						memcpy(&o[0], &buf[index-len], len);
						return *this;
				}

				template<> inline Packet& Packet::operator<< <std::wstring> (const std::wstring& o) {
						*this << (std::size_t)o.size();
						const char* cdata = reinterpret_cast<const char*>(o.c_str());
						buf.insert(buf.end(), cdata, cdata+o.size()*sizeof(wchar_t));
						return *this;
				}
				
				template<> inline Packet& Packet::operator>> <std::wstring> (std::wstring& o) {
						std::size_t len;
						*this >> len;
						index += len*sizeof(wchar_t);
						if (size() < index)
								throw std::out_of_range("Out of range: Packet::operator>> <std::wstring>");
						o.resize(len);
						memcpy(&o[0], &buf[index-len*sizeof(wchar_t)], len*sizeof(wchar_t));
						return *this;
				}

				template<> inline Packet& Packet::operator<< <Gosu::Buffer> (const Gosu::Buffer& o) {
						*this << (std::size_t)o.size();
						// Keine Grösse = das könnte böse werden.
						if (!o.size()) return *this;
						const char* cdata = reinterpret_cast<const char*>(o.data());
						buf.insert(buf.end(), cdata, cdata+o.size());
						return *this;
				}

				template<> inline Packet& Packet::operator>> <Gosu::Buffer> (Gosu::Buffer& o) {
						std::size_t len;
						*this >> len;
						index += len;
						if (size() < index)
								throw std::out_of_range("Out of range: Packet::operator>> <Gosu::Buffer>");
						o.resize(len);
						// Das könnte, auch, böse werden.
						if (!len) return *this;
						memcpy(o.data(), &buf[index-len], len);
						return *this;
				}

				inline Packet& operator<<(Packet& p, const char* c) {
						p << std::string(c);
						return p;
				}

				inline Packet& operator<<(Packet& p, const wchar_t* c) {
						p << std::wstring(c);
						return p;
				}


		}
}
#endif