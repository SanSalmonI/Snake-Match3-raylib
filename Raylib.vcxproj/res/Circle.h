#pragma once
#include <raylib.h>
#include <math.h>
#include <algorithm>


using namespace std;

struct Circle
{
	Vector2 center = {0, 0}; // the current center of the circle

	float angle = 0; // th e to which we should progress

	float radius = 0; // 
	Color color = BLUE;
	bool active = false;

	float gravity = 6;
	float speed = 0;


	void ForSpiro(Vector2 cen)
	{
		active = true;
		center = cen;
		radius = 1;
	}

	
	void Activate(Vector2 center, float angle, float radius,
		Color color, float speed)
	{
		this->center = center;
		this->angle = angle;
		this->radius = radius;
		this->color = color;
		this->speed = speed;
		this->active = true;
	}

	void Update()
	{
		if (!active) 
			return;


		// update Circles position
		// find the increments on x and y
		center.x += cos(angle * DEG2RAD) * speed;
		center.y += sin(angle * DEG2RAD) * speed + gravity;

		float alpha = color.a / 255.0; // 0 - 1
		alpha -= 0.01;

		if (alpha >= 0)
			color.a = alpha * 255;
		else
			active = false;

		speed -= 0.01; // it can become negative

		speed = max(speed, 1.0f);

		radius += 0.5; // 25 -> 85
	}

	void Draw()
	{
		if (!active)
			return;

		DrawCircleV(center, radius, color);
	}
};