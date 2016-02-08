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

#ifndef JOP_DRAWABLE_HPP
#define JOP_DRAWABLE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class Layer;
    class Model;
    class Shader;
    class Texture;
    class Camera;

    class JOP_API Drawable : public Component
    {
    private:

        JOP_DISALLOW_MOVE(Drawable);

        void operator =(const Drawable&) = delete;

    public:
    
        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param ID Unique component identifier
        ///
        Drawable(Object& object, const std::string& ID);

        /// \brief Copy constructor
        ///
        Drawable(const Drawable& other);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() = 0;


        /// \copydoc jop::Component::clone()
        ///
        virtual Drawable* clone() const override = 0;

        /// \brief Draw function
        ///
        virtual void draw(const Camera&) = 0;


        /// \brief Set the model
        ///
        /// \param model Reference to the model
        ///
        void setModel(const Model& model);

        /// \brief Set the shader
        ///
        /// \param shader Reference to the shader
        ///
        void setShader(Shader& shader);

        /// \brief Set the texture
        ///
        /// \param texture Reference to the texture
        ///
        void setTexture(const Texture& texture);

    protected:

        std::weak_ptr<const Model> m_model; ///< The bound model
        std::weak_ptr<Shader> m_shader;     ///< The bound shader
        std::weak_ptr<Texture> m_texture;   ///< The bound texture
        
    };
}

#endif

/// \class Drawable
/// \ingroup graphics
///
/// #TODO Detailed description