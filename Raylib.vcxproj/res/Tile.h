#pragma once

#include "raylib.h"

#ifndef TILE_H
#define TILE_H


enum TileState
{
	Hidden, Flipped, Found
};



class Tile
{
public:

	Tile(int h, int v, int tileSize, int num);

	Rectangle rect; // rect where we draw the tile
	Vector2 txtPos = Vector2{};
	int num; // this is the number shown on the tile

	TileState state = TileState::Hidden;

	Color bgColor, txtColor;

	void Draw();

	// i need to know what state they are in 
	// Hidden, Flipped, Matche
};




#endif // !TILE_H



/*
	we land a click -> find out if it hits any one of the tiles
		if a tile -> reveal it
	click one has been registered -. next is a compare mode
	we land 2nd click 
		if a tile -> reveal AND compare
		wait for a 1/2 second
		Compare
			they are a match -> remove from the array
			they are not a match -> flip the card, hide again
		de-register click on -> next time, it's again the 1st click

	Continue until array empty

	states of each tile
		HIDDEN (default -> ?), FLIPPED (num), MATCHED (emoji)
		
	Game Over - [Time-based, Move-based] -> Restart Button
	Game Finished -> When all are matched -> Restart Button
	Quit Button
	Level selection button
	Numbers vs Images Mode





*/