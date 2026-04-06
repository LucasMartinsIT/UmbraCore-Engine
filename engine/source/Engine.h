#pragma once
#include <memory>
#include <chrono>

namespace eng
{
	class Application;
	class Engine
	{
	public:
		bool Init();
		void Run();
		void Destroy();

		void SetApplication(Application* app);
		Application* GetApplication();

	private:
		// Using unique_ptr so the engine automatically manages the app's memory and avoids leaks
		std::unique_ptr<Application> m_application;

		// Stores the exact time of the last frame so we can calculate deltaTime later
		std::chrono::steady_clock::time_point m_lastTimePoint;

	};
}