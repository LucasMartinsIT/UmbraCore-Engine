#include "Game.h"
#include <iostream>

bool Game::Init()
{
	return true;
}

void Game::Update(float deltaTime)
{
	std::cout << "Current Delta time" << deltaTime << std::endl;
}

void Game::Destroy()
{

}