#include "Application.h"

namespace eng
{
	// Simple getters and setters for the application close flag
	void Application::SetNeedsToBeClosed(bool value)
	{
		m_needsToBeClosed = value;
	}

	bool Application::NeedsToBeClosed() const
	{
		return m_needsToBeClosed;
	}
}