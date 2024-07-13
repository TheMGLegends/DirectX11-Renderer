#pragma once

#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,

			Pressed,
			Held,
			Released
		};

	private:
		Type type;
		unsigned char code;

	public:
		/// @brief Default Constructor - Initializes the Event to Invalid State
		inline Event() : type(Type::Invalid), code(0u) {}
		inline Event(Type _type, unsigned char _code) : type(_type), code(_code) {}

		inline bool IsInvalid() const { return type == Type::Invalid; }
		inline bool IsPressed() const { return type == Type::Pressed; }
		inline bool IsHeld() const { return type == Type::Held; }
		inline bool IsReleased() const { return type == Type::Released; }

		inline unsigned char GetCode() const { return code; }
	};

private:
	static constexpr unsigned int keyLength = 256u;
	static constexpr unsigned int bufferSize = 16u;

	std::bitset<keyLength> currentKeyStates;
	std::bitset<keyLength> previousKeyStates;

	std::queue<Event> keyBuffer;
	std::queue<char> characterBuffer;

private:
	void OnKeyPressed(unsigned char keyCode);
	void OnKeyHeld(unsigned char keyCode);
	void OnKeyReleased(unsigned char keyCode);
	void OnChar(char character);

	inline void ClearKeyStates() { currentKeyStates.reset(); previousKeyStates.reset(); }

	template<typename T>
	void TrimBuffer(std::queue<T>& buffer);

public:
	Keyboard() = default;

	// INFO: Key Event Handling
	inline bool GetKeyPressed(unsigned char keyCode) const { return currentKeyStates[keyCode] && !previousKeyStates[keyCode]; }
	inline bool GetKeyHeld(unsigned char keyCode) const { return currentKeyStates[keyCode] && previousKeyStates[keyCode]; }
	bool GetKeyReleased(unsigned char keyCode);

	Event ReadKey();
	inline bool KeyBufferIsEmpty() const { return keyBuffer.empty(); }
	inline void ClearKeyBuffer() { keyBuffer = std::queue<Event>(); }

	// INFO: Character Event Handling
	char ReadCharacter();
	inline bool CharacterBufferIsEmpty() const { return characterBuffer.empty(); }
	inline void ClearCharacterBuffer() { characterBuffer = std::queue<char>(); }

	inline void ClearBuffers() { ClearKeyBuffer(); ClearCharacterBuffer(); }
};
