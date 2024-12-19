#include "Memory.h"

#include <time.h>

#include <string>

#include <fstream>
#include <iostream>



void Memory::Main() 
{
	Start(); 

	InitWindow(widthHeight, widthHeight, "Memory Puzzle Game");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing(); 
		ClearBackground(bgColor);

		if (std::ifstream("Resource Files\\doggy.png"))
			std::cout << " found" << std::endl;
		else
			std::cout << "Not found" << std::endl;

		Image myImg = LoadImage("Resource Files\\doggy.png");





		Update(); 
		Draw(); 

		EndDrawing();
	}
	CloseWindow();
}

void Memory::Start()
{
	tilesArr.clear();
	int tileSize = (int)((widthHeight - (GAP * (gridSize + 1))) / gridSize);

	vector<int> numbers;
	for (int i = 0; i < gridSize * gridSize / 2; i++)
	{
		numbers.push_back(i + 1); // 1 -> 8
		numbers.push_back(i + 1); // 1 -> 8
	}

	srand(time(NULL));
	for (int row = 0; row < gridSize; row++)
		for (int col = 0; col < gridSize; col++)
		{
			int randomIndex = rand() % numbers.size(); // 132158
			int tileNum = numbers[randomIndex];
			Tile tile = Tile(	row, 
								col, 
								tileSize, 
								tileNum);
			tilesArr.push_back(tile); // populting the array
			numbers.erase(numbers.begin() + randomIndex);
		}
}

void Memory::Update()
{
	if (compareMode)
	{
		// conmpare them after waiting for a little whil;e
		if (GetTime() > timeOfSecondClick + WAIT_Time)
		{
			if (tile1->num == tile2->num)
			{
				tile1->state = Found; 
				tile2->state = Found;

				int counter = 0;
				
				for (int i = 0; i < tilesArr.size(); i++)
				{
					if (tilesArr[i].state == Found)
						counter++;
				}

				if (counter == tilesArr.size())
					gameFinished = true;

			}
			else // change back
			{
				tile1->state = Hidden; tile2->state = Hidden;
			}


			tile1 = nullptr; tile2 = nullptr;
			compareMode = false;
		}

		return;
	}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			for (int i = 0; i < tilesArr.size(); i++)
			{
				if (CheckCollisionPointRec(GetMousePosition(), tilesArr[i].rect))
			{
				if (tilesArr[i].state != Hidden) return;

				tilesArr[i].state = Flipped;

				if (tile1 == nullptr) 
					tile1 = &tilesArr[i];
				else
				{
					tile2 = &tilesArr[i];
					compareMode = true;
					timeOfSecondClick = GetTime();
				}
			}
		}
	}
}

void Memory::Draw()
{
	string txt = "Finished";

	if (!gameFinished)
		for (Tile tile : tilesArr)
			tile.Draw();
	else
		DrawText(txt.c_str(), 200, 200, 50, BLACK);

}
