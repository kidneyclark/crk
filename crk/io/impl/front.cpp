#include "../front.h"
#include <cstdio>
#include "../../mem/front.h"

using namespace crk::io;

class _impl_cstdioFile : public IStream
{
private:
	IDisk::OpenMode openMode;
	FILE *file;
public:
	_impl_cstdioFile(const char *path, IDisk::OpenMode mode)
	{
		const char *modestr = "";
		if (mode & IDisk::DISK_OPENREAD && mode & IDisk::DISK_OPENWRITE)
			modestr = "r+b";
		else if (mode & IDisk::DISK_OPENREAD)
			modestr = "rb";
		else
			modestr = "wb";
		file = fopen(path, modestr);
	}
	~_impl_cstdioFile()
	{
		fclose(file);
	}

	virtual void Flush()
	{
		fflush(file);
	}
	virtual void Seek(size_t offset, OriginType origin)
	{
		int stdorigin = 0;
		switch (origin) 
		{
		case IStream::STREAM_CUR:
			stdorigin = SEEK_CUR;
			break;
		case IStream::STREAM_SET:
			stdorigin = SEEK_SET;
			break;
		case IStream::STREAM_END:
			stdorigin = SEEK_END;
			break;
		}
		fseek(file, offset, stdorigin);
	}
	virtual size_t Tell()
	{
		return ftell(file);
	}
	virtual size_t Read(void *ptr, size_t size, size_t count)
	{
		return fread(ptr, size, count, file);
	}
	virtual size_t Write(const void *ptr, size_t size, size_t count)
	{
		return fwrite(ptr, size, count, file);
	}
};

class _impl_RealDisk : public IDisk
{
public:
	static constexpr u64 tag = crk::com::TagFromString("rdisk");
	_impl_RealDisk()
	{
		crk::mem::CreateContext();
		crk::mem::RegisterTag(tag, "Physical disk");
	}
	const char *GetName() const { return "PHYSICAL"; }
	IStream *OpenFile(const char *path, OpenMode mode)
	{
		return crk::mem::Alloc<_impl_cstdioFile>(tag, path, mode);
	};
	void CloseFile(IStream *file)
	{
		crk::mem::Free(file, tag);
	}
};

static _impl_RealDisk g_RealDisk;

namespace crk::io
{

IDisk *GetRealDisk()
{
	return &g_RealDisk;
}

} // namespace crk
