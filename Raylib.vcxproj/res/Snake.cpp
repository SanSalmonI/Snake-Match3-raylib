#include "Snake.h"
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>

// an operator to allow += on Vectors
Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}



void Snake::Main()
{
	Start();

	InitWindow(screenWidth, screenHeight, "Snake Game");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		Update();

		BeginDrawing();
		ClearBackground(BLACK);
		
		Draw();

		EndDrawing();
	}
	CloseWindow();
}

void Snake::Start()
{
	Piece head_1;
	head_1.color = DARKBLUE;
	head_1.position = Vector2{borderGap.x / 2, borderGap.y / 2};
	player.push_back(head_1);
}

void Snake::Update()
{
	if (!pickup.active)
	{
		pickup.active = true;
		pickup.NewRandomLocation(borderGap);
	}


	frameCount += 1; // every fame add one

	if (frameCount % 25 == 0)
	{
		player[0].position += speed;
	}

	Interaction();

	if (player[0].position.x >= (screenWidth - borderGap.x / 2) ||
		player[0].position.x < (borderGap.x / 2) ||
		player[0].position.y >= (screenHeight - borderGap.y / 2) ||
		player[0].position.y < (borderGap.y / 2))
	{
		// game is over
		gameOver = true;
	}

}

void Snake::Interaction()
{
	if (IsKeyPressed(KEY_RIGHT) && speed.x == 0) 
	{ 
		speed = {TILE_SIZE, 0};
	}
	if (IsKeyPressed(KEY_LEFT) && speed.x == 0)
	{
		speed = { -TILE_SIZE, 0 };
	}
	if (IsKeyPressed(KEY_UP) && speed.y == 0)
	{
		speed = { 0, -TILE_SIZE };
	}
	if (IsKeyPressed(KEY_DOWN) && speed.y == 0)
	{
		speed = { 0, TILE_SIZE };
	}

}

void Snake::Draw()
{
	if (gameOver)
	{
		std::string txt = "Game Over\n\nPress [ENTER] to Restart";
		int fontSize = 30;

		DrawText(txt.c_str(),
			screenWidth / 2 - MeasureText (txt.c_str(), fontSize) / 2,
			screenHeight / 2 - fontSize, 
			fontSize, WHITE);


		return;
	}


	for (Piece any : player)
		any.Draw();

	pickup.Draw();

	// draw vertical lines
	for (int i = 0; i <= screenWidth / TILE_SIZE; i++)
	{
		Vector2 start = { TILE_SIZE * i + borderGap.x / 2, borderGap.y / 2 };
		Vector2 end = { TILE_SIZE * i + borderGap.x / 2, screenHeight - borderGap.y / 2 };
		
		DrawLineV(start, end, GRAY);
	}

	// draw horizontal lines
	for (int i = 0; i <= screenHeight / TILE_SIZE; i++)
	{
		Vector2 start = { borderGap.x / 2, borderGap.y / 2 + TILE_SIZE * i };
		Vector2 end = { screenWidth - borderGap.x / 2, borderGap.y / 2 + TILE_SIZE * i };

		DrawLineV(start, end, GRAY);
	}


}












