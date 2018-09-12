#include "Define.h"
#include <comdef.h>


DxException::DxException(HRESULT hr, const std::tstring& functionName, const std::tstring& fileName, int lineNumber)
	: ErrorCode(hr), FunctionName(functionName), FileName(fileName), LineNumber(lineNumber)
{}

std::tstring DxException::ToString() const
{
	_com_error err(ErrorCode);
	std::tstring msg = err.ErrorMessage();


	return FunctionName + L"failed in " + FileName + L"; line " + std::to_tstring(LineNumber) + L"; error: " + msg;
}