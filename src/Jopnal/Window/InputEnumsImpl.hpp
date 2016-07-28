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

#ifndef JOP_INPUTENUMSIMPL_HPP
#define JOP_INPUTENUMSIMPL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Window/Keyboard.hpp>
#include <Jopnal/Window/Mouse.hpp>
#include <Jopnal/Window/Controller.hpp>
#include <Jopnal/Window/Touch.hpp>

//////////////////////////////////////////////
namespace jop
{
	namespace input
	{
		JOP_API int getJopKey(const int key);
        JOP_API int getGlKey(const int key);
        JOP_API int getJopControllerButton(const int button);
        JOP_API int getJopControllerAxis(const int button);
        JOP_API int getGlControllerButton(const int button);
        JOP_API int getGlControllerAxis(const int button);
        JOP_API int getJopMouseButton(const int button);
        JOP_API int getGlMouseButton(const int button);
        JOP_API int getJopTouchInfo(const int info);
        JOP_API int getGlTouchInfo(const int info);
	}
}
#endif