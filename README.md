# 🌑 UmbraCore Engine

[![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-5586A4.svg?style=flat&logo=opengl)](https://www.opengl.org/)
[![ImGui](https://img.shields.io/badge/UI-Dear_ImGui-red.svg?style=flat)](https://github.com/ocornut/imgui)
[![Bullet Physics](https://img.shields.io/badge/Physics-Bullet-green.svg?style=flat)](https://github.com/bulletphysics/bullet3)

**UmbraCore** is a custom 3D game engine built from scratch using C++ and OpenGL. Developed as a Computer Engineering capstone project (TCC) at Centro Universitário Facens, it features a modular Entity-Component System (ECS), a fully integrated real-time Editor, and a robust procedural dungeon generation system.

---

## 📸 Showcase

> **Note:** Watch the engine in action below.

<img width="400" height="225" alt="Video Project 4" src="https://github.com/user-attachments/assets/9c0999f6-3efe-4b3b-b132-b219afb96083" />

<img width="400" height="225" alt="ezgif-461ca3afa993d7ee" src="https://github.com/user-attachments/assets/6fcdc661-3ddc-44c0-b8c0-daefa9bc33f3" />

---

## ✨ Key Features

*   🖥️ **Integrated ImGui Editor:** A complete suite of developer tools including a Scene Hierarchy, Component Inspector, and Real-time parameter tweaking without recompiling.
*   🧩 **Entity-Component System (ECS):** Modular architecture allowing dynamic attachment of components (`MeshComponent`, `PhysicsComponent`, `LightComponent`, `AudioComponent`).
*   🎲 **Procedural Generation System:** Built-in "Golden Path" algorithm for dungeon generation. Automatically carves guaranteed critical paths, dead-end branches, and expansive arenas with physics-ready modular instantiation.
*   ⚙️ **Physics Integration:** Powered by Bullet Physics. Supports Static, Dynamic, and Kinematic RigidBodies with Box, Sphere, and Capsule colliders mapped directly to the visual meshes.
*   🎨 **Material & Asset Workflow:** Unity-inspired `.mat` JSON files for seamless material loading, texture mapping, and shader property injection.
*   💾 **Scene Serialization:** Load and manage complete 3D environments via external JSON scene files.

---

## 🛠️ Tech Stack & Dependencies

UmbraCore leverages industry-standard open-source libraries to power its core systems:

| Core Subsystem | Technology / Library |
| :--- | :--- |
| **Language** | C++17 |
| **Graphics API** | OpenGL 3.3 |
| **Windowing & Input** | GLFW & GLAD |
| **Mathematics** | GLM (OpenGL Mathematics) |
| **User Interface** | Dear ImGui |
| **Physics Engine** | Bullet Physics |
| **Model Loading** | Assimp (GLTF/OBJ support) |
| **Data Serialization** | nlohmann/json |

---

## 🏗️ Architecture Overview

UmbraCore is designed with separation of concerns in mind. The rendering pipeline is completely decoupled from the game logic and procedural generation algorithms. 

### The Material Workflow
Materials are handled via external `.mat` files, allowing artists and developers to assign textures and shader properties without touching C++ code:

```json
{
    "shader": {
        "vertex": "shaders/vertex.glsl",
        "fragment": "shaders/fragment.glsl"
    },
    "params": {
        "float3": [ { "name": "color", "value0": 1.0, "value1": 1.0, "value2": 1.0 } ],
        "textures": [ { "name": "baseColorTexture", "path": "textures/wall.png" } ]
    }
}
