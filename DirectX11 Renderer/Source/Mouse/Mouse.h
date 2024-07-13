#pragma once

#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Invalid,

			LPressed,
			LReleased,
			MPressed,
			MReleased,
			RPressed,
			RReleased,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave
		};

	private:
		Type type;

		bool leftIsPressed;
		bool middleIsPressed;
		bool rightIsPressed;

		int x;
		int y;

	public:
		/// @brief Default Constructor - Initializes the Event to Invalid State
		inline Event() : type(Type::Invalid), leftIsPressed(false), middleIsPressed(false), rightIsPressed(false), x(0), y(0) {}
		inline Event(Type _type, const Mouse& mouse) : type(_type), leftIsPressed(mouse.leftIsPressed), middleIsPressed(mouse.middleIsPressed), rightIsPressed(mouse.rightIsPressed), x(mouse.x), y(mouse.y) {}

		inline bool IsValid() const { return type != Type::Invalid; }
		inline Type GetType() const { return type; }

		inline bool LeftIsPressed() const { return leftIsPressed; }
		inline bool MiddleIsPressed() const { return middleIsPressed; }
		inline bool RightIsPressed() const { return rightIsPressed; }

		inline std::pair<int, int> GetPosition() const { return { x, y }; }
		inline int GetXPosition() const { return x; }
		inline int GetYPosition() const { return y; }
	};

private:
	static constexpr unsigned int bufferSize = 16u;

	std::queue<Event> mouseBuffer;

	bool leftIsPressed;
	bool middleIsPressed;
	bool rightIsPressed;

	bool isInWindow;

	int x;
	int y;

	int wheelDeltaCarry;

private:
	void OnLeftPressed(int _x, int _y);
	void OnLeftReleased(int _x, int _y);

	void OnMiddlePressed(int _x, int _y);
	void OnMiddleReleased(int _x, int _y);

	void OnRightPressed(int _x, int _y);
	void OnRightReleased(int _x, int _y);

	void OnWheelDelta(int _x, int _y, int _delta);
	void OnWheelUp(int _x, int _y);
	void OnWheelDown(int _x, int _y);

	void OnMouseMove(int _x, int _y);

	void OnMouseEnter();
	void OnMouseLeave();

	void TrimBuffer();

public:
	Mouse() = default;

	Event ReadMouse();
	inline bool MouseBufferIsEmpty() const { return mouseBuffer.empty(); }

	inline bool LeftIsPressed() const { return leftIsPressed; }
	inline bool MiddleIsPressed() const { return middleIsPressed; }
	inline bool RightIsPressed() const { return rightIsPressed; }

	inline bool IsInWindow() const { return isInWindow; }

	inline std::pair<int, int> GetPosition() const { return { x, y }; }
	inline int GetXPosition() const { return x; }
	inline int GetYPosition() const { return y; }

	inline void ClearMouseBuffer() { mouseBuffer = std::queue<Event>(); }

};

