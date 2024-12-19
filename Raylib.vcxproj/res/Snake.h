#pragma once

#include "raylib.h"
#include <vector>

#ifndef SNAKE_H
#define SNAKE_H


class Snake
{
public:

#define TILE_SIZE 77

	static const int screenWidth = 800;
	static const int screenHeight = 600;

	Vector2 borderGap = { screenWidth % TILE_SIZE,
						  screenHeight % TILE_SIZE };

	typedef struct Piece
	{
		Vector2 position;
		Vector2 size = {TILE_SIZE, TILE_SIZE};
		Color color;

		void Draw() { DrawRectangleV(position, size, color); }

	} Piece;


	typedef struct Pickup
	{
		Vector2 position;
		Vector2 size = { TILE_SIZE, TILE_SIZE };
		Color color = SKYBLUE;
		bool active = false;

		void Draw() { DrawRectangleV(position, size, color); }

		void NewRandomLocation(Vector2 gap)
		{
			float ranX = GetRandomValue(0, (screenWidth / TILE_SIZE) - 1) * TILE_SIZE + gap.x / 2;
			float ranY = GetRandomValue (0, (screenHeight / TILE_SIZE) - 1) * TILE_SIZE + gap.y / 2;

			position = {ranX, ranY};
		}

	} Pickup;
	Pickup pickup = { 0 };

	std::vector<Piece> player; // so, please is a series of pieces that follow each other

	int frameCount = 0;
	Vector2 speed = {TILE_SIZE, 0};

	bool gameOver = false;

	void Main();
	void Start();
	void Draw();
	void Update();
	void Interaction();
};

#endif
