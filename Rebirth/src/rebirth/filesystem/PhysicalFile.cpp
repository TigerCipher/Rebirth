// ------------------------------------------------------------------------------
// 
// Rebirth
//    Copyright 2022 Matthew Rogers
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// 
// File Name: PhysicalFile.cpp
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "PhysicalFile.h"

namespace rebirth
{

	// #CONSIDER: Require Windows Only
	// For now, this file requires Windows. Being realistic... Most people play games on Windows... I want to support directx at some point. AND, who else but me will ultimately use this engine? I really should consider requiring windows... then it's only graphics API stuff I need to abstract

	PhysicalFile::~PhysicalFile()
	{
		PhysicalFile::Close();
	}

	bool PhysicalFile::SetPointer(const size_t position)
	{
		return SetPointerInternal(position, false);
	}

	bool PhysicalFile::SetPointerFromEnd(const size_t position)
	{
		return SetPointerInternal(position, true);
	}

	size_t PhysicalFile::Read(void* dest, size_t size)
	{
		auto bytesToRead = (ulong)size;

		while (bytesToRead > 0)
		{
			ulong read = 0;
			if (!ReadFile(mHandle, (char*)dest + size - bytesToRead, bytesToRead, &read, nullptr) || read == 0)
				return size - bytesToRead;

			bytesToRead -= read;
			mFilePointer += read;
		}

		return size - bytesToRead;
	}

	size_t PhysicalFile::Write(const void* data, size_t size)
	{
		if (mHandle == INVALID_HANDLE)
			return 0;

		auto bytesToWrite = (ulong)size;

		while (bytesToWrite > 0)
		{
			ulong written = 0;
			if (!WriteFile(mHandle, data, bytesToWrite, &written, nullptr))
				return 0;
			bytesToWrite -= written;
			mFilePointer += written;
			mSize += written;
		}

		return size - bytesToWrite;
	}

	bool PhysicalFile::OpenRead(const bool allowWrite)
	{
		ulong access = GENERIC_READ;
		if (allowWrite)
			access |= GENERIC_WRITE;

		const ulong creationDisposition = allowWrite ? CREATE_NEW : OPEN_EXISTING;

		mHandle = CreateFileA(mPhysicalPath.c_str(), access, 0, nullptr, creationDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (mHandle == INVALID_HANDLE)
		{
			mSize = 0;
			return false;
		}

		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(mHandle, &fileSize))
			mSize = 0;
		else mSize = (size_t)fileSize.QuadPart;

		return true;
	}

	static std::string GetLastErrorMsg()
	{
		ulong msgId = GetLastError();
		if (!msgId)
			return {};
		char* msgBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, msgId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&msgBuffer, 0, nullptr);
		std::string msg(msgBuffer, size);

		LocalFree(msgBuffer);

		return msg;
	}

	bool PhysicalFile::OpenWrite(const bool append, const bool allowRead)
	{
		ulong access = GENERIC_WRITE;
		if (allowRead)
			access |= GENERIC_READ;
		if (append)
			access |= FILE_APPEND_DATA;
		//if (!CreateDirectoryTree(mPhysicalPath))
		//{
		//	return false;
		//}

		CreateDirectoryTree(mPhysicalPath);

		//const ulong creationDisposition = !file::Exists(mPhysicalPath) ? CREATE_NEW : append ? CREATE_ALWAYS : OPEN_EXISTING;
		const ulong creationDisposition = !file::Exists(mPhysicalPath) ? CREATE_NEW : append ? OPEN_ALWAYS : CREATE_ALWAYS;

		mHandle = CreateFileA(mPhysicalPath.c_str(), access, 0, nullptr, creationDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (mHandle == INVALID_HANDLE_VALUE)
		{
			mSize = 0;
			RB_CORE_WARN(GetLastErrorMsg());
			return false;
		}

		if (append && !SetPointerFromEnd(0))
			return false;
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(mHandle, &fileSize))
			mSize = 0;
		else mSize = (size_t)fileSize.QuadPart;

		return true;
	}

	bool PhysicalFile::Flush()
	{
		if (mHandle == INVALID_HANDLE)
			return false;
		return FlushFileBuffers(mHandle) > 1;
	}

	bool PhysicalFile::Close()
	{
		if (mHandle != INVALID_HANDLE)
		{
			CloseHandle(mHandle);
			mHandle = INVALID_HANDLE;
		}

		return mHandle == INVALID_HANDLE;
	}

	bool PhysicalFile::IsOpen() const
	{
		return mHandle != INVALID_HANDLE;
	}

	bool PhysicalFile::CreateDirectoryTree(const std::string& path)
	{
		return fs::create_directories(file::GetDirectoryPath(path));
	}

	bool PhysicalFile::SetPointerInternal(const size_t position, const bool fromEnd)
	{
		if (mHandle == INVALID_HANDLE)
		{
			RB_CORE_WARN("Invalid file handle while trying to set file pointer");
			return false;
		}

		//if (mFilePointer == position)
		//{
		//	return true;
		//}

		ulong moved = SetFilePointer(mHandle, 0, nullptr, fromEnd ? FILE_END : FILE_BEGIN);
		if (moved == INVALID_SET_FILE_POINTER)
			return false;

		mFilePointer = (size_t)moved;
		RB_CORE_WARN("File pointer: {}", mFilePointer);
		return true;
	}


}

