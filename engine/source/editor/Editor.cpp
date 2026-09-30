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

namespace eng {
    void Editor::Render(Scene* currentScene) {
        if (!currentScene) return;

        // --- SISTEMA DE SEGURANÇA (Prevenção de Crash) ---
        // Verifica se o objeto selecionado ainda existe na memória da cena
        if (m_selectedObject != nullptr) {
            bool objectStillExists = false;
            for (const auto& objPtr : currentScene->GetObjects()) {
                if (objPtr.get() == m_selectedObject) {
                    objectStillExists = true;
                    break;
                }
            }
            // Se o objeto foi destruído pelo jogo, limpamos a seleção
            if (!objectStillExists) {
                m_selectedObject = nullptr;
            }
        }

        // --- PAINEL: HIERARQUIA ---
        ImGui::Begin("Hierarquia");

        if (ImGui::Button("+ Novo GameObject")) {
            m_selectedObject = currentScene->CreateObject("Novo Objeto");
            m_currentMeshIdx = 0;
            m_currentColliderIdx = 0;
        }

        ImGui::Separator();

        for (const auto& objPtr : currentScene->GetObjects()) {
            GameObject* obj = objPtr.get();
            bool isSelected = (m_selectedObject == obj);

            if (ImGui::Selectable(obj->GetName().c_str(), isSelected)) {
                // Se o utilizador clicou num objeto diferente, redefinimos os menus
                if (m_selectedObject != obj) {
                    m_selectedObject = obj;
                    m_currentMeshIdx = 0;
                    m_currentColliderIdx = 0;
                }
            }
        }
        ImGui::End();

        // --- PAINEL: INSPETOR ---
        ImGui::Begin("Inspetor");
        if (m_selectedObject != nullptr) {

            // 1. TRANSFORM BASE
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                char nameBuf[128];
                strncpy(nameBuf, m_selectedObject->GetName().c_str(), sizeof(nameBuf));
                nameBuf[sizeof(nameBuf) - 1] = '\0';

                if (ImGui::InputText("Nome", nameBuf, IM_ARRAYSIZE(nameBuf))) {
                    m_selectedObject->SetName(nameBuf);
                }
                ImGui::Separator();

                glm::vec3 pos = m_selectedObject->GetPosition();
                if (ImGui::DragFloat3("Posicao", &pos.x, 0.1f)) {
                    m_selectedObject->SetPosition(pos);

                    // SINCRONIZAÇÃO: Atualiza a posição da Bullet Physics em tempo real!
                    if (auto phys = m_selectedObject->GetComponent<PhysicsComponent>()) {
                        if (auto rb = phys->GetRigidBody()) {
                            rb->SetPosition(pos);
                        }
                    }
                }

                // A ESCALA DE VOLTA AQUI:
                glm::vec3 scale = m_selectedObject->GetScale();
                if (ImGui::DragFloat3("Escala", &scale.x, 0.1f)) {
                    m_selectedObject->SetScale(scale);

                    // Nota: Se quiser que a caixa de colisão da física escale junto com 
                    // o visual futuramente, precisaremos implementar um rb->SetScale()
                }
            }

            // 2. COMPONENTES ATUAIS
            if (auto light = m_selectedObject->GetComponent<LightComponent>()) {
                if (ImGui::CollapsingHeader("Light Component", ImGuiTreeNodeFlags_DefaultOpen)) {
                    glm::vec3 color = light->GetColor();
                    if (ImGui::ColorEdit3("Cor da Luz", &color.x)) {
                        light->SetColor(color);
                    }
                }
            }

            // --- MESH COMPONENT ---
            if (auto meshComp = m_selectedObject->GetComponent<MeshComponent>()) {
                if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen)) {

                    const char* meshTypes[] = { "Selecione...", "Cubo", "Esfera" };
                    if (ImGui::Combo("Formato", &m_currentMeshIdx, meshTypes, IM_ARRAYSIZE(meshTypes))) {
                        if (m_currentMeshIdx == 1) meshComp->SetMesh(Mesh::CreateBox());
                        else if (m_currentMeshIdx == 2) meshComp->SetMesh(Mesh::CreateSphere(1.0f, 36, 18));
                    }
                }

                // --- MATERIAL COMPONENT (Aba Separada) ---
                if (auto material = meshComp->GetMaterial()) {
                    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {

                        // Edição de Cor em tempo real
                        glm::vec3 color = glm::vec3(1.0f);
                        if (material->HasFloat3Param("color")) {
                            color = material->GetFloat3Param("color");
                        }

                        if (ImGui::ColorEdit3("Cor Base", &color.x)) {
                            material->SetParam("color", color);
                        }

                        ImGui::Separator();

                        // Espaço reservado para a interface das texturas
                        ImGui::Text("Texturas Ativas:");
                        ImGui::TextDisabled("baseColorTexture (Em breve)");
                    }
                }
            }

            // --- PHYSICS COMPONENT ---
            if (auto physics = m_selectedObject->GetComponent<PhysicsComponent>()) {
                if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen)) {

                    if (auto rb = physics->GetRigidBody()) {

                        // 1. TIPO DE CORPO (Static, Dynamic, Kinematic)
                        const char* bodyTypes[] = { "Static", "Dynamic", "Kinematic" };
                        int currentTypeIdx = static_cast<int>(rb->GetType());

                        if (ImGui::Combo("Tipo (Body)", &currentTypeIdx, bodyTypes, IM_ARRAYSIZE(bodyTypes))) {
                            rb->SetType(static_cast<BodyType>(currentTypeIdx));
                        }

                        // 2. MASSA E FRICÇÃO
                        float mass = rb->GetMass();
                        if (ImGui::DragFloat("Massa", &mass, 0.1f, 0.0f, 1000.0f)) {
                            rb->SetMass(mass);
                        }

                        float friction = rb->GetFriction();
                        if (ImGui::DragFloat("Friccao", &friction, 0.05f, 0.0f, 1.0f)) {
                            rb->SetFriction(friction);
                        }

                        ImGui::Separator();

                        // 3. FORMATO DO COLISOR
                        const char* colliderTypes[] = { "Box", "Sphere", "Capsule" };
                        int currentColliderIdx = -1;

                        if (auto collider = rb->GetCollider()) {
                            if (dynamic_cast<BoxCollider*>(collider.get())) currentColliderIdx = 0;
                            else if (dynamic_cast<SphereCollider*>(collider.get())) currentColliderIdx = 1;
                            else if (dynamic_cast<CapsuleCollider*>(collider.get())) currentColliderIdx = 2;
                        }

                        if (currentColliderIdx != -1) {
                            if (ImGui::Combo("Colisor", &currentColliderIdx, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
                                std::shared_ptr<Collider> newCollider = nullptr;
                                if (currentColliderIdx == 0) newCollider = std::make_shared<BoxCollider>(glm::vec3(1.0f));
                                else if (currentColliderIdx == 1) newCollider = std::make_shared<SphereCollider>(1.0f);
                                else if (currentColliderIdx == 2) newCollider = std::make_shared<CapsuleCollider>(0.5f, 2.0f);

                                if (newCollider) rb->SetCollider(newCollider);
                            }
                        }
                    }
                    else {
                        ImGui::Text("RigidBody não inicializado.");
                    }
                }
            }

            ImGui::Separator();

            // 3. MENU PARA ADICIONAR NOVOS COMPONENTES
            if (ImGui::Button("Adicionar Componente")) {
                ImGui::OpenPopup("MenuAdicionarComponente");
            }

            if (ImGui::BeginPopup("MenuAdicionarComponente")) {

                if (ImGui::MenuItem("Light Component")) {
                    if (!m_selectedObject->GetComponent<LightComponent>()) m_selectedObject->AddComponent(new LightComponent());
                }

                if (ImGui::MenuItem("Mesh Component")) {
                    if (!m_selectedObject->GetComponent<MeshComponent>()) {
                        auto meshComp = new MeshComponent();

                        // 1. Injeta a malha de um Cubo por padrão
                        meshComp->SetMesh(Mesh::CreateBox());

                        // 2. Carrega o material base exatamente como no seu scene.json
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

                        // Garante que o colisor nasce exatamente onde o objeto visual está
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
            ImGui::Text("Selecione um objeto na Hierarquia.");
        }
        ImGui::End();
    }
}