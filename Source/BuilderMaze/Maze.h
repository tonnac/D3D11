#pragma once
#include "Plane_Object.h"
#include "DirectInput.h"

class Subject;

class Observer
{
public:
	Observer() = default;
public:
	void AddSubject(Subject* subject);
	void DelSubject(Subject* subject);
	void MoveX(const D2D1_POINT_2F& fVal);
	void MoveY(const D2D1_POINT_2F& fVal);
	inline void setCenterPos(const FLOAT& x, const FLOAT& y)
	{
		m_CenterPos = { x,y };
	}
private:
	std::vector<Subject*> m_Subject;
	D2D1_POINT_2F		  m_CenterPos;
};

class Subject
{
public:
	virtual void MoveX(const D2D1_POINT_2F& fVal) = 0;
	virtual void MoveY(const D2D1_POINT_2F& fVal) = 0;
private:

};
class Tile : public Subject
{
public:
	Tile() : _TileType(1), _CenterPos({ 0.0f,0.0f })
	{}
	INT _TileType;
	Plane_Object  _Object;
	D2D1_POINT_2F _CenterPos;
	void MoveX(const D2D1_POINT_2F& fVal)
	{
		for (int i = 0; i < _Object.m_VertexList.size(); ++i)
		{
			_Object.m_VertexList[i].Pos.x += fVal.x;
		}
	}
	void MoveY(const D2D1_POINT_2F& fVal)
	{
		//	D2D1_POINT_2F pe = _Object.ComputeCoord(fVal);
		for (int i = 0; i < _Object.m_VertexList.size(); ++i)
		{
			_Object.m_VertexList[i].Pos.y += fVal.y;
		}
	}
};
class Maze : public Plane_Object
{
	enum class DIR : unsigned char
	{
		LEFT = 0,
		TOP,
		RIGHT,
		BOTTOM
	};

	class MazeInfo
	{
	public:
		MazeInfo() : left(1), top(1), right(1), bottom(1), isVisit(false)
		{}
	public:
		inline int getLeft() const { return left; }
		inline int getTop() const { return top; }
		inline int getRight() const { return right; }
		inline int getBottom() const { return bottom; }
		inline bool getVisit() const { return isVisit; }
	public:
		inline void setLeft(const int& Left) { left = Left; }
		inline void setTop(const int& Top) { top = Top; }
		inline void setRight(const int& Right) { right = Right; }
		inline void setBottom(const int& Bottom) { bottom = Bottom; }
		inline void setVisit(const bool& Visit) { isVisit = Visit; }
	private:
		unsigned int left : 1;
		unsigned int top : 1;
		unsigned int right : 1;
		unsigned int bottom : 1;
		bool isVisit;
	};
	using TileArray = std::vector<std::vector<Tile>>;
	using MazeArray = std::vector<std::vector<MazeInfo>>;
public:
	void CreateMaze(ID3D11Device* pDevice, const INT& iWidth, const INT& iHeight);
public:
	void MoveXPos(const D2D1_POINT_2F& Pos);
	void MoveYPos(const D2D1_POINT_2F& Pos);
public:
	bool Frame() override;
	bool PostRender(ID3D11DeviceContext* pContext) override;
	bool PreRender(ID3D11DeviceContext* pContext) override;
	bool Render(ID3D11DeviceContext* pContext) override;
private:
	bool VisitVertex(const INT& x, const INT& y);
	void CreateTile(ID3D11Device* pDevice);
private:
	bool CanMove(const INT& x, const INT& y);
	bool CanMoveLeft(const INT& x, const INT& y);
	bool CanMoveTop(const INT& x, const INT& y);
	bool CanMoveRight(const INT& x, const INT& y);
	bool CanMoveBottom(const INT& x, const INT& y);
private:
	void CreateMazeVertex(ID3D11Device* pDevice, const FLOAT& OffsetWidth, const FLOAT& OffsetHeight);
private:
	INT m_iWidth;
	INT m_iHeight;
	INT m_iTileWidth;
	INT m_iTileHeight;
	MazeArray m_MazeArray;
	TileArray m_TileArray;
	Observer m_Observer;
};