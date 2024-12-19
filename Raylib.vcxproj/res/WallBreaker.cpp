#include "WallBreaker.h"

// finish the game
// calculate collisions with bricks and boundaries
// Create ball re-direction upon hit
// connect ball exit to the player lives
// update player lives on screen
// have game over menu when out of lives
// produce a trail behind the ball
// add power ups in some bricks that player has to collect
// if the all bricks are broken, load next level
// generate levels randomly



void WallBreaker::Main()
{
	Start();

	InitWindow(screenWidth, screenHeight, "Wall Breaker Game");
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

void WallBreaker::Start()
{
	bricks.clear();

	brickHeight = screenHeight * 0.05;
	brickGap = brickHeight * 0.4;

	rowCount = 5; // 4 + rand() % 4; // 4 - 8
	colCount = 8; // +rand() % 4; // 8 - 12

	float spaceForBriks = screenWidth - (colCount * brickGap + brickGap);
	float brickWdith = spaceForBriks / colCount;
	Vector2 brickSize = { brickWdith, brickHeight };

	for (int row = 0; row < rowCount; row++)
	{
		for (int col = 0; col < colCount; col++)
		{
			float x = brickGap + (brickWdith + brickGap) * col;
			float y = brickGap + (brickHeight + brickGap) * row;

			Rectangle rect = { x, y, brickWdith, brickHeight };

			Brick brick = Brick{ YELLOW, rect };
			bricks.push_back(brick);
		}
	}








	player.curLife = MAX_LIVES;
	player.position = {screenWidth / 2, screenHeight - 100};
}

void WallBreaker::Update()
{
	// temp
	player.curLife = 3;

#pragma region playerControl

	if (IsKeyDown(KEY_LEFT))
		player.position.x -= 10;

	if (player.position.x - player.size.x / 2 < 0)
		player.position.x = player.size.x / 2;

	if (IsKeyDown(KEY_RIGHT))
		player.position.x += 10;

	if (player.position.x > screenWidth - player.size.x / 2)
		player.position.x = screenWidth - player.size.x / 2;
#pragma endregion

	if (IsKeyPressed(KEY_SPACE))
	{
		ball.active = true;
		ball.speed = { 0, -5 };
	}

	if (!ball.active)
		ball.position = {	player.position.x, 
							player.position.y + player.size.y / 2 - ball.radius};
	else
	{
		ball.position.x += ball.speed.x;
		ball.position.y += ball.speed.y;
	}


}						

void WallBreaker::Draw()
{
	player.Draw();
	ball.Draw();

	for (Brick brick : bricks)
		brick.Draw();


	int cellWidth = 30;
	int cellHeight = 15;
	int cellGap = 10;
	int borderGap = 20;

	for (int i = 0; i < MAX_LIVES; i++)
	{
		Color cellColor = DARKGRAY;

		if (i < player.curLife)
			cellColor = RAYWHITE;

		int x = borderGap + (cellWidth + cellGap) * i;
		int y = screenHeight - borderGap - cellHeight;
		DrawRectangle(x, y, cellWidth, cellHeight, cellColor);
	}

}
















