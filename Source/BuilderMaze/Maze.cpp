#include "Maze.h"

void Observer::AddSubject(Subject* subject)
{
	m_Subject.push_back(subject);
}
void Observer::DelSubject(Subject* subject)
{
	std::vector<Subject*>::iterator iter;
	iter = std::find(m_Subject.begin(), m_Subject.end(), subject);
	m_Subject.erase(iter);
}
void Observer::MoveX(const D2D1_POINT_2F& fVal)
{
	for (int i = 0; i < m_Subject.size(); ++i)
	{
		m_Subject[i]->MoveX(fVal);
	}
}
void Observer::MoveY(const D2D1_POINT_2F& fVal)
{
	for (int i = 0; i < m_Subject.size(); ++i)
	{
		m_Subject[i]->MoveY(fVal);
	}
}

void Maze::CreateMaze(ID3D11Device* pDevice,const INT& iWidth, const INT& iHeight)
{
	m_Observer.setCenterPos(0, 0);
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	std::vector<MazeInfo> maze(iWidth, MazeInfo());
	m_MazeArray.assign(iHeight, maze);
	VisitVertex(0, 0);

	CreateTile(pDevice);
}
void Maze::MoveXPos(const D2D1_POINT_2F& Pos)
{
	m_Observer.MoveX(Pos);
}
void Maze::MoveYPos(const D2D1_POINT_2F& Pos)
{
	m_Observer.MoveY(Pos);
}
bool Maze::VisitVertex(const INT& x, const INT& y)
{
	m_MazeArray[y][x].setVisit(true);
	while (CanMove(x, y))
	{
		DIR type = static_cast<DIR>(rand() % 4);
		switch (type)
		{
		case DIR::LEFT:
		{
			if (CanMoveLeft(x, y) == true)
			{
				m_MazeArray[y][x].setLeft(0);
				m_MazeArray[y][x - 1].setRight(0);
				VisitVertex(x - 1, y);
			}
		}break;
		case DIR::TOP:
		{
			if (CanMoveTop(x, y) == true)
			{
				m_MazeArray[y][x].setTop(0);
				m_MazeArray[y - 1][x].setBottom(0);
				VisitVertex(x, y - 1);
			}
		}break;
		case DIR::RIGHT:
		{
			if (CanMoveRight(x, y) == true)
			{
				m_MazeArray[y][x].setRight(0);
				m_MazeArray[x + 1][y].setLeft(0);
				VisitVertex(x + 1, y);
			}
		}break;
		case DIR::BOTTOM:
		{
			if (CanMoveBottom(x, y) == true)
			{
				m_MazeArray[y][x].setBottom(0);
				m_MazeArray[y + 1][x].setTop(0);
				VisitVertex(x, y + 1);
			}
		}break;
		}
	}
	return true;
}
void Maze::CreateTile(ID3D11Device* pDevice)
{
	m_iTileWidth = m_iWidth * 2 + 1;
	m_iTileHeight = m_iHeight * 2 + 1;
	std::vector<Tile> tile(m_iTileWidth, Tile());
	m_TileArray.assign(m_iTileHeight, tile);

	for (int i = 0; i < m_iHeight; ++i)
	{
		for (int j = 0; j < m_iWidth; ++j)
		{
			int k = 2 * i + 1;
			int p = 2 * j + 1;
			m_TileArray[k][p]._TileType = 0;
			if (m_MazeArray[i][j].getLeft() == 0)
			{
				m_TileArray[k][p - 1]._TileType = 0;
			}
			if (m_MazeArray[i][j].getTop() == 0)
			{
				m_TileArray[k - 1][p]._TileType = 0;
			}
			if (m_MazeArray[i][j].getRight() == 0)
			{
				m_TileArray[k][p + 1]._TileType = 0;
			}
			if (m_MazeArray[i][j].getBottom() == 0)
			{
				m_TileArray[k + 1][p]._TileType = 0;
			}
		}
	}
	FLOAT iOffSetWidth = g_rtClient.right / CASTING(FLOAT, m_iTileWidth);
	FLOAT iOffSetHeight = g_rtClient.bottom / CASTING(FLOAT, m_iTileHeight);
	FLOAT halfX = iOffSetWidth * 0.5f;
	FLOAT halfY = iOffSetHeight * 0.5f;
	for (int y = 0; y < m_iTileHeight; ++y)
	{
		for (int x = 0; x < m_iTileWidth; ++x)
		{
			m_TileArray[y][x]._CenterPos = { x * iOffSetWidth + halfX, y*iOffSetHeight + halfY };
		}
	}
	CreateMazeVertex(pDevice, iOffSetWidth, iOffSetHeight);
	for (int i = 0; i < m_iTileHeight; ++i)
	{
		for (int j = 0; j < m_iTileWidth; ++j)
		{
			m_Observer.AddSubject(&m_TileArray[i][j]);
		}
	}
}
bool Maze::CanMove(const INT& x, const INT& y)
{
	return CanMoveLeft(x, y) || CanMoveTop(x, y) || CanMoveRight(x, y) || CanMoveBottom(x, y);
}
bool Maze::CanMoveLeft(const INT& x, const INT& y)
{
	if (x > 0 && m_MazeArray[y][x - 1].getVisit() == false) return true;
	return false;
}
bool Maze::CanMoveTop(const INT& x, const INT& y)
{
	if (y > 0 && m_MazeArray[y - 1][x].getVisit() == false) return true;
	return false;
}
bool Maze::CanMoveRight(const INT& x, const INT& y)
{
	if (x < m_iWidth - 1 && m_MazeArray[y][x + 1].getVisit() == false) return true;
	return false;
}
bool Maze::CanMoveBottom(const INT& x, const INT& y)
{
	if (y < m_iHeight - 1 && m_MazeArray[y + 1][x].getVisit() == false) return true;
	return false;
}
void Maze::CreateMazeVertex(ID3D11Device* pDevice,const FLOAT& OffsetWidth, const FLOAT& OffsetHeight)
{
	for (int i = 0; i < m_iTileHeight; ++i)
	{
		for (int j = 0; j < m_iTileWidth; ++j)
		{
			P3_VERTEX vertex[4];
			D2D1_POINT_2F ep = ComputeCoord(m_TileArray[i][j]._CenterPos);
			FLOAT fWidth = OffsetWidth / g_rtClient.right;
			FLOAT fHeight = OffsetHeight / g_rtClient.bottom;
			P3_VERTEX e[4];
			for (int k = 0; k < 4; ++k)
			{
				e[k].Pos.z = 0.5f;
			}
			e[0].Pos.x = ep.x - fWidth;
			e[0].Pos.y = ep.y + fHeight;
			e[0].TexPos.x = 0.0f;
			e[0].TexPos.y = 0.0f;

			e[1].Pos.x = ep.x + fWidth;
			e[1].Pos.y = e[0].Pos.y;
			e[1].TexPos.x = 1.0f;
			e[1].TexPos.y = 0.0f;

			e[2].Pos.x = e[0].Pos.x;
			e[2].Pos.y = ep.y - fHeight;
			e[2].TexPos.x = 0.0f;
			e[2].TexPos.y = 1.0f;

			e[3].Pos.x = e[1].Pos.x;
			e[3].Pos.y = e[2].Pos.y;
			e[3].TexPos.x = 1.0f;
			e[3].TexPos.y = 1.0f;

			vertex[0] = e[0];
			vertex[1] = e[1];
			vertex[2] = e[2];
			vertex[3] = e[3];
			for (int k = 0; k < 4; ++k)
			{
				m_TileArray[i][j]._Object.VertexAdd(vertex[k]);
			}
			if (m_TileArray[i][j]._TileType == 0)
			{
				m_TileArray[i][j]._Object.InitSet(pDevice, L"Maze", L"../../data/sand.bmp", L"vertexshader.txt");
			}
			else
				m_TileArray[i][j]._Object.InitSet(pDevice, L"Maze1", L"../../data/tile4.bmp", L"vertexshader.txt");
		}
	}
}
bool Maze::PostRender(ID3D11DeviceContext* pContext)
{
	return true;
}
bool Maze::PreRender(ID3D11DeviceContext* pContext)
{
	return true;
}
bool Maze::Render(ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < m_iTileHeight; ++i)
	{
		for (int j = 0; j < m_iTileWidth; ++j)
		{
			m_TileArray[i][j]._Object.Render(pContext);
		}
	}
	return true;
}
bool Maze::Frame()
{
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		D2D1_POINT_2F Cur = m_Centerpos;
		m_Centerpos.x += g_fSecPerFrame;
		Cur.x = Cur.x - m_Centerpos.x;
		MoveXPos(Cur);
	}
	if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_HOLD)
	{
		D2D1_POINT_2F Cur = m_Centerpos;
		m_Centerpos.y -= g_fSecPerFrame;
		Cur.y = Cur.y - m_Centerpos.y;
		MoveYPos(Cur);
	}
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		D2D1_POINT_2F Cur = m_Centerpos;
		m_Centerpos.x -= g_fSecPerFrame;
		Cur.x = Cur.x - m_Centerpos.x;
		MoveXPos(Cur);
	}
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_HOLD)
	{
		D2D1_POINT_2F Cur = m_Centerpos;
		m_Centerpos.y += g_fSecPerFrame;
		Cur.y = Cur.y - m_Centerpos.y;
		MoveYPos(Cur);
	}
	return true;
}