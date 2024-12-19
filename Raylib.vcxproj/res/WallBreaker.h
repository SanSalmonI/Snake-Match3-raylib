#pragma once

#include "raylib.h"
#include <vector>
#include "Tile.h"

#ifndef WALLBREAKER_H
#define WALLBREAKER_H

using namespace std;

class WallBreaker
{
public:
#define MAX_LIVES 5


	float brickGap;
	float brickHeight;
	int rowCount;
	int colCount;



	// alias for struct
	// for better readbility
	typedef struct Player
	{
		int curLife;
		Vector2 position; // the top left of the rect
		Vector2 size = {125, 25};

		void Draw()
		{
			int xCen = position.x - size.x / 2;
			int yCen = position.y + size.y / 2;

			Rectangle rect = Rectangle{	(float)xCen,
										(float)yCen, 
										size.x, 
										size.y };

			DrawRectangleRounded(rect, 0.4, 12, DARKGREEN);
		}


	} Player;

	Player player = { 0 }; // instantiate my player

	typedef struct Ball
	{
		Vector2 position;
		int radius = 12;
		bool active = false;
		Vector2 speed;

		void Draw()
		{
			DrawCircle(position.x, position.y, radius, BLUE);
		}
	} Ball;

	Ball ball = { 0 };

	typedef struct Brick
	{
		Color color;
		Rectangle rect;
		void Draw()
		{
			DrawRectangleRounded(rect, 0.25, 8, color);
		}
	} Brick;

	std::vector<Brick> bricks;


	static const int screenWidth = 1400;
	static const int screenHeight = 500;


	void Main();
	void Start();
	void Draw();
	void Update();
};

#endif 
#pragma once
