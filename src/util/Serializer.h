#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>

template<typename T>
struct is_serializable
{
	static const bool value = false;
};

namespace serializer
{
	class OutArchive
	{
	private:
		std::uint8_t* ppStart;
		std::uint8_t* pp;
		std::uint8_t* ppEnd;
		std::size_t size = 0;
		std::allocator<std::uint8_t> byte_alloc;

	public:
		inline void save(const char* fullPath)
		{
			std::fstream file(fullPath, std::ios::out | std::ios::binary);
			std::uint8_t* sz = byte_alloc.allocate(sizeof(std::size_t));
			std::memcpy(sz, &size, sizeof(std::size_t));
			file.write((char*)sz, sizeof(std::size_t));
			byte_alloc.deallocate(sz, sizeof(std::size_t));
			file.write((char*)ppStart, size);
			file.close();
		}

		inline void allocate(std::size_t bytes)
		{
			assert(size == 0);
			pp = byte_alloc.allocate(bytes);
			size = bytes;
			ppEnd = pp + size + 1;
			ppStart = pp;
		}

		inline void reallocate(std::size_t bytes)
		{
			assert(size != 0);
			std::uint8_t* temp = pp;
			pp = byte_alloc.allocate(bytes);
			std::memcpy(pp, temp, size);
			byte_alloc.deallocate(temp, size);
			size = bytes;
			ppEnd = pp + size + 1;
			ppStart = pp;
		}

		inline void write(const void* p, std::size_t size)
		{
			assert(pp + size < ppEnd);
			std::memcpy(pp, p, size);
			pp += size;
		}

		template<typename T, typename P = int, typename std::enable_if_t<std::is_pod<T>::value, int> = 0>
		inline void process(const T& value)
		{
			write(&value, sizeof(T));
		}

		template<typename T, typename P = int, typename std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
		inline void process(const std::string& value)
		{
			std::size_t l = value.length();
			write(&l, sizeof(std::size_t));
			write(value.c_str(), l);
		}

		template<typename T, typename P, typename std::enable_if_t<std::is_same<T, std::vector<P>>::value, int> = 0>
		inline void process(const std::vector<P>& value)
		{
			std::size_t sz = value.size();
			write(&sz, sizeof(std::size_t));

			if (std::is_pod<P>::value)
			{
				write(value.data(), sizeof(P) * sz);
			}
			else if(is_serializable<P>::value)
			{
				for (auto v : value)
				{
					//v.serialize(this);
				}
			}
		}
	};

	class InArchive
	{
	private:
		std::uint8_t* pp;
		std::uint8_t* ppEnd;
		std::size_t size = 0;
		std::allocator<std::uint8_t> byte_alloc;

	public:
		inline void load(const char* fullPath)
		{
			assert(size == 0);
			std::fstream file(fullPath, std::ios::in | std::ios::binary);
			std::uint8_t* sz = byte_alloc.allocate(sizeof(std::size_t));
			file.read((char*)sz, sizeof(std::size_t));
			std::memcpy(&size, sz, sizeof(std::size_t));
			byte_alloc.deallocate(sz, sizeof(std::size_t));
			pp = byte_alloc.allocate(size);
			file.read((char*)pp, size);
			file.close();
			ppEnd = pp + size + 1;
		}

		inline void read(void* p, std::size_t size)
		{
			assert(pp + size < ppEnd);
			std::memcpy(p, pp, size);
			pp += size;
		}

		template<typename T, typename P = int, typename std::enable_if_t<std::is_pod<T>::value, int> = 0>
		inline void process(T* value)
		{
			read(value, sizeof(T));
		}

		template<typename T, typename P = int, typename std::enable_if_t<std::is_same<T, std::string>::value, int> = 0>
		inline void process(std::string* value)
		{
			std::size_t l;
			read(&l, sizeof(std::size_t));
			assert(pp + l < ppEnd);
			new (value) std::string(reinterpret_cast<const char*>(pp), l);
			pp += l;
		}

		template<typename T, typename P, typename std::enable_if_t<std::is_same<T, std::vector<P>>::value, int> = 0>
		inline void process(std::vector<P>* value)
		{
			std::size_t sz;
			read(&sz, sizeof(std::size_t));
			value->resize(sz);
			if (std::is_pod<P>::value)
			{
				read(value->data(), sizeof(P) * sz);
			}
			else if(is_serializable<P>::value)
			{
				for (std::size_t i = 0; i < sz; i++)
				{
					value->push_back(P(this));
				}
			}
		}
	};
};

