#include "ProceduralGenerator.h"

namespace eng {

    ProceduralGenerator::ProceduralGenerator(int width, int height)
        : m_width(width), m_height(height)
    {
        // Inicializa a semente aleatória com o tempo atual do PC
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Define o início num canto e o fim no canto oposto (deixando bordas)
        m_startPos = glm::ivec2(1, 1);
        m_endPos = glm::ivec2(m_width - 2, m_height - 2);

        // Aloca a memória da grelha
        m_grid.resize(m_width, std::vector<CellType>(m_height, CellType::Wall));
    }

    void ProceduralGenerator::GenerateMap()
    {
        ClearGrid();         // 1. Enche tudo de paredes
        CarveGoldenPath();   // 2. Escava o caminho principal garantido

        // 3. NOVO: Lança 20 ramificações (becos), cada uma com até 15 blocos de profundidade
        // (No futuro, você pode expor estes números no ImGui!)
        CarveBranches(20, 15);

        // 4. Marca o fim do mapa
        m_grid[m_endPos.x][m_endPos.y] = CellType::Interactable;
    }

    void ProceduralGenerator::ClearGrid()
    {
        for (int x = 0; x < m_width; ++x) {
            for (int y = 0; y < m_height; ++y) {
                m_grid[x][y] = CellType::Wall;
            }
        }
    }

    void ProceduralGenerator::CarveGoldenPath()
    {
        glm::ivec2 current = m_startPos;
        m_grid[current.x][current.y] = CellType::Floor;

        // Enquanto o caminhante não chegar ao destino
        while (current != m_endPos)
        {
            // Lança uma moeda: 0 = tenta mover no X, 1 = tenta mover no Y
            bool moveX = (std::rand() % 2 == 0);

            if (moveX && current.x != m_endPos.x) {
                // Move-se em direção ao X final
                current.x += (m_endPos.x > current.x) ? 1 : -1;
            }
            else if (current.y != m_endPos.y) {
                // Move-se em direção ao Y final se falhou o X
                current.y += (m_endPos.y > current.y) ? 1 : -1;
            }
            else {
                // Se já estiver alinhado no eixo escolhido, força o movimento no outro
                current.x += (m_endPos.x > current.x) ? 1 : -1;
            }

            // Escava o chão nesta nova coordenada
            m_grid[current.x][current.y] = CellType::Floor;
        }
    }

    CellType ProceduralGenerator::GetCell(int x, int y) const
    {
        // Proteção contra leitura fora da memória
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            return m_grid[x][y];
        }
        return CellType::Wall;
    }

    void ProceduralGenerator::CarveBranches(int numBranches, int maxLength)
    {
        int branchesCreated = 0;
        int attempts = 0; // Prevenção contra loop infinito

        // Tenta criar o número desejado de ramificações
        while (branchesCreated < numBranches && attempts < 1000)
        {
            attempts++;

            // Escolhe uma coordenada aleatória (ignorando a borda extrema)
            int startX = 1 + std::rand() % (m_width - 2);
            int startY = 1 + std::rand() % (m_height - 2);

            // Só inicia uma ramificação se caiu num bloco que JÁ É chão
            if (m_grid[startX][startY] == CellType::Floor)
            {
                glm::ivec2 current(startX, startY);

                // O escavador secundário dá 'maxLength' passos
                for (int step = 0; step < maxLength; ++step)
                {
                    // Sorteia uma direção: 0=Cima, 1=Baixo, 2=Esquerda, 3=Direita
                    int dir = std::rand() % 4;
                    if (dir == 0) current.y += 1;
                    else if (dir == 1) current.y -= 1;
                    else if (dir == 2) current.x -= 1;
                    else if (dir == 3) current.x += 1;

                    // Verifica se bateu nas bordas do mapa (mantém um anel de parede em volta de tudo)
                    if (current.x <= 0 || current.x >= m_width - 1 || current.y <= 0 || current.y >= m_height - 1) {
                        break; // Aborta esta ramificação e tenta outra
                    }

                    // Escava o novo chão
                    m_grid[current.x][current.y] = CellType::Floor;
                }
                branchesCreated++;
            }
        }
    }
}