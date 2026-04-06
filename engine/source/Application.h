#pragma once

namespace eng
{
	class Application
	{
	public:
		// Pure virtual functions (= 0). This forces whoever creates the game to inherit this class and implement their own logic.
		virtual bool Init() = 0;
		// Delta time in seconds
		virtual void Update(float deltaTime) = 0;
		virtual void Destroy() = 0;

		void SetNeedsToBeClosed(bool value);
		bool NeedsToBeClosed() const;

	private:
		// Internal flag to tell the Engine it's time to break the while loop and shut down
		bool m_needsToBeClosed = false;
	};
}