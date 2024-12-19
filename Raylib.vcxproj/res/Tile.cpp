#include "Tile.h"
#include "raylib.h"

#include "Memory.h"
#include <string>

using namespace std;

Tile::Tile(int h, int v, int tileSize, int num)
{
	rect = Rectangle{	(float)h * (tileSize + GAP) + GAP, 
						(float)v * (tileSize + GAP) + GAP, 
						(float)tileSize, 
						(float)tileSize};

	this->num = num;
}

void Tile::Draw()
{
	float fontSize = 50; // height
	string txt = "";
	float txtWidth;

	switch (state)
	{
	case Hidden:
		txt = "?"; bgColor = DARKGRAY; txtColor = WHITE;
		break;

	case Flipped:
		txt = to_string(num); bgColor = ORANGE; txtColor = BLACK;
		break;

	case Found:
		txt = ""; bgColor = GRAY; 
		break;

	default: break;
	}

	txtWidth = MeasureText(txt.c_str(), fontSize); // width of the question mark
	txtPos = { rect.x + rect.width / 2 - txtWidth / 2,
		   rect.y + rect.height / 2 - fontSize / 2 };
	DrawRectangleRounded(rect, 0.2, 12, bgColor);
	DrawText(txt.c_str(), txtPos.x, txtPos.y, fontSize, txtColor);

}