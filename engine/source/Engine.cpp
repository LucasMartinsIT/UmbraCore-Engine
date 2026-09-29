#include "Engine.h"
#include "Application.h"
#include "scene/GameObject.h"
#include "scene/Component.h"
#include "scene/components/CameraComponent.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
// --- INCLUDES DA IMGUI E EDITOR ---
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "editor/Editor.h"


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

	void mouseButtonCallBack(GLFWwindow* window, int button, int action, int)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();
		if (action == GLFW_PRESS)
		{
			inputManager.SetMouseButtonPressed(button, true);
		}
		else if (action == GLFW_RELEASE)
		{
			inputManager.SetMouseButtonPressed(button, false);
		}
	}

	void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos)
	{
		auto& inputManager = eng::Engine::GetInstance().GetInputManager();

		inputManager.SetMousePositionOld(inputManager.GetMousePositionCurrent());

		glm::vec2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
		inputManager.SetMousePositionCurrent(currentPos);

		inputManager.SetMousePositionChanged(true);
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

		Scene::RegisterTypes();
		m_application->RegisterTypes();

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
		glfwSetMouseButtonCallback(m_window, mouseButtonCallBack);
		glfwSetCursorPosCallback(m_window, cursorPositionCallBack);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwMakeContextCurrent(m_window);

		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}

		// --- INICIALIZAÇÃO DA IMGUI ---
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		m_graphicsAPI.Init();
		m_physicsManager.Init();
		m_audioManager.Init();
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

		eng::Editor sceneEditor;
		bool isEditorMode = true;
		bool tabWasPressed = false; // Para evitar que a tecla ative/desative múltiplas vezes por frame

		while (!glfwWindowShouldClose(m_window) && !m_application->NeedsToBeClosed())
		{
			glfwPollEvents();//Process events

			auto now = std::chrono::high_resolution_clock::now();// Calculates the time passed since the last frame (deltaTime) 
			float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
			m_lastTimePoint = now;

			// Lógica da Tecla TAB ANTES da atualização da Aplicação/Física
			// Usamos glfwGetKey para ler o input crú e evitar atrasos do InputManager
			bool isTabPressed = (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS);

			if (isTabPressed && !tabWasPressed) {
				isEditorMode = !isEditorMode;
			}
			tabWasPressed = isTabPressed;

			// Se estiver no Modo Editor, a física e a lógica do jogo são pausadas.
			// Isso impede que o seu PlayerController sobrescreva o estado do cursor!
			if (!isEditorMode) {
				m_physicsManager.Update(deltaTime);
				m_application->Update(deltaTime);
			}

			// Força o cursor para o estado correto TODOS OS FRAMES
			int cursorMode = isEditorMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
			glfwSetInputMode(m_window, GLFW_CURSOR, cursorMode);

			m_graphicsAPI.SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_graphicsAPI.ClearBuffers();

			CameraData cameraData;
			std::vector<LightData> lights;

			int width = 0;
			int height = 0;
			glfwGetWindowSize(m_window, &width, &height);
			float aspect = static_cast<float>(width) / static_cast<float>(height);

			if (m_currentScene)
			{
				if (auto cameraObject = m_currentScene->GetMainCamera())
				{
					//Logic for matrices
					auto cameraComponent = cameraObject->GetComponent<CameraComponent>();
					if (cameraComponent)
					{
						cameraData.viewMatrix = cameraComponent->GetViewMatrix();
						cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
						cameraData.position = cameraObject->GetWorldPosition();
					}
				}

				lights = m_currentScene->CollectLights();
			}

			m_renderQueue.Draw(m_graphicsAPI, cameraData, lights);

			// --- RENDERIZAÇÃO DA IMGUI ---
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Se estiver no modo editor, desenha a interface
			if (isEditorMode) {
				sceneEditor.Render(m_currentScene.get());
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// -----------------------------

			glfwSwapBuffers(m_window);//Handle the rendering swaping buffers

			m_inputManager.SetMousePositionChanged(false);
		}
	}

	void Engine::Destroy()
	{
		if (m_application)
		{
			// --- LIMPEZA DA IMGUI (antes de glfwTerminate) ---
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

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

	RenderQueue& Engine::GetRenderQueue()
	{
		return m_renderQueue;
	}

	AudioManager& Engine::GetAudioManager()
	{
		return m_audioManager;
	}

	void Engine::SetScene(Scene* scene)
	{
		m_currentScene.reset(scene);
	}

	FileSystem& Engine::GetFileSystem()
	{
		return m_fileSystem;
	}

	TextureManager& Engine::GetTextureManager()
	{
		return m_textureManager;
	}

	PhysicsManager& Engine::GetPhysicsManager()
	{
		return m_physicsManager;
	}

	Scene* Engine::GetScene()
	{
		return m_currentScene.get();
	}
}