#include "scene/components/PlayerControllerComponent.h"
#include "input/InputManager.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

namespace eng
{
	void PlayerControllerComponent::Update(float deltaTime)
	{
		auto& inputManager = Engine::GetInstance().GetInputManager();
		auto rotation = m_owner->GetRotation();

		if (inputManager.IsMousePositionChanged())
		{
			const auto& oldPos = inputManager.GetMousePositionOld();
			const auto& currentPos = inputManager.GetMousePositionCurrent();

			float deltaX = currentPos.x - oldPos.x;
			float deltaY = currentPos.y - oldPos.y;

			//Rot around Y axis
			float yDeltaAngle = -deltaX * m_sensitivity * deltaTime;
			m_yRot += yDeltaAngle;
			glm::quat yRot = glm::angleAxis(glm::radians(m_yRot), glm::vec3(0.0f, 1.0f, 0.0f));

			//Rot around X axis
			float xDeltaAngle = -deltaY * m_sensitivity * deltaTime;
			m_xRot += xDeltaAngle;
			m_xRot = std::clamp(m_xRot, - 89.0f, 89.0f);
			glm::quat xRot = glm::angleAxis(glm::radians(m_xRot), glm::vec3(1.0f, 0.0f, 0.0f));

			rotation = glm::normalize(yRot * xRot);

			m_owner->SetRotation(rotation);
		}
		
		glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		
		auto position = m_owner->GetPosition();

		//Left/Right movement
		if (inputManager.IsKeyPressed(GLFW_KEY_A))
		{
			position -= right * m_moveSpeed * deltaTime;
		}
		else if (inputManager.IsKeyPressed(GLFW_KEY_D))
		{
			position += right * m_moveSpeed * deltaTime;
		}
		//Vertical move
		if (inputManager.IsKeyPressed(GLFW_KEY_S))
		{
			position -= front * m_moveSpeed * deltaTime;
		}
		else if (inputManager.IsKeyPressed(GLFW_KEY_W))
		{
			position += front * m_moveSpeed * deltaTime;
		}
		m_owner->SetPosition(position);
	
	}
}

