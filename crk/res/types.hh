#ifndef CRK_RES_TYPES_HH
#define CRK_RES_TYPES_HH

#include <format>
#include <map>
#include <memory>
#include <string>
#include <cassert>

namespace crk
{

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T> class RefCache
{
	typename std::map<std::string, std::weak_ptr<T>> cache;

public:
	RefCache()
	{
	}

	~RefCache()
	{
		for (auto &ptr : cache)
		{
			if (!ptr.second.expired())
				assert(false &&
				       std::format("Memory leaked, at key {}!", ptr.first)
				           .c_str());
		}
	}

	bool Exists(const std::string &key) const
	{
		if (!cache.contains(key))
			return false;
		return !cache.at(key).expired();
	}

	crk::Ref<T> Request(const std::string &key)
	{
		assert(cache.contains(key) &&
		       "Requesting nonexistent value. Use Exists() beforehand!");
		return crk::Ref<T>(cache.at(key));
	}

	void Set(const std::string &key, const crk::Ref<T> &ptr)
	{
		cache[key] = ptr;
	}
};

} // namespace crk

#endif