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

#ifndef JOP_RENDERTARGET_HPP
#define JOP_RENDERTARGET_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <glm/vec2.hpp>
#include <mutex>
#include <atomic>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API RenderTarget
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderTarget);

    public:

        /// Buffer clear bits
        ///
        enum
        {
            ColorBit    = 1,        ///< Color bit
            DepthBit    = 1 << 1,   ///< Depth bit
            StencilBit  = 1 << 2,   ///< Stencil bit

            AllBit      = ColorBit | DepthBit | StencilBit  ///< Clear all
        };

    public:
    
        /// \brief Constructor
        ///
        /// \param ID Identifier of this render target
        ///
        RenderTarget();

        /// \brief Virtual destructor
        ///
        virtual ~RenderTarget() = 0;


        /// \brief Clear the selected buffers
        ///
        /// \param bits Bit field describing the buffer combination to clear
        ///
        void clear(const unsigned int bits);


        /// \brief Bind this render target for drawing
        ///
        /// \return True if successful
        ///
        virtual bool bind() const = 0;
        

        /// \brief Get the size
        ///
        /// \return The size
        ///
        virtual glm::uvec2 getSize() const = 0;


        /// \brief Set the clear color
        ///
        /// \param color The new clear color value
        ///
        /// \return Reference to self
        ///
        RenderTarget& setClearColor(const Color& color);

        /// \brief Get the clear color
        ///
        /// \return The clear color
        ///
        Color getClearColor() const;

        /// \brief Set the clear depth
        ///
        /// \param depth The new clear depth value
        ///
        /// \return Reference to self
        ///
        RenderTarget& setClearDepth(const float depth);

        /// \brief Get the clear depth
        ///
        /// \return The clear depth
        ///
        float getClearDepth() const;

        /// \brief Set the clear stencil
        ///
        /// \param stencil The new clear stencil value
        ///
        /// \return Reference to self
        ///
        RenderTarget& setClearStencil(const int stencil);

        /// \brief Get the clear stencil
        ///
        /// \return The clear stencil
        ///
        int getClearStencil() const;

    protected:

        mutable std::recursive_mutex m_mutex;   ///< Mutex

    private:

        Color m_clearColor;                         ///< The clear color
        std::atomic<float> m_clearDepth;            ///< The clear depth
        std::atomic<int> m_clearStencil;            ///< The clear stencil
    };
}

#endif

/// \class RenderTarget
/// \ingroup graphics
///
/// #TODO Detailed description