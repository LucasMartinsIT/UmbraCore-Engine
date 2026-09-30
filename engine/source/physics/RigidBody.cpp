#include "physics/RigidBody.h"
#include "Engine.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace eng
{
	RigidBody::RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction)
		: m_type(type), m_collider(collider), m_mass(mass), m_friction(friction)
	{
		if (!collider)
		{
			return;
		}

		btVector3 intertia(0, 0, 0);
		if (m_type == BodyType::Dynamic && mass > 0.0f && m_collider->GetShape())
		{
			m_collider->GetShape()->calculateLocalInertia(btScalar(mass), intertia);
		}

		btTransform transform;
		transform.setIdentity();
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);

		btRigidBody::btRigidBodyConstructionInfo info(
			(m_type == BodyType::Dynamic) ? btScalar(mass) : btScalar(0),
			motionState, m_collider->GetShape(), intertia
		);

		m_body = std::make_unique<btRigidBody>(info);
		m_body->setFriction(friction);

		if (m_type == BodyType::Kinematic)
		{
			m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			m_body->setActivationState(DISABLE_DEACTIVATION);
		}
	}

	RigidBody::~RigidBody()
	{
		if (m_addedToWorld)
		{
			Engine::GetInstance().GetPhysicsManager().RemoveRigidBody(this);
		}
	}

	btRigidBody* RigidBody::GetBody()
	{
		return m_body.get();
	}

	void RigidBody::SetAddedToWorld(bool added)
	{
		m_addedToWorld = added;
	}

	bool RigidBody::IsAddedToWorld() const
	{
		return m_addedToWorld;
	}

	BodyType RigidBody::GetType() const
	{
		return m_type;
	}

	void RigidBody::SetPosition(const glm::vec3& pos)
	{
		if (!m_body)
		{
			return;
		}

		auto& tr = m_body->getWorldTransform();
		tr.setOrigin(btVector3(btScalar(pos.x), btScalar(pos.y), btScalar(pos.z)));
		if (m_body->getMotionState())
		{
			m_body->getMotionState()->setWorldTransform(tr);
		}
		m_body->setWorldTransform(tr);
	}

	glm::vec3 RigidBody::GetPosition() const
	{
		if (!m_body)
		{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}
		const auto& pos = m_body->getWorldTransform().getOrigin();
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}

	void RigidBody::SetRotation(const glm::quat& rot)
	{
		if (!m_body)
		{
			return;
		}

		auto& tr = m_body->getWorldTransform();
		tr.setRotation(btQuaternion(btScalar(rot.x), btScalar(rot.y), btScalar(rot.z), btScalar(rot.w)));
		if (m_body->getMotionState())
		{
			m_body->getMotionState()->setWorldTransform(tr);
		}
		m_body->setWorldTransform(tr);
	}

	glm::quat RigidBody::GetRotation() const
	{
		if (!m_body)
		{
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}
		const auto& rot = m_body->getWorldTransform().getRotation();
		return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		if (!m_body)
		{
			return;
		}
		m_body->applyCentralImpulse(btVector3(
			btScalar(impulse.x), btScalar(impulse.y), btScalar(impulse.z)
		));
	}

	void RigidBody::SetMass(float mass)
	{
		if (m_mass == mass || !m_body) return;

		auto& physicsManager = Engine::GetInstance().GetPhysicsManager();
		bool wasInWorld = m_addedToWorld;

		if (wasInWorld) physicsManager.RemoveRigidBody(this);

		m_mass = mass;
		if (m_type == BodyType::Dynamic)
		{
			btVector3 inertia(0, 0, 0);
			if (mass > 0.0f && m_collider->GetShape())
			{
				m_collider->GetShape()->calculateLocalInertia(btScalar(mass), inertia);
			}
			m_body->setMassProps(btScalar(mass), inertia);
			m_body->updateInertiaTensor();
		}

		if (wasInWorld) physicsManager.AddRigidBody(this);
		m_body->activate(true);
	}

	void RigidBody::SetCollider(const std::shared_ptr<Collider>& collider)
	{
		if (!collider || !m_body) return;

		auto& physicsManager = Engine::GetInstance().GetPhysicsManager();
		bool wasInWorld = m_addedToWorld;

		if (wasInWorld) physicsManager.RemoveRigidBody(this);

		m_collider = collider;
		m_body->setCollisionShape(m_collider->GetShape());

		if (m_type == BodyType::Dynamic && m_mass > 0.0f)
		{
			btVector3 inertia(0, 0, 0);
			m_collider->GetShape()->calculateLocalInertia(btScalar(m_mass), inertia);
			m_body->setMassProps(btScalar(m_mass), inertia);
			m_body->updateInertiaTensor();
		}

		if (wasInWorld) physicsManager.AddRigidBody(this);
		m_body->activate(true);
	}

	void RigidBody::SetFriction(float friction)
	{
		m_friction = friction;
		if (m_body)
		{
			m_body->setFriction(friction);
		}
	}

	void RigidBody::SetType(BodyType type)
	{
		if (m_type == type || !m_body) return;

		auto& physicsManager = Engine::GetInstance().GetPhysicsManager();
		bool wasInWorld = m_addedToWorld;

		// 1. Remove do mundo físico temporariamente para limpar o cache da Bullet
		if (wasInWorld) physicsManager.RemoveRigidBody(this);

		m_type = type;
		int flags = m_body->getCollisionFlags();

		if (m_type == BodyType::Static) {
			flags |= btCollisionObject::CF_STATIC_OBJECT;
			flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
			m_body->setMassProps(0, btVector3(0, 0, 0));
		}
		else if (m_type == BodyType::Kinematic) {
			flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
			flags &= ~btCollisionObject::CF_STATIC_OBJECT;
			m_body->setMassProps(0, btVector3(0, 0, 0));
			m_body->setActivationState(DISABLE_DEACTIVATION);
		}
		else if (m_type == BodyType::Dynamic) {
			flags &= ~(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);
			btVector3 inertia(0, 0, 0);
			if (m_mass > 0.0f && m_collider->GetShape()) {
				m_collider->GetShape()->calculateLocalInertia(btScalar(m_mass), inertia);
			}
			m_body->setMassProps(btScalar(m_mass), inertia);
		}

		m_body->setCollisionFlags(flags);
		m_body->updateInertiaTensor();
		m_body->clearForces(); // Zera forças antigas
		m_body->setLinearVelocity(btVector3(0, 0, 0));

		// 2. Re-adiciona ao mundo para ele ser colocado na lista correta de gravidade
		if (wasInWorld) physicsManager.AddRigidBody(this);
		m_body->activate(true);
	}
}