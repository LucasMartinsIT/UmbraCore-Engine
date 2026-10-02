#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()

namespace eng {

    // O que cada bloco da nossa grelha representa
    enum class CellType {
        Wall = 0,
        Floor = 1,
        Interactable = 2 // Reservado para o seu Baú / Saída
    };

    class ProceduralGenerator {
    public:
        ProceduralGenerator(int width, int height);

        // O método principal que vai orquestrar as fases da geração
        void GenerateMap();

        // Leitura da grelha para a engine saber o que instanciar
        CellType GetCell(int x, int y) const;
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        void ClearGrid();
        void CarveGoldenPath();

        // Espaço para futuras ramificações
        void CarveBranches(int numBranches, int maxLength);

        int m_width;
        int m_height;
        std::vector<std::vector<CellType>> m_grid;

        glm::ivec2 m_startPos;
        glm::ivec2 m_endPos;
    };
}