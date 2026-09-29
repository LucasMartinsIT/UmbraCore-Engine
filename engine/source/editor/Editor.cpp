#include "Editor.h"
#include <imgui.h> // Usando <> para o CMake achar na pasta thirdparty

namespace eng {
    void Editor::Render(Scene* currentScene) {
        // --- PAINEL: HIERARQUIA ---
        ImGui::Begin("Hierarquia");

        if (!currentScene) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "NENHUMA CENA ATIVA!");
            ImGui::End();
        }
        else {
            if (ImGui::Button("+ Novo GameObject")) {
                m_selectedObject = currentScene->CreateObject("Novo Objeto");
            }
            ImGui::Separator();

            for (const auto& objPtr : currentScene->GetObjects()) {
                GameObject* obj = objPtr.get();
                bool isSelected = (m_selectedObject == obj);

                if (ImGui::Selectable(obj->GetName().c_str(), isSelected)) {
                    m_selectedObject = obj;
                }
            }
            ImGui::End();
        }

        // --- PAINEL: INSPETOR ---
        ImGui::Begin("Inspetor");
        if (m_selectedObject != nullptr && currentScene != nullptr) {

            char nameBuf[128];
            strncpy(nameBuf, m_selectedObject->GetName().c_str(), sizeof(nameBuf));
            nameBuf[sizeof(nameBuf) - 1] = '\0';

            if (ImGui::InputText("Nome", nameBuf, IM_ARRAYSIZE(nameBuf))) {
                m_selectedObject->SetName(nameBuf);
            }
            ImGui::Separator();

            glm::vec3 pos = m_selectedObject->GetPosition();
            if (ImGui::DragFloat3("Posicao", &pos.x, 0.1f)) m_selectedObject->SetPosition(pos);

            glm::vec3 scale = m_selectedObject->GetScale();
            if (ImGui::DragFloat3("Escala", &scale.x, 0.1f)) m_selectedObject->SetScale(scale);

        }
        else {
            ImGui::Text("Selecione um objeto na Hierarquia.");
        }
        ImGui::End();
    }
}