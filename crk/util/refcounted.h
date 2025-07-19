#ifndef CRK_REFCOUNTED_H
#define CRK_REFCOUNTED_H

#include "../mem/allocator.h"
#include <functional>
#include <memory>
#include <utility>

namespace crk
{

template <typename T> class Lifetime
{
	std::function<void(T *)> death;
	T *obj;

public:
	Lifetime(T *_obj, auto &&_death)
	    : obj(_obj)
	    , death(_death)
	{
	}

	~Lifetime()
	{
		death(obj);
	}

	Lifetime(Lifetime &&other) = delete;
	Lifetime(const Lifetime &other) noexcept = delete;
	Lifetime &operator=(const Lifetime &other) = delete;
	Lifetime &operator=(Lifetime &&other) noexcept = delete;

	T &Get() const noexcept
	{
		assert((obj != nullptr) && "Invalid lifetime");
		return *obj;
	}
};

template <typename T> class Counted
{
	T obj;
	size_t count;

public:
	template <typename... Args>
	Counted(Args &&...args)
	    : T(std::forward<Args>(args)...)
	    , count(0)
	{
	}

	T &Get() const
	{
		return obj;
	};

	size_t GetCount() const
	{
		return count;
	}

	void Increase() noexcept
	{
		count++;
	}

	void Decrease() noexcept
	{
		count--;
	}
}

} // namespace crk

#endif