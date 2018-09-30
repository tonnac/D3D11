#include "d3dUtil.h"
#include <comdef.h>

FilePathMap Filepath::m_Pngpath;
FilePathMap Filepath::m_Txtpath;

DxException::DxException(HRESULT hr, const std::tstring& functionname, const std::tstring& filename, const int& lineNumber)
	: ErrorCode(hr), Functionname(functionname), LineNumber(lineNumber)
{
	Filename = filename.substr(filename.find_last_of('\\') + 1, filename.length() - (filename.find_last_of(L".cpp") + 4));
}

std::tstring DxException::ToString() const
{
	_com_error err(ErrorCode);
	std::tstring msg = err.ErrorMessage();

	return  L"Failed: " + Functionname + L"\nFile: " + Filename + L"\nLine: " + std::to_tstring(LineNumber) + L"\nError: " + msg;
}

FileException::FileException(const std::tstring& Path, const std::tstring& filename, const std::tstring function, const int& line) :
Filepath(Path), Function(function), LineNumber(line)
{
	FileName = filename.substr(filename.find_last_of('\\') + 1, filename.length() - (filename.find_last_of(L".cpp") + 4));
	std::tstring Message = Function + L"\n" + FileName + L"\nLineNumber: " + std::to_tstring(LineNumber) + L"\n" + Filepath + L"\nCould not find.";
	MessageBox(nullptr, Message.c_str(), L"File Open Error", MB_OK);
}

bool Filepath::Init(const std::tstring& path)
{
	std::tstring FileType[] =
	{
		L"Txt",
		L"Png"
	};

	auto Searchstring = [FileType](const std::tstring& src)
	{
		int size = sizeof(FileType) / sizeof(FileType[0]);
		for (int i = 0; i < size; ++i)
		{
			if (src == FileType[i])
			{
				return true;
			}
		}
		return false;
	};

	std::tifstream fp;
	FileExceptErr(fp, path);
	std::tstring Type;
	while (!fp.eof())
	{
		std::tstring buffer;
		std::getline(fp, buffer);
		if (Searchstring(buffer) == true)
		{
			Type = buffer;
			continue;
		}
		std::tistringstream is(buffer);

		std::tstring first;
		std::tstring second;

		is >> first >> second;

		if (Type == FileType[0])
		{
			m_Txtpath[first] = second;
		}
		else if (Type == FileType[1])
		{
			m_Pngpath[first] = second;
		}
	}
	return true;
}