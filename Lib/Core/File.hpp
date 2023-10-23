#pragma once

#include "Defines.hpp"
#include "Containers\String.hpp"
#include "Containers\Vector.hpp"

enum FileOpenMode
{
	FILE_OPEN_BINARY = 0x8000,
	FILE_OPEN_TEXT = 0x4000,
	FILE_OPEN_UTF_16 = 0x20000,
	FILE_OPEN_UTF_8 = 0x40000,

	FILE_OPEN_READ = 0x0000,
	FILE_OPEN_WRITE = 0x0001,
	FILE_OPEN_READ_WRITE = 0x0002,

	FILE_OPEN_RANDOM = 0x0010,
	FILE_OPEN_SEQUENTIAL = 0x0020,

	FILE_OPEN_NEW = 0x0100 | 0x0200,
	FILE_OPEN_APPEND = 0x0008,

	FILE_OPEN_FLUSH_IMMEDIATE = 0x100000,

	FILE_OPEN_TEMPORARY = 0x0040,

	FILE_OPEN_LOG = FILE_OPEN_WRITE | FILE_OPEN_NEW | FILE_OPEN_SEQUENTIAL | FILE_OPEN_TEXT,
	FILE_OPEN_CONSOLE = FILE_OPEN_WRITE | FILE_OPEN_SEQUENTIAL | FILE_OPEN_TEXT | FILE_OPEN_FLUSH_IMMEDIATE,
	FILE_OPEN_RESOURCE_READ = FILE_OPEN_READ | FILE_OPEN_SEQUENTIAL | FILE_OPEN_BINARY,
	FILE_OPEN_RESOURCE_WRITE = FILE_OPEN_WRITE | FILE_OPEN_NEW | FILE_OPEN_SEQUENTIAL | FILE_OPEN_BINARY,
	FILE_OPEN_TEMP_RESOURCE = FILE_OPEN_WRITE | FILE_OPEN_SEQUENTIAL | FILE_OPEN_BINARY | FILE_OPEN_NEW | FILE_OPEN_FLUSH_IMMEDIATE | FILE_OPEN_TEMPORARY,
};

struct FileStats
{
	U32 driveNumber;
	U16 informationNode;
	U16 mode;
	I16 nlink;
	I16 userId;
	I16 groupId;
	U32 driveNumber2;
	I64 size;
	I64 lastAccessed;
	I64 lastModified;
	I64 creationTime;
};

static constexpr I32 READ_MODE = 0x0001;
static constexpr I32 WRITE_MODE = 0x0002;
static constexpr I32 READ_WRITE_MASK = 0x0003;

//TODO: Make thread safe, use mutex
//TODO: Documentation
//TODO: Read and Write at same time
struct File
{
public:
	File();
	template <typename Str> File(const Str& path, I32 mode);
	~File();
	void Destroy();
	void Close();

	template <StringType Str> bool Open(const Str& path, I32 mode);
	bool Open(const C8* path, I32 mode);
	bool Opened() const;

	template <StringType Str> U32 ReadAll(Str& data);
	template<typename Type> U32 ReadAll(Vector<Type>& data);
	template<typename Type> U32 ReadAll(Type** data);

	template <StringType Str> U32 ReadCount(Str& data, U32 count);
	template<typename Type> U32 ReadCount(Vector<Type>& data, U32 count);
	template<typename Type> U32 ReadCount(Type* data, U32 count);

	template<typename Type> U32 Read(Type& value);
	U32 Read(void* buffer, U32 size);
	String ReadString();
	String ReadLine();

	template <StringType Str> U32 Write(const Str& data);
	template<typename Type> U32 Write(const Vector<Type>& data);

	template <StringType Str> U32 WriteCount(const Str& data, U32 count);
	template<typename Type> U32 WriteCount(const Type* data, U32 count);

	template<typename Type> U32 Write(const Type& value);
	U32 Write(const void* buffer, U32 size);

	void Reset();
	void Seek(I64 offset);
	void SeekFromStart(I64 offset);
	void SeekToEnd();

	I64 Pointer() const;
	I64 Size();

	static const String& WorkingDirectory();
	static bool Delete(const String& path);

private:
	bool Flush();
	bool FillBuffer();
	bool EmptyBuffer();

	static bool GetWorkingDirectory(String& str);

	I32 handle{ -1 };
	I64 pointer{ 0 };
	bool opened{ false };

	U8* streamPtr{ nullptr };
	U8* streamBuffer{ nullptr };
	U32 bufferSize{ 0 };
	U32 bufferRemaining{ 0 };
	U64 streamFlag{ 0 };

	FileStats stats{};
};

template <typename Str>
inline File::File(const Str& path, I32 mode) { Memory::AllocateArray(&streamBuffer, bufferSize, bufferSize); streamPtr = streamBuffer; Open(path, mode); }

template <StringType Str>
inline bool File::Open(const Str& path, I32 mode)
{
	C8* p = path.ToType<C8*>();
	return Open(p, mode);
}

template <StringType Str>
inline U32 File::ReadAll(Str& data)
{
	data.Resize(stats.size);
	return Read(data.Data(), (U32)stats.size);
}

template <typename Type>
inline U32 File::ReadAll(Vector<Type>& data)
{
	data.Resize(stats.size);
	return Read(data.Data(), (U32)stats.size);
}

template<typename Type> 
inline U32 File::ReadAll(Type** data)
{
	Memory::AllocateSize(data, (U32)stats.size);
	return Read((void*)*data, (U32)stats.size);
}

template <StringType Str>
inline U32 File::ReadCount(Str& data, U32 count)
{
	data.Resize(count);
	return Read(data.Data(), count);
}

template <typename Type>
inline U32 File::ReadCount(Vector<Type>& data, U32 count)
{
	data.Resize(count);
	return Read(data.Data(), count);
}

template<typename Type> 
inline U32 File::ReadCount(Type* data, U32 count)
{
	return Read(data, count);
}

template <typename Type>
inline U32 File::Read(Type& value)
{
	return Read(&value, sizeof(Type));
}

template <StringType Str>
inline U32 File::Write(const Str& data)
{
	return Write(data.Data(), (U32)data.Size() + 1);
}

template <typename Type>
inline U32 File::Write(const Vector<Type>& data)
{
	return Write(data.Data(), sizeof(Type) * data.Size());
}

template <StringType Str>
inline U32 File::WriteCount(const Str& data, U32 count)
{
	return Write(data.Data(), count);
}

template <typename Type>
inline U32 File::WriteCount(const Type* data, U32 count)
{
	return Write(data, sizeof(Type) * count);
}

template <typename Type>
inline U32 File::Write(const Type& value)
{
	return Write(&value, sizeof(Type));
}