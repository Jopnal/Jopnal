// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

#include <Jopnal/Window/Controller.hpp>

#ifdef JOP_OS_DESKTOP
#include <GLFW/glfw3.h>
#endif

#endif

//////////////////////////////////////////////

namespace
{
	int getXBoxToPlaystation(const int XBoxButton)
	{
		using X = jop::Controller::XBox::Button;
		using P = jop::Controller::Playstation::Button;

		switch (XBoxButton)
		{
		case X::A:            return P::X;
		case X::X:            return P::Square;
		case X::Y:            return P::Triangle;
		case X::B:            return P::O;
		case X::LShoulder:    return P::LShoulder;
		case X::RShoulder:    return P::RShoulder;
		case X::Back:         return P::Select;
		case X::Start:        return P::Start;
		case X::LStick:       return P::LStick;
		case X::RStick:       return P::RStick;
		case X::PadUp:        return P::PadUp;
		case X::PadRight:     return P::PadRight;
		case X::PadDown:      return P::PadDown;
		case X::PadLeft:      return P::PadLeft;
		}		
		return XBoxButton;
	}

	int getPlaystationToXBox(const int PlaystationButton)
	{
		using X = jop::Controller::XBox::Button;
		using P = jop::Controller::Playstation::Button;

		switch (PlaystationButton)
		{
		case P::X:            return X::A;
		case P::Square:       return X::X;
		case P::Triangle:     return X::Y;
		case P::O:            return X::B;
		case P::LShoulder:    return X::LShoulder;
		case P::RShoulder:    return X::RShoulder;
		case P::Select:       return X::Back;
		case P::Start:        return X::Start;
		case P::LStick:       return X::LStick;
		case P::RStick:       return X::RStick;
		case P::PadUp:        return X::PadUp;
		case P::PadRight:     return X::PadRight;
		case P::PadDown:      return X::PadDown;
		case P::PadLeft:      return X::PadLeft;
		}
		return PlaystationButton;
	}
}

namespace jop
{

	bool Controller::isPressed(int controller, Playstation::Button button)
	{
		return false;
	}

	//////////////////////////////////////////////

	bool Controller::isPressed(int controller, XBox::Button button)
	{
		return false;
	}

	//////////////////////////////////////////////

	bool Controller::isReleased(int controller, Playstation::Button button)
	{
		return false;
	}

	//////////////////////////////////////////////

	bool Controller::isReleased(int controller, XBox::Button button)
	{
		return false;
	}

	//////////////////////////////////////////////

	glm::vec2 Controller::rightStickOffset(int controller)
	{
		return glm::vec2(0, 0);
	}

	//////////////////////////////////////////////

	glm::vec2 Controller::leftStickOffset(int controller)
	{
		return glm::vec2(0, 0);
	}

	//////////////////////////////////////////////

	void Controller::controllerConnected(const int, const std::string&)
	{}

	//////////////////////////////////////////////

	void Controller::controllerDisconnected(const int)
	{}

	//////////////////////////////////////////////

	void Controller::controllerAxisShifted(const int, const int, const float)
	{}

	//////////////////////////////////////////////

	void Controller::controllerButtonPressed(const int, const int)
	{}

	//////////////////////////////////////////////

	void Controller::controllerButtonReleased(const int, const int)
	{}

}