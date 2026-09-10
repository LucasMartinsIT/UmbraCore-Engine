#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace eng
{
	class KinematicCharacterController
	{
	public:
		KinematicCharacterController(float radius, float height);
		~KinematicCharacterController();

		glm::vec3 GetPosition() const;
		glm::quat GetRotation() const;

		void Walk(const glm::vec3& direction);
		void Jump(const glm::vec3& direction);
		bool OnGround() const;

	private:
		float m_radius = 0.4f;
		float m_height = 1.2f;

		std::unique_ptr<btPairCachingGhostObject> m_ghostObj;
		std::unique_ptr<btKinematicCharacterController> m_controller;
	};
}