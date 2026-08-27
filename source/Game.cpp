#include "Game.h"
#include "TestObject.h"
#include <iostream>

bool Game::Init()
{
    auto& fs = eng::Engine::GetInstance().GetFileSystem();
    auto texture = eng::Texture::Load("brick.png");

	m_scene = new eng::Scene();

	auto camera = m_scene->CreateObject("Camera");
	camera->AddComponent(new eng::CameraComponent());
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	camera->AddComponent(new eng::PlayerControllerComponent());

	m_scene->SetMainCamera(camera);

	m_scene->CreateObject<TestObject>("TestObject");
    

    auto material = eng::Material::Load("materials/brick.mat");

    

    auto mesh = eng::Mesh::CreateCube();

    auto objectA = m_scene->CreateObject("ObjectA");
    objectA->AddComponent(new eng::MeshComponent(material, mesh));
    objectA->SetPosition(glm::vec3(1.0f, 1.0f, -5.0f));

    auto objectB = m_scene->CreateObject("ObjectB");
    objectB->AddComponent(new eng::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));
    objectB->SetScale(glm::vec3(0.5f, 0.5f, 1.0f));


    auto objectC = m_scene->CreateObject("ObjectC");
    objectC->AddComponent(new eng::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.0f, 0.5f, 1.3f));

    auto suzanneMesh = eng::Mesh::Load("models/Suzanne.gltf");
	auto suzanneMaterial = eng::Material::Load("materials/suzanne.mat");

	auto suzanneObject = m_scene->CreateObject("Suzanne");
	suzanneObject->AddComponent(new eng::MeshComponent(suzanneMaterial, suzanneMesh));
	suzanneObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto light = m_scene->CreateObject("Light");
    auto lightComp = new eng::LightComponent();
    lightComp->SetColor(glm::vec3(2.0f, 1.0f, 3.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));


	//Trying to load a ninja model, but it doesn't work for some reason. I think it's because the ninja model is too complex and has too many vertices. I will try to load a simpler model instead.
    /*auto ninjaMesh = eng::Mesh::Load("models/Ninja.gltf");
    auto ninjaMaterial = eng::Material::Load("materials/ninja.mat");

    auto ninjaObject = m_scene->CreateObject("Ninja");
    ninjaObject->AddComponent(new eng::MeshComponent(ninjaMaterial,ninjaMesh));
    ninjaObject->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));*/

	eng::Engine::GetInstance().SetScene(m_scene);

	return true;
}

void Game::Update(float deltaTime)
{
	m_scene->Update(deltaTime);
}

void Game::Destroy()
{

}