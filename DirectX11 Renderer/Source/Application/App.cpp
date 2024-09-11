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
	const float s = sin(TimeHandler::GetElapsedTime()) / 2.0f + 0.5f;
	const float c = cos(TimeHandler::GetElapsedTime()) / 2.0f + 0.5f;
	window.GetRenderer().ClearBuffer(s, c, 1.0f);
	window.GetRenderer().DrawTestTriangle(TimeHandler::GetElapsedTime() - 1.0f, 0.0f, 0.0f);
	window.GetRenderer().DrawTestTriangle(TimeHandler::GetElapsedTime(), window.mouse.GetXPosition() / 400.0f - 1.0f, -window.mouse.GetYPosition() / 300.0f + 1.0f);
	window.GetRenderer().EndFrame();
}
