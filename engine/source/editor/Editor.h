#pragma once
#include "../scene/Scene.h"
#include "../scene/GameObject.h"

namespace eng {
    class Editor {
    private:
        GameObject* m_selectedObject = nullptr;
        // Estado temporário da UI para os menus suspensos
        int m_currentMeshIdx = 0;
        int m_currentColliderIdx = 0;

    public:
        void Render(Scene* currentScene);
    };
}