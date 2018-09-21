#include "MazeBuilder.h"

Maze MazeBuilder::CreateMaze(ID3D11Device* pDevice, const INT& iWidth, const INT& iHeight)
{
	Maze maze;
	maze.CreateMaze(pDevice, iWidth, iHeight);
	return maze;
}