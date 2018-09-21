#pragma once
#include "Maze.h"

class MazeBuilder
{
public:
	Maze CreateMaze(ID3D11Device* pDevice, const INT& iWidth, const INT& iHeight);
};