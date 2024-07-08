#include "Keyboard.h"

void Keyboard::OnKeyPressed(unsigned char keyCode)
{
	// INFO: Set the Previous Key State to the Current Key State
	previousKeyStates[keyCode] = currentKeyStates[keyCode];

	// INFO: Set the Current Key State to True
	currentKeyStates[keyCode] = true;

	// INFO: Push the Key Pressed Event to the Key Buffer
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Pressed, keyCode));
	
	// INFO: Trim the Key Buffer
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyHeld(unsigned char keyCode)
{
	// INFO: Set the Previous Key State to the Current Key State
	previousKeyStates[keyCode] = currentKeyStates[keyCode];

	// INFO: Set the Current Key State to True
	currentKeyStates[keyCode] = true;

	// INFO: Push the Key Held Event to the Key Buffer
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Held, keyCode));

	// INFO: Trim the Key Buffer
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode)
{
	// INFO: Set the Previous Key State to the Current Key State
	previousKeyStates[keyCode] = currentKeyStates[keyCode];

	// INFO: Set the Current Key State to False
	currentKeyStates[keyCode] = false;

	// INFO: Push the Key Released Event to the Key Buffer
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Released, keyCode));

	// INFO: Trim the Key Buffer
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character)
{
	// INFO: Push the Character to the Character Buffer
	characterBuffer.push(character);

	// INFO: Trim the Character Buffer
	TrimBuffer(characterBuffer);
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	// INFO: While the Buffer Size is Greater than the Maximum Allowed Buffer Size
	while (buffer.size() > bufferSize)
		buffer.pop();
}

bool Keyboard::GetKeyPressed(unsigned char keyCode)
{
	if (currentKeyStates[keyCode] && !previousKeyStates[keyCode])
	{
		previousKeyStates[keyCode] = currentKeyStates[keyCode];
		return true;
	}
	else
		return false;
}

bool Keyboard::GetKeyReleased(unsigned char keyCode)
{
	if (!currentKeyStates[keyCode] && previousKeyStates[keyCode])
	{
		previousKeyStates[keyCode] = currentKeyStates[keyCode];
		return true;
	}
	else
		return false;
}

std::optional<Keyboard::Event> Keyboard::ReadKey()
{
	if (!KeyBufferIsEmpty())
	{
		Keyboard::Event event = keyBuffer.front();
		keyBuffer.pop();
		return event;
	}
	else
		return std::nullopt;
}

std::optional<char> Keyboard::ReadCharacter()
{
	if (!CharacterBufferIsEmpty())
	{
		unsigned char character = characterBuffer.front();
		characterBuffer.pop();
		return character;
	}
	else
		return std::nullopt;
}