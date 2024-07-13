#include "Mouse.h"
#include "../Generics/MinimalWindows.h"

Mouse::Event Mouse::ReadMouse()
{
	if (!MouseBufferIsEmpty())
	{
		Event event = mouseBuffer.front();
		mouseBuffer.pop();
		return event;
	}
	else
		return Event();
}

void Mouse::OnLeftPressed(int _x, int _y)
{
	leftIsPressed = true;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LPressed, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int _x, int _y)
{
	leftIsPressed = false;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LReleased, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int _x, int _y)
{
	middleIsPressed = true;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::MPressed, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int _x, int _y)
{
	middleIsPressed = false;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::MReleased, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int _x, int _y)
{
	rightIsPressed = true;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RPressed, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int _x, int _y)
{
	rightIsPressed = false;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RReleased, *this));
	TrimBuffer();

}

void Mouse::OnWheelDelta(int _x, int _y, int _delta)
{
	wheelDeltaCarry += _delta;

	while (wheelDeltaCarry > WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(_x, _y);
	}

	while (wheelDeltaCarry < -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(_x, _y);
	}
}

void Mouse::OnWheelUp(int _x, int _y)
{
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int _x, int _y)
{
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
	mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	// INFO: While the Buffer Size is Greater than the Maximum Allowed Buffer Size
	while (mouseBuffer.size() > bufferSize)
		mouseBuffer.pop();
}
