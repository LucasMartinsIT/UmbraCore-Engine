#include "Engine.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace eng
{
	void keyCallback(GLFWwindow* window, int key, int, int action, int)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS)
		{
			inputManager.SetKeyPressed(key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			inputManager.SetKeyPressed(key, false);
		}
	}

	Engine& Engine::GetInstance()
	{
		static Engine instance;
		return instance;

	}
	bool Engine::Init(int width, int height)
	{
		// Safety check: making sure we have an Application set before initializing, otherwise it crashes
		if (!m_application)
		{
			return false;
		}

		if (!glfwInit())
		{
			return false;
		}

		//Config of wich OpenGl version were using here
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Window Parameter
		m_window = glfwCreateWindow(width, height, "GameEngine", nullptr, nullptr);

		//Checks window ok
		if (m_window == nullptr)
		{
			std::cout << "Error creating window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwSetKeyCallback(m_window, keyCallback);

		glfwMakeContextCurrent(m_window);

		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
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

		while (!glfwWindowShouldClose(m_window) && !m_application->NeedsToBeClosed())
		{
			glfwPollEvents();//Process events

			auto now = std::chrono::high_resolution_clock::now();// Calculates the time passed since the last frame (deltaTime) 
			float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
			m_lastTimePoint = now;

			m_application->Update(deltaTime);

			glfwSwapBuffers(m_window);//Handle the rendering swaping buffers	
		}
	}

	void Engine::Destroy()
	{
		if (m_application)
		{
			m_application->Destroy();
			// reset() destroys the object held by the unique_ptr and safely frees the memory
			m_application.reset();
			glfwTerminate();
			m_window = nullptr;
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

	InputManager& Engine::GetInputManager()
	{
		return m_inputManager;
	}

	GraphicsAPI& Engine::GetGraphicsAPI()
	{
		return m_graphicsAPI;
	}

}