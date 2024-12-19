#pragma once

#include "raylib.h"
#include <vector>
#include "Tile.h"

#include "Circle.h"

#include <vector>

#ifndef SPIRO_H
#define SPIRO_H

using namespace std;

class Spiro
{
public:

#define POOL_SIZE 15000

	vector<Circle> pool;


	void Main();
	void Start();
	void Draw();
	void Update();
};

#endif 
#pragma once
