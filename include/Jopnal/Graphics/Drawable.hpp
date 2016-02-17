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
#include <Jopnal/Graphics/Model.hpp>
#include <Jopnal/Utility/Json.hpp>
#include <memory>
#include <unordered_set>

//////////////////////////////////////////////


namespace jop
{
    class Model;
    class Shader;
    class Camera;
    class Scene;

    class JOP_API Drawable : public Component
    {
    private:

        JOP_DISALLOW_MOVE(Drawable);

        void operator =(const Drawable&) = delete;

        friend class Layer;

    public:
    
        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param ID Unique component identifier
        ///
        Drawable(Object& object, const std::string& ID);

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
        /// The model will be copied.
        ///
        /// \param model Reference to the model
        ///
        void setModel(const Model& model);

        const Model& getModel() const;

        /// \brief Set the shader
        ///
        /// \param shader Reference to the shader
        ///
        void setShader(Shader& shader);

        std::weak_ptr<Shader> getShader() const;

        const std::unordered_set<Layer*> getBoundLayers() const;

        static bool loadStateBase(Drawable& drawable, const Scene& scene, const json::Value& val);

        static bool saveStateBase(const Drawable& comp, json::Value& val, json::Value::AllocatorType& alloc);

    private:

        Model m_model;                                      ///< The bound model
        mutable std::unordered_set<Layer*> m_boundToLayers; ///< Set of layers this drawable is bound to
        std::weak_ptr<Shader> m_shader;                     ///< The bound shader
        
    };
}

#endif

/// \class Drawable
/// \ingroup graphics
///
/// #TODO Detailed description