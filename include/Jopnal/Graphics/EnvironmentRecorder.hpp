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

#ifndef JOP_ENVIRONMENTRECORDER_HPP
#define JOP_ENVIRONMENTRECORDER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <memory>
#include <set>

//////////////////////////////////////////////


namespace jop
{
    class Drawable;
    class Renderer;

    class JOP_API EnvironmentRecorder final : public Component
    {
    private:

        /// \brief Copy constructor
        ///
        /// \param other The other recorder to copy
        /// \param newObj The new object
        ///
        EnvironmentRecorder(const EnvironmentRecorder& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(EnvironmentRecorder);
        JOP_GENERIC_COMPONENT_CLONE(EnvironmentRecorder);

    public:

        /// \brief Constructor
        ///
        /// \param obj Reference to the object
        /// \param renderer The renderer
        ///
        EnvironmentRecorder(Object& obj, Renderer& renderer);

        /// \brief Destructor
        ///
        ~EnvironmentRecorder() override;


        /// \brief Record/render the environment map
        ///
        /// This will fetch the drawables & lights straight from the renderer.
        ///
        void record();

        /// \brief Set the render mask
        ///
        /// \param mask The new mask to set
        ///
        /// \comm setEnvRecorderRenderMask
        ///
        void setRenderMask(const uint32 mask);

        /// \brief Get the render mask
        ///
        /// \return The render mask
        ///
        uint32 getRenderMask() const;

        /// \brief Get the texture
        ///
        /// \return Const pointer to the texture, nullptr the creation of one failed
        ///
        const Texture* getTexture() const;

    private:

        RenderTexture m_fbo;                ///< The frame buffer
        std::vector<glm::mat4> m_matrices;  ///< Matrices for each cube face
        uint32 m_mask;                      ///< Render mask
        Renderer& m_rendererRef;            ///< Reference to the renderer
    };
}

#endif