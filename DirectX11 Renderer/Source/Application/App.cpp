#include "App.h"

#include "../Time/TimeHandler.h"

int App::SystemUpdate()
{
	while (true)
	{
		// INFO: Handle Time
		TimeHandler::Tick();

		// process all messages pending, but to not block for new messages
		if (const auto exitCode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *exitCode;
		}

		CustomUpdate();
	}
}

void App::CustomUpdate()
{
}
