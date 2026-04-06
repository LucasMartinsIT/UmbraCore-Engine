#include "Engine.h"
#include "Application.h"

namespace eng
{
	bool Engine::Init()
	{
		// Safety check: making sure we have an Application set before initializing, otherwise it crashes
		if (!m_application)
		{
			return false;
		}

		return m_application->Init();
	}

	void Engine::Run()
	{
		if (!m_application)
		{
			return;
		}

		// The classic Game Loop starts here
		m_lastTimePoint = std::chrono::high_resolution_clock::now();

		while (!m_application->NeedsToBeClosed())
		{
			// Calculates the time passed since the last frame (deltaTime) 
			// This is crucial so physics and movement aren't tied to the framerate
			auto now = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
			m_lastTimePoint = now;

			m_application->Update(deltaTime);
		}
	}

	void Engine::Destroy()
	{
		if (m_application)
		{
			m_application->Destroy();
			// reset() destroys the object held by the unique_ptr and safely frees the memory
			m_application.reset();
		}
	}

	void Engine::SetApplication(Application* app)
	{
		// Passes ownership of the "app" pointer over to the engine
		m_application.reset(app);
	}

	Application* Engine::GetApplication()
	{
		// Returns a raw pointer just for viewing/usage, the engine still owns the actual object
		return m_application.get();
	}
}