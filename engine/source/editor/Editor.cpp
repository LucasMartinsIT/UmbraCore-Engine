#include "Editor.h"
#include "Engine.h"
#include <imgui.h>

#include "../scene/components/LightComponent.h"
#include "../scene/components/MeshComponent.h"
#include "../scene/components/PhysicsComponent.h"
#include "../scene/components/CameraComponent.h"
#include "../scene/components/PlayerControllerComponent.h"
#include "../render/Mesh.h"
#include "../physics/RigidBody.h"
#include "../physics/Collider.h"
#include "../render/Material.h"
#include "../scene/ProceduralGenerator.h"
#include "../graphics/Texture.h"

namespace eng {
    void Editor::Render(Scene* currentScene) {
        if (!currentScene) return;

        // --- SAFETY SYSTEM (Crash Prevention) ---
        // Checks if the selected object still exists in the scene memory
        if (m_selectedObject != nullptr) {
            bool objectStillExists = false;
            for (const auto& objPtr : currentScene->GetObjects()) {
                if (objPtr.get() == m_selectedObject) {
                    objectStillExists = true;
                    break;
                }
            }
            // If the object was destroyed by the game, clear the selection
            if (!objectStillExists) {
                m_selectedObject = nullptr;
            }
        }

        // --- HIERARCHY PANEL ---
        ImGui::Begin("Hierarchy");

        if (ImGui::Button("+ New GameObject")) {
            m_selectedObject = currentScene->CreateObject("New Object");
            m_currentMeshIdx = 0;
            m_currentColliderIdx = 0;
        }

        ImGui::Separator();

        for (const auto& objPtr : currentScene->GetObjects()) {
            GameObject* obj = objPtr.get();
            bool isSelected = (m_selectedObject == obj);

            if (ImGui::Selectable(obj->GetName().c_str(), isSelected)) {
                // If the user clicked on a different object, reset the menus
                if (m_selectedObject != obj) {
                    m_selectedObject = obj;
                    m_currentMeshIdx = 0;
                    m_currentColliderIdx = 0;
                }
            }
        }
        ImGui::End();

        // --- INSPECTOR PANEL ---
        ImGui::Begin("Inspector");
        if (m_selectedObject != nullptr) {

            // 1. BASE TRANSFORM
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                char nameBuf[128];
                strncpy(nameBuf, m_selectedObject->GetName().c_str(), sizeof(nameBuf));
                nameBuf[sizeof(nameBuf) - 1] = '\0';

                if (ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf))) {
                    m_selectedObject->SetName(nameBuf);
                }
                ImGui::Separator();

                glm::vec3 pos = m_selectedObject->GetPosition();
                if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
                    m_selectedObject->SetPosition(pos);

                    // SYNCHRONIZATION: Update Bullet Physics position in real-time!
                    if (auto phys = m_selectedObject->GetComponent<PhysicsComponent>()) {
                        if (auto rb = phys->GetRigidBody()) {
                            rb->SetPosition(pos);
                        }
                    }
                }

                // SCALE
                glm::vec3 scale = m_selectedObject->GetScale();
                if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) {
                    m_selectedObject->SetScale(scale);
                }
            }

            // 2. CURRENT COMPONENTS
            if (auto light = m_selectedObject->GetComponent<LightComponent>()) {
                if (ImGui::CollapsingHeader("Light Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                    glm::vec3 color = light->GetColor();
                    if (ImGui::ColorEdit3("Light Color", &color.x)) {
                        light->SetColor(color);
                    }
                }
            }

            // --- MESH COMPONENT ---
            if (auto meshComp = m_selectedObject->GetComponent<MeshComponent>()) {
                if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen)) {

                    const char* meshTypes[] = { "Select...", "Cube", "Sphere" };
                    if (ImGui::Combo("Shape", &m_currentMeshIdx, meshTypes, IM_ARRAYSIZE(meshTypes))) {
                        if (m_currentMeshIdx == 1) meshComp->SetMesh(Mesh::CreateBox());
                        else if (m_currentMeshIdx == 2) meshComp->SetMesh(Mesh::CreateSphere(1.0f, 36, 18));
                    }
                }

                // --- MATERIAL COMPONENT (Separate Tab) ---
                if (auto material = meshComp->GetMaterial()) {
                    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {

                        // 1. LOAD NEW MATERIALS (.mat)
                        ImGui::Text("Material Asset:");

                        // Static buffer keeps the typed text alive between UI frames
                        static char matPathBuf[128] = "materials/checker.mat";
                        ImGui::InputText("Path", matPathBuf, IM_ARRAYSIZE(matPathBuf));

                        if (ImGui::Button("Assign Material", ImVec2(-1, 30))) {
                            // Tries to load the new .mat file (e.g., wall.mat, floor.mat)
                            auto newMaterial = Material::Load(matPathBuf);

                            if (newMaterial) {
                                // If the file exists, replace the old mesh material with this one
                                meshComp->SetMaterial(newMaterial);
                            }
                            else {
                                // Visual warning in the console if the file is not found
                                printf("[Editor] Error: Could not load material at %s\n", matPathBuf);
                            }
                        }

                        ImGui::Separator();
                        ImGui::Text("Material Properties:");

                        // 2. EDIT CURRENT MATERIAL PROPERTIES (e.g., Base Color)
                        glm::vec3 color = glm::vec3(1.0f);
                        if (material->HasFloat3Param("color")) {
                            color = material->GetFloat3Param("color");
                        }

                        if (ImGui::ColorEdit3("Base Color", &color.x)) {
                            material->SetParam("color", color);
                        }
                    }
                }
            }

            // --- PHYSICS COMPONENT ---
            if (auto physics = m_selectedObject->GetComponent<PhysicsComponent>()) {
                if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen)) {

                    if (auto rb = physics->GetRigidBody()) {

                        // 1. BODY TYPE (Static, Dynamic, Kinematic)
                        const char* bodyTypes[] = { "Static", "Dynamic", "Kinematic" };
                        int currentTypeIdx = static_cast<int>(rb->GetType());

                        if (ImGui::Combo("Body Type", &currentTypeIdx, bodyTypes, IM_ARRAYSIZE(bodyTypes))) {
                            rb->SetType(static_cast<BodyType>(currentTypeIdx));
                        }

                        // 2. MASS AND FRICTION
                        float mass = rb->GetMass();
                        if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 1000.0f)) {
                            rb->SetMass(mass);
                        }

                        float friction = rb->GetFriction();
                        if (ImGui::DragFloat("Friction", &friction, 0.05f, 0.0f, 1.0f)) {
                            rb->SetFriction(friction);
                        }

                        ImGui::Separator();

                        // 3. COLLIDER SHAPE
                        const char* colliderTypes[] = { "Box", "Sphere", "Capsule" };
                        int currentColliderIdx = -1;

                        if (auto collider = rb->GetCollider()) {
                            if (dynamic_cast<BoxCollider*>(collider.get())) currentColliderIdx = 0;
                            else if (dynamic_cast<SphereCollider*>(collider.get())) currentColliderIdx = 1;
                            else if (dynamic_cast<CapsuleCollider*>(collider.get())) currentColliderIdx = 2;
                        }

                        if (currentColliderIdx != -1) {
                            if (ImGui::Combo("Collider", &currentColliderIdx, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
                                std::shared_ptr<Collider> newCollider = nullptr;
                                if (currentColliderIdx == 0) newCollider = std::make_shared<BoxCollider>(glm::vec3(1.0f));
                                else if (currentColliderIdx == 1) newCollider = std::make_shared<SphereCollider>(1.0f);
                                else if (currentColliderIdx == 2) newCollider = std::make_shared<CapsuleCollider>(0.5f, 2.0f);

                                if (newCollider) rb->SetCollider(newCollider);
                            }
                        }
                    }
                    else {
                        ImGui::Text("RigidBody not initialized.");
                    }
                }
            }

            ImGui::Separator();

            // 3. MENU TO ADD NEW COMPONENTS
            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponentMenu");
            }

            if (ImGui::BeginPopup("AddComponentMenu")) {

                if (ImGui::MenuItem("Light Component")) {
                    if (!m_selectedObject->GetComponent<LightComponent>()) m_selectedObject->AddComponent(new LightComponent());
                }

                if (ImGui::MenuItem("Mesh Component")) {
                    if (!m_selectedObject->GetComponent<MeshComponent>()) {
                        auto meshComp = new MeshComponent();

                        // 1. Inject a Cube mesh by default
                        meshComp->SetMesh(Mesh::CreateBox());

                        // 2. Load the base material exactly as in scene.json
                        auto defaultMaterial = Material::Load("materials/checker.mat");
                        meshComp->SetMaterial(defaultMaterial);

                        m_selectedObject->AddComponent(meshComp);
                    }
                }

                if (ImGui::MenuItem("Physics Component")) {
                    if (!m_selectedObject->GetComponent<PhysicsComponent>()) {
                        auto physComp = new PhysicsComponent();

                        auto defaultCollider = std::make_shared<BoxCollider>(glm::vec3(1.0f));
                        auto defaultRb = std::make_shared<RigidBody>(BodyType::Static, defaultCollider, 0.0f, 0.5f);

                        // Ensure the collider spawns exactly where the visual object is
                        defaultRb->SetPosition(m_selectedObject->GetPosition());

                        physComp->SetRigidBody(defaultRb);
                        m_selectedObject->AddComponent(physComp);
                    }
                }

                if (ImGui::MenuItem("Camera Component")) {
                    if (!m_selectedObject->GetComponent<CameraComponent>()) m_selectedObject->AddComponent(new CameraComponent());
                }

                ImGui::EndPopup();
            }

        }
        else {
            ImGui::Text("Select an object in the Hierarchy.");
        }

        ImGui::End();

        // --- PROCEDURAL GENERATOR PANEL ---
        ImGui::Begin("Procedural Generator");

        // Static variables keep their values between UI frames
        static int mapWidth = 10;
        static int mapHeight = 10;
        static float tileSize = 2.0f; // Size of each block (e.g., 2x2x2)
        static glm::vec3 spawnOffset = glm::vec3(0.0f, 5.0f, -20.0f); // Default offset to avoid scene collision
        static GameObject* templateWall = nullptr;
        static GameObject* templateFloor = nullptr;

        ImGui::SliderInt("Width", &mapWidth, 5, 50);
        ImGui::SliderInt("Height", &mapHeight, 5, 50);
        ImGui::DragFloat("Tile Size", &tileSize, 0.1f);
        ImGui::DragFloat3("Spawn Offset", &spawnOffset.x, 0.5f);
        ImGui::Separator();

        // A quick Lambda function to draw GameObject selection dropdowns
        auto DrawTemplateCombo = [&](const char* label, GameObject*& selectedObj) {
            std::string preview = selectedObj ? selectedObj->GetName() : "None (Select in Scene)";
            if (ImGui::BeginCombo(label, preview.c_str())) {
                for (const auto& objPtr : currentScene->GetObjects()) {
                    if (ImGui::Selectable(objPtr->GetName().c_str(), selectedObj == objPtr.get())) {
                        selectedObj = objPtr.get();
                    }
                }
                ImGui::EndCombo();
            }
            };

        DrawTemplateCombo("Template: Wall", templateWall);
        DrawTemplateCombo("Template: Floor", templateFloor);

        ImGui::Separator();

        // The button only does something if both templates are set
        if (ImGui::Button("GENERATE MAP", ImVec2(-1, 30))) {
            if (templateWall && templateFloor) {

                // 1. Run the Math
                ProceduralGenerator generator(mapWidth, mapHeight);
                generator.GenerateMap();

                // 2. Translate Math into 3D
                for (int x = 0; x < generator.GetWidth(); ++x) {
                    for (int y = 0; y < generator.GetHeight(); ++y) {

                        CellType type = generator.GetCell(x, y);
                        GameObject* sourceTemplate = nullptr;

                        if (type == CellType::Wall) sourceTemplate = templateWall;
                        else if (type == CellType::Floor) sourceTemplate = templateFloor;

                        if (sourceTemplate) {
                            // Create the clone
                            GameObject* newObj = currentScene->CreateObject(sourceTemplate->GetName() + "_Clone");

                            // Adjust Y position based on whether it is a wall or floor to prevent clipping
                            float heightOffset = 0.0f;
                            if (type == CellType::Wall) {
                                heightOffset = sourceTemplate->GetScale().y * 0.5f; // Raise walls above the floor
                            }

                            // Position the block applying the UI offset
                            glm::vec3 spawnPos = spawnOffset + glm::vec3(x * tileSize, heightOffset, y * tileSize);
                            newObj->SetPosition(spawnPos);
                            newObj->SetScale(sourceTemplate->GetScale());

                            // Clone the Visuals
                            if (auto sourceMesh = sourceTemplate->GetComponent<MeshComponent>()) {
                                auto newMeshComp = new MeshComponent();
                                newMeshComp->SetMesh(sourceMesh->GetMesh());
                                newMeshComp->SetMaterial(sourceMesh->GetMaterial());
                                newObj->AddComponent(newMeshComp);
                            }

                            // Clone the Physics (Ensures the wall cannot be passed through)
                            if (auto sourcePhys = sourceTemplate->GetComponent<PhysicsComponent>()) {
                                auto newPhysComp = new PhysicsComponent();

                                auto collider = std::make_shared<BoxCollider>(sourceTemplate->GetScale());

                                auto rb = std::make_shared<RigidBody>(BodyType::Static, collider, 0.0f, 0.5f);
                                rb->SetPosition(newObj->GetPosition());
                                newPhysComp->SetRigidBody(rb);
                                newObj->AddComponent(newPhysComp);
                            }
                        }
                    }
                }
            }
        }

        if (!templateWall || !templateFloor) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Warning: Set the templates first!");
        }

        ImGui::Separator();
        ImGui::Text("Utilities");

        // Botão para teletransportar o objeto selecionado para o início do labirinto
        if (ImGui::Button("Teleport Selected to Maze Start", ImVec2(-1, 30))) {
            if (m_selectedObject) {
                // Adicionamos +5.0f no eixo Y para o jogador cair suavemente no chão gerado
                glm::vec3 mazeStartPos = spawnOffset + glm::vec3(1.0f * tileSize, 5.0f, 1.0f * tileSize);

                m_selectedObject->SetPosition(mazeStartPos);

                if (auto phys = m_selectedObject->GetComponent<PhysicsComponent>()) {
                    if (auto rb = phys->GetRigidBody()) {
                        rb->SetPosition(mazeStartPos);

                        // Truque: Mudar para Kinematic temporariamente zera a inércia de queda
                        BodyType currentType = rb->GetType();
                        rb->SetType(BodyType::Kinematic);
                        rb->SetType(currentType);
                    }
                }
            }
        }

        ImGui::End();
    }
}