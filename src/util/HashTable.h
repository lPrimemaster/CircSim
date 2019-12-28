#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <type_traits>

#include "../features/Chunk.h"

#define PER_HASH

struct Hashable
{
	virtual size_t operator()(const Hashable& h) const = 0;
	virtual bool operator==(const Hashable& rhs) const = 0;
};

template<typename H, typename K = ChunkCoord>
class HashTable
{
private:
#ifdef PER_HASH
	static_assert(std::is_convertible<K*, Hashable*>::value, "Key class must inherit Hashable");
	std::unordered_map <K, H, K> table;
#else
	std::vector<std::vector<H>> RU;
	std::vector<std::vector<H>> RD;

	std::vector<std::vector<H>> LU;
	std::vector<std::vector<H>> LD;
#endif // PER_HASH

#ifndef PER_HASH
private:
	__forceinline H getAlignment(const K& key) const
	{
		if (key.chunk_id_x >= 0)
		{
			if (key.chunk_id_y >= 0)
			{
				return RU.at(key.chunk_id_x).at(key.chunk_id_y);
			}
			else
			{
				return RD.at(key.chunk_id_x).at(-key.chunk_id_y + 1);
			}
		}
		else
		{
			if (key.chunk_id_y >= 0)
			{
				return LU.at(-key.chunk_id_x + 1).at(key.chunk_id_y);
			}
			else
			{
				return RD.at(-key.chunk_id_x + 1).at(-key.chunk_id_y + 1);
			}
		}
	}

	//TODO: Finish this function for alignment
	__forceinline H setAlignment(const K& key, const H& value)
	{
		if (key.chunk_id_x >= 0)
		{
			size_t kcs_x = key.chunk_id_x + 1;
			kcs_x > RU.size() ? RU.resize(kcs_x) : 0;
			if (key.chunk_id_y >= 0)
			{
				RU.at(key.chunk_id_x).at(key.chunk_id_y) = value;
			}
			else
			{
				RD.at(key.chunk_id_x).at(-key.chunk_id_y + 1) = value;
			}
		}
		else
		{
			if (key.chunk_id_y >= 0)
			{
				LU.at(-key.chunk_id_x + 1).at(key.chunk_id_y) = value;
			}
			else
			{
				RD.at(-key.chunk_id_x + 1).at(-key.chunk_id_y + 1) = value;
			}
		}
	}

	//TODO: Erase align
	__forceinline void eraseAlignment(const K& key)
	{

	}

	//TODO: Check align
	__forceinline bool checkAlignment(const K& key)
	{
		return false;
	}
#endif // PER_HASH

public:
	inline H at(const K& key) const throw()
	{
		H value;
		try
		{
#ifdef PER_HASH
			value = table.at(key);
#else
			value = getAlignment(key);
#endif // PER_HASH
		}
		catch (const std::exception & e)
		{
			throw std::exception(e);
		}

		return value;
	}

	inline void try_emplace(const K& key, const H& value)
	{
#ifdef PER_HASH
		table.try_emplace(key, value);
#else
		setAlignment(key, value);
#endif // PER_HASH
	}

	inline void erase(const K& key) throw()
	{
		try
		{
#ifdef PER_HASH
			table.erase(key);
#else
			eraseAlignment(key);
#endif // PER_HASH
		}
		catch (const std::exception & e)
		{
			throw std::exception(e);
		}
	}

	bool evaluate(const K& key) const
	{
#ifdef PER_HASH
		return table.find(key) != table.cend();
#else
		return checkAlignment(key);
#endif // PER_HASH
	}
};

