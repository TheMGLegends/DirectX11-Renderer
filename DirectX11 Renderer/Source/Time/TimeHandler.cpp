#include "TimeHandler.h"

float TimeHandler::deltaTime = 0.0f;
float TimeHandler::elapsedTime = 0.0f;
float TimeHandler::timeScale = 1.0f;

std::chrono::steady_clock::time_point TimeHandler::currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point TimeHandler::previousTime = std::chrono::steady_clock::now();

void TimeHandler::Tick()
{
	currentTime = std::chrono::steady_clock::now();

	// INFO: The time between frames in seconds
	deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1e9);

	// INFO: The time since the start of the application in seconds
	elapsedTime += deltaTime;

	previousTime = currentTime;
}
