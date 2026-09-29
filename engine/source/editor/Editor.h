#pragma once
#include "../scene/Scene.h"
#include "../scene/GameObject.h"

namespace eng {
    class Editor {
    private:
        GameObject* m_selectedObject = nullptr;

    public:
        void Render(Scene* currentScene);
    };
}