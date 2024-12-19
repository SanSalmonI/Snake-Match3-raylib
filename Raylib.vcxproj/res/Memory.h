#pragma once

#include "raylib.h"
#include <vector>
#include "Tile.h"

#ifndef MEMORY_H
#define MEMORY_H
using namespace std;

class Memory
{
public:
#define GAP 8 // 8 pixels gap between each two tiles
#define WAIT_Time 0.5

	Color bgColor = RAYWHITE;

	int gridSize = 6; // it's a 4 by 4 grid
	static const int widthHeight = 800; // width and height of canvas
	vector<Tile> tilesArr;
	bool compareMode = false;

	Tile* tile1 = nullptr;
	Tile* tile2 = nullptr;

	float timeOfSecondClick;

	bool gameFinished = false;

	void Main();
	void Start();
	void Draw();
	void Update();
};

#endif 
#pragma once
