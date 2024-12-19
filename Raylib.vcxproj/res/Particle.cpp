#include "Particle.h"

#include <time.h>

#include <string>

#include <fstream>
#include <iostream>




void Particle::Main()
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

void Particle::Start() 
{
	for (int i = 0; i < POOL_SIZE; i++)
		circles.push_back(Circle());
}

void Particle::Update()
{
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		Vector2 mousePos = GetMousePosition();

		// rate
		int rate = rand() % 8 + 8;
		for (int i = 0; i < rate; i++) // 10
		{
			float angle = rand() % 360;
			float radius = 5 + rand() % 21;
			Color color = colors[rand() % colors.size()];
			float speed = 2 + ((float)(rand() % 100000)) / 10000; 

			// pick an inactive circle and bring it to life
			for (int i = 0; i < circles.size(); i++)
			{
				if (!circles[i].active)
				{
					circles[i].Activate(mousePos,
						angle,
						radius,
						color,
						speed);
					break;
				}
			}
		}
	}

	for (int i = 0; i < circles.size(); i++)
		circles[i].Update();
}

void Particle::Draw()
{
	for (int i = 0; i < circles.size(); i++)
		circles[i].Draw();
}












