#ifndef CRK_IO_TYPES_H
#define CRK_IO_TYPES_H

#include "../com/types.h"

#include <vector>

namespace crk::io
{
class IStream
{
public:
	virtual ~IStream()
	{
	}
	enum OriginType
	{
		STREAM_SET,
		STREAM_CUR,
		STREAM_END
	};
	virtual bool IsValid() = 0;
	virtual void Flush() = 0;
	virtual void Seek(size_t offset, OriginType origin) = 0;
	virtual size_t Tell() = 0;
	virtual size_t Read(void *ptr, size_t size, size_t count) = 0;
	template <typename T> size_t Read(T *ptr, size_t count)
	{
		return Read((void *)ptr, sizeof(T), count);
	}
	virtual size_t Write(const void *ptr, size_t size, size_t count) = 0;
	template <typename T> size_t Write(const T *ptr, size_t count)
	{
		return Write((const void *)ptr, sizeof(T), count);
	}
	size_t GetSize()
	{
		size_t old_seek = Tell();
		Seek(0, STREAM_END);
		size_t filesize = Tell();
		Seek(old_seek, STREAM_SET);
		return filesize;
	}
};

class IDisk
{
public:
	enum OpenMode
	{
		DISK_OPENREAD = 1,
		DISK_OPENWRITE = 2,
	};
	virtual const char *GetName() const = 0;
	virtual IStream *OpenFile(const char *path, OpenMode mode) = 0;
	virtual void CloseFile(IStream *file) = 0;
	std::vector<u8> ReadFile(const char *path)
	{
		IStream *file = OpenFile(path, DISK_OPENREAD);
		if (!file->IsValid())
		{
			return {};
		}
		size_t size = file->GetSize();
		std::vector<u8> data(size);
		file->Read(data.data(), size, 1);
		CloseFile(file);
		return data;
	}
};

} // namespace crk::io

#endif