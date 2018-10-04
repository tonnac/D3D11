#include "MessageBox.h"


bool XMessageBox::Frame()
{
	if (isPopup == true)
	{
		m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
		m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
		UI::Frame();
	}
	return true;
}
bool XMessageBox::Render(ID3D11DeviceContext* pContext)
{
	if (isPopup == true)
	{
		UI::Render(pContext);
	}
	return true;
}

bool XMessageBox::getPopup() const
{
	return isPopup;
}
void XMessageBox::setPopup(const bool& flag)
{
	isPopup = flag;
}