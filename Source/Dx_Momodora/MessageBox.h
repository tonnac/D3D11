#pragma once
#include "UI.h"

class XMessageBox : public UI
{
public:
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext) override;
	bool getPopup() const;
	void setPopup(const bool& flag);
private:
	bool isPopup;
};

using MsgPTR = std::shared_ptr<XMessageBox>;