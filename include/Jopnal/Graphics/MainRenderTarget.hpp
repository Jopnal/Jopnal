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

#ifndef JOP_MAINRENDERTARGET_HPP
#define JOP_MAINRENDERTARGET_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Graphics/RenderTexture.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Window;

    class JOP_API MainRenderTarget : public RenderTexture, public Subsystem
    {
    public:

        /// \brief Constructor
        ///
        /// \param window Reference to the main window
        ///
        MainRenderTarget(const Window& window);


        /// \brief Update
        ///
        /// This will simply clear the back buffer.
        ///
        /// \param deltaTime The delta time, not used
        ///
        void preUpdate(const float deltaTime) override;

    private:

        const Window& m_windowRef;  ///< Main window reference
    };
}

/// \class jop::MainRenderTarget
/// \ingroup graphics

#endif