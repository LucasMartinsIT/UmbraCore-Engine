#include "Game.h"
#include "TestObject.h"
#include <iostream>
#include <random>

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

    std::vector<float> vertices =
    {
        //Front face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //Top face 
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //Right face
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //Bottom face
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //Back face
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices =
    {
        //Front face
        0, 1, 2,
        0, 2, 3,
        //Top face
        4, 5, 6,
        4, 6, 7,
        //Right face
        8, 9, 10,
        8, 10, 11,
        //Left face
        12, 13, 14,
        12, 14, 15,
        //Bottom face
        16, 17, 18,
        16, 18, 19,
        //Back face
        20, 21, 22,
        20, 22, 23
    };

    eng::VertexLayout vertexLayout;

    //Position
    vertexLayout.elements.push_back({
        0,
        3,
        GL_FLOAT,
        0
        });
    //Color
    vertexLayout.elements.push_back({
        1,
        3,
        GL_FLOAT,
        sizeof(float) * 3
        });
    //UV
    vertexLayout.elements.push_back({
        2,
        2,
        GL_FLOAT,
        sizeof(float) * 6
        });
    vertexLayout.stride = sizeof(float) * 8;

    auto mesh = std::make_shared<eng::Mesh>(vertexLayout, vertices, indices);

    //CRIA O CHÃO DE BLOCOS
    int gridSize = 40; //40x40 blocos. 
    float blockSize = 1.0f; //tamanho de cada cubo do chão
    float offset = (gridSize * blockSize) / 2.0f; //Usado para centralizar o grid no 0,0

    for (int x = 0; x < gridSize; ++x)
    {
        for (int z = 0; z < gridSize; ++z)
        {
            auto block = m_scene->CreateObject("Ground_" + std::to_string(x) + "_" + std::to_string(z));
            block->AddComponent(new eng::MeshComponent(material, mesh));

            //Calcula a posição no grid
            float posX = (x * blockSize) - offset;
            float posZ = (z * blockSize) - offset;

            //Coloca no Y = -1.0f para ficar abaixo dos seus pés
            block->SetPosition(glm::vec3(posX, -1.0f, posZ));
            block->SetScale(glm::vec3(blockSize, blockSize, blockSize));

            m_groundBlocks.push_back(block);
        }
    }


    //ESPALHA OS OBSTÁCULOS COM FORMATOS ALEATÓRIOS
    int poolSize = 60;

    std::random_device rd;
    std::mt19937 gen(rd());

    //Distância menor para nascerem mais perto da câmera
    std::uniform_real_distribution<float> randomDist(-20.0f, 20.0f);

    //Escalas randomicas
    std::uniform_real_distribution<float> randScaleX(0.2f, 2.5f);
    std::uniform_real_distribution<float> randScaleY(0.5f, 4.0f); 
    std::uniform_real_distribution<float> randScaleZ(0.2f, 2.5f);

    //Rotação de 0 a 2*PI para 360 graus radiando
    std::uniform_real_distribution<float> randRot(0.0f, 6.28318f);

    for (int i = 0; i < poolSize; ++i)
    {
        auto obj = m_scene->CreateObject("Obstacle_" + std::to_string(i));
        obj->AddComponent(new eng::MeshComponent(material, mesh));

        //Sorteia os tamanhos
        float sX = randScaleX(gen);
        float sY = randScaleY(gen);
        float sZ = randScaleZ(gen);
        obj->SetScale(glm::vec3(sX, sY, sZ));

        //Sorteia a rotação nos 3 eixos para ficar bem maluco
        float rX = randRot(gen);
        float rY = randRot(gen);
        float rZ = randRot(gen);
        //O GLM constrói um Quaternion a partir de um vec3 em radianos
        obj->SetRotation(glm::quat(glm::vec3(rX, rY, rZ)));

        //MATEMÁTICA PARA FICAR EM CIMA DO CHÃO:
        //O topo do chão está no Y = -0.5f. 
        //Soma a metade da altura (sY / 2.0f) para o objeto "sentar" perfeitamente no chão.
        float posY = -0.5f + (sY / 2.0f);

        obj->SetPosition(glm::vec3(randomDist(gen), posY, randomDist(gen)));

        m_mapObjects.push_back(obj);
    }


	eng::Engine::GetInstance().SetScene(m_scene);

	return true;
}

void Game::Update(float deltaTime)
{
	m_scene->Update(deltaTime);

    auto camera = m_scene->GetMainCamera();
    if (!camera) return;

    glm::vec3 camPos = camera->GetPosition();


    //ESTEIRA DO CHÃO
    float blockSize = 1.0f;
    int gridSize = 40; //MESMO VALOR DO INIT
    float halfGridSize = (gridSize * blockSize) / 2.0f;

    for (auto block : m_groundBlocks)
    {
        glm::vec3 pos = block->GetPosition();

        //Calcula a distância do bloco para a câmera
        float distX = pos.x - camPos.x;
        float distZ = pos.z - camPos.z;

        //Se o bloco ficou muito para trás no X, joga para a frente (e vice-versa)
        if (distX < -halfGridSize) pos.x += gridSize * blockSize;
        else if (distX > halfGridSize) pos.x -= gridSize * blockSize;

        //Se o bloco ficou muito para trás no Z, joga para a frente (e vice-versa)
        if (distZ < -halfGridSize) pos.z += gridSize * blockSize;
        else if (distZ > halfGridSize) pos.z -= gridSize * blockSize;

        block->SetPosition(pos);
    }


    //RECICLAGEM DOS OBSTÁCULOS
    glm::quat camRot = camera->GetRotation();
    glm::vec3 camFront = camRot * glm::vec3(0.0f, 0.0f, -1.0f);
    camFront.y = 0.0f;
    camFront = glm::normalize(camFront);

    glm::vec3 camRight = camRot * glm::vec3(1.0f, 0.0f, 0.0f);
    camRight.y = 0.0f;
    camRight = glm::normalize(camRight);

    std::random_device rd;
    std::mt19937 gen(rd());

    // Valores ajustados para a reciclagem acontecer mais perto da câmera
    std::uniform_real_distribution<float> spread(-15.0f, 15.0f); // Menos espalhado pros lados
    std::uniform_real_distribution<float> depth(-5.0f, 5.0f);

    // Os mesmos geradores de formato do Init
    std::uniform_real_distribution<float> randScaleX(0.2f, 2.5f);
    std::uniform_real_distribution<float> randScaleY(0.5f, 4.0f);
    std::uniform_real_distribution<float> randScaleZ(0.2f, 2.5f);
    std::uniform_real_distribution<float> randRot(0.0f, 6.28318f);

    for (auto obj : m_mapObjects)
    {
        glm::vec3 objPos = obj->GetPosition();
        glm::vec3 dirToObj = objPos - camPos;
        float distance = glm::length(dirToObj);

        if (distance > 0.1f)
        {
            float dotProduct = glm::dot(camFront, glm::normalize(dirToObj));

            //Diminui a distância máxima de 60 para 30 para sumirem mais rápido
            if (distance > 30.0f || (dotProduct < -0.2f && distance > 5.0f))
            {
                //NASCE COMO UM NOVO OBJETO
                float sX = randScaleX(gen);
                float sY = randScaleY(gen);
                float sZ = randScaleZ(gen);
                obj->SetScale(glm::vec3(sX, sY, sZ));

                obj->SetRotation(glm::quat(glm::vec3(randRot(gen), randRot(gen), randRot(gen))));

                //Teletransporte para mais perto da câmera 25 metros na frente
                float spawnDist = 25.0f;
                glm::vec3 newPos = camPos + (camFront * spawnDist) + (camRight * spread(gen)) + (camFront * depth(gen));

                //Aplica a mesma matemática para sentar no chão com a nova escala
                newPos.y = -0.5f + (sY / 2.0f);

                obj->SetPosition(newPos);
            }
        }
    }
}

void Game::Destroy()
{

}