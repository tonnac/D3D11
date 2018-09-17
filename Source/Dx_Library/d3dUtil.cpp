#include "d3dUtil.h"
#include <comdef.h>

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