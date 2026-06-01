#pragma once
#include <eng.h>
#include <memory>

class Game : public eng::Application
{
public:
	bool Init() override;
	void Update(float deltaTime) override;
	void Destroy() override;

private:
	eng::Scene* m_scene = nullptr;

	//Lista para os blocos do chão 
	std::vector<eng::GameObject*> m_groundBlocks;

	//Lista para os obstáculos em cima do chão
	std::vector<eng::GameObject*> m_mapObjects;
};