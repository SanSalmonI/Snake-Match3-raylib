#include "Spiro.h"

#include <time.h>

#include <string>

#include <fstream>
#include <iostream>




void Spiro::Main()
{
	Start();

	InitWindow(800, 800, "Particle System");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Update();
		Draw();

		EndDrawing();
	}
	CloseWindow();
}

void Spiro::Start()
{

}

void Spiro::Update()
{
	Circle circ = Circle();

	float R = 168.0;
	float r = 55.0;
	float d = 95;

	int counter = GetTime() * 60;

	float x = 400 + (R - r) * cos(counter * DEG2RAD) + d * cos((R - r) / r * (counter * DEG2RAD));
	float y = 400 + (R - r) * sin(counter * DEG2RAD) - d * sin((R - r) / r * (counter * DEG2RAD));

	circ.ForSpiro({ x, y });


	pool.push_back(circ);
}

void Spiro::Draw()
{
	for (int i = 0; i < pool.size(); i++)
		pool[i].Draw();
}












