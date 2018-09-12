#include "Define.h"
#include <comdef.h>

DxException::DxException(HRESULT hr, const std::tstring& functionName, const std::tstring& fileName, int lineNumber)
	: ErrorCode(hr), FunctionName(functionName), Filename(fileName), LineNumber(lineNumber)
{}

std::wstring DxException::ToString() const
{
	_com_error err(ErrorCode);
	std::wstring msg = err.ErrorMessage();

	return FunctionName + L"\nFile: " + Filename + L";\nLine: " + std::to_wstring(LineNumber) + L";\nError: " + msg;
}