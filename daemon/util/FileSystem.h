/*
 *  This file is part of nzbget
 *
 *  Copyright (C) 2007-2015 Andrey Prygunkov <hugbug@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * $Revision$
 * $Date$
 *
 */


#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "NString.h"

class FileSystem
{
public:
	static CString GetLastErrorMessage();
	static char* BaseFileName(const char* filename);
	static bool SameFilename(const char* filename1, const char* filename2);
	static void NormalizePathSeparators(char* path);
	static bool LoadFileIntoBuffer(const char* fileName, char** buffer, int* bufferLength);
	static bool SaveBufferIntoFile(const char* fileName, const char* buffer, int bufLen);
	static bool CreateSparseFile(const char* filename, int64 size, CString& errmsg);
	static bool TruncateFile(const char* filename, int size);
	static void MakeValidFilename(char* filename, char cReplaceChar, bool allowSlashes);
	static CString MakeUniqueFilename(const char* destDir, const char* basename);
	static bool MoveFile(const char* srcFilename, const char* dstFilename);
	static bool CopyFile(const char* srcFilename, const char* dstFilename);
	static bool DeleteFile(const char* filename);
	static bool FileExists(const char* filename);
	static bool FileExists(const char* path, const char* filenameWithoutPath);
	static bool DirectoryExists(const char* dirFilename);
	static bool CreateDirectory(const char* dirFilename);
	static bool RemoveDirectory(const char* dirFilename);
	static bool DeleteDirectoryWithContent(const char* dirFilename, CString& errmsg);
	static bool ForceDirectories(const char* path, CString& errmsg);
	static CString GetCurrentDirectory();
	static bool SetCurrentDirectory(const char* dirFilename);
	static int64 FileSize(const char* filename);
	static int64 FreeDiskSize(const char* path);
	static bool DirEmpty(const char* dirFilename);
	static bool RenameBak(const char* filename, const char* bakPart, bool removeOldExtension, CString& newName);
#ifndef WIN32
	static CString ExpandHomePath(const char* filename);
	static void FixExecPermission(const char* filename);
#endif
	static CString ExpandFileName(const char* filename);
	static CString GetExeFileName(const char* argv0);

	/* Flush disk buffers for file with given descriptor */
	static bool FlushFileBuffers(int fileDescriptor, CString& errmsg);

	/* Flush disk buffers for file metadata (after file renaming) */
	static bool FlushDirBuffers(const char* filename, CString& errmsg);
};

class DirBrowser
{
private:
#ifdef WIN32
	WIN32_FIND_DATA		m_findData;
	HANDLE				m_file;
	bool				m_first;
#else
	DIR*				m_dir;
	struct dirent*		m_findData;
#endif

#ifdef DIRBROWSER_SNAPSHOT
	bool				m_snapshot;
	typedef std::deque<char*>	FileList;
	FileList			m_snapshot;
	FileList::iterator	m_itSnapshot;
#endif

	const char*			InternNext();
public:
#ifdef DIRBROWSER_SNAPSHOT
	DirBrowser(const char* path, bool snapshot = true);
#else
	DirBrowser(const char* path);
#endif
	~DirBrowser();
	const char*			Next();
};

class DiskFile
{
public:
	enum EOpenMode
	{
		omRead,			// file must exist
		omReadWrite,	// file must exist
		omWrite,		// create new or overwrite existing
		omAppend		// create new or append to existing
	};

	enum ESeekOrigin
	{
		soSet,
		soCur,
		soEnd
	};

private:
	FILE*				m_file = nullptr;

public:
						~DiskFile();
	bool				Open(const char* filename, EOpenMode mode);
	bool				Close();
	bool				Active() { return m_file != nullptr; }
	int64				Read(void* buffer, int64 size);
	int64				Write(const void* buffer, int64 size);
	int64				Position();
	int64				Seek(int64 position, ESeekOrigin origin = soSet);
	bool				Eof();
	bool				Error();
	int64				Print(const char* format, ...) PRINTF_SYNTAX(2);
	char*				ReadLine(char* buffer, int64 size);
	bool				SetWriteBuffer(int size);
	bool				Flush();
	bool				Sync(CString& errmsg);
};

#endif