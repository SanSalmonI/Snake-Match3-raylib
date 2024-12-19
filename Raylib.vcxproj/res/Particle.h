#pragma once

#include "raylib.h"
#include <vector>
#include "Tile.h"

#include "Circle.h"

#include <vector>

#ifndef PARTICLE_H
#define PARTICLE_H

using namespace std;

class Particle
{
public:

#define POOL_SIZE 5000 // there's always 500 partciles instantiated

	vector<Circle> circles;
	vector<Color> colors = {BLUE, RED, GREEN, YELLOW, ORANGE,
	PURPLE, PINK, WHITE, BROWN, LIGHTGRAY, 
		SKYBLUE, GOLD, LIME, VIOLET, BEIGE};

	void Main();
	void Start();
	void Draw();
	void Update();
};

#endif 
#pragma once
