#pragma once
#include "Core.h"
#include "Maze.h"
#include "MazeBuilder.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	MazeBuilder			m_Builder;
	Maze				m_Maze;
};