#pragma once
#include <vector>

namespace eng
{
	class Material;
	class Mesh;
	class GraphicsAPI;

	struct RenderCommand
	{
		Mesh* mesh = nullptr;
		Material* material = nullptr;
	};

	class RenderQueue
	{
	public:
		void Submit(const RenderCommand command);
		void Draw(GraphicsAPI& graphicsAPI);

	private:
		std::vector<RenderCommand> m_commands;
	};
}