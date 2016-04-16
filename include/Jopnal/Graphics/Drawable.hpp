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

//////////////////////////////////////////////


namespace jop
{
    class Shader;
    class LightSource;
    class LightContainer;
    class Renderer;
    class Camera;

    class JOP_API Drawable : public Component
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Drawable);

        enum
        {
            ReceiveLights = 1,
            ReceiveShadows = 1 << 1,
            CastShadows = 1 << 2,
            Reflected = 1 << 3
        };

    protected:

        /// \brief Copy constructor
        ///
        /// \param other The other drawable to copy
        /// \param newObj The new object
        ///
        Drawable(const Drawable& other, Object& newObj);

    public:
    
        /// \brief Constructor
        ///
        /// \param object Reference to the object this drawable will be bound to
        /// \param renderer Reference to the renderer
        /// \param ID Component identifier
        ///
        Drawable(Object& object, Renderer& renderer, const std::string& ID);

        /// \brief Virtual destructor
        ///
        virtual ~Drawable() override = 0;


        
        /// \brief Base draw function
        ///
        /// This will use the shader bound to this drawable.
        /// 
        /// \param camera The camera to use
        /// \param lights The light container
        ///
        void draw(const Camera& camera, const LightContainer& lights) const;

        /// \brief Draw function
        ///
        /// The camera pointer can be null some times. In these cases it means that the view & perspective
        /// matrices have already been taken care of and you shouldn't try to set them.
        ///
        virtual void draw(const Camera*, const LightContainer&, Shader&) const = 0;


        /// \brief Get the renderer this drawable is bound to
        ///
        /// \return Reference to the renderer
        ///
        Renderer& getRendrer();

        /// \copydoc getRenderer
        ///
        const Renderer& getRenderer() const;


        /// \brief Set the render group
        ///
        /// The value will be clamped between 0 and 31.
        ///
        /// \param group The new group to set
        ///
        /// \comm setRenderGroup
        ///
        Drawable& setRenderGroup(const uint8 group);

        /// \brief Get the render group
        ///
        /// \return The render group 
        ///
        uint8 getRenderGroup() const;


        /// \brief Set the model
        ///
        /// The model will be copied.
        ///
        /// \param model Reference to the model
        ///
        /// \comm setModel
        ///
        /// \return Reference to self
        ///
        Drawable& setModel(const Model& model);

        /// \brief Get the model
        ///
        /// \return Reference to the model
        ///
        Model& getModel();

        /// \copydoc getModel()
        ///
        const Model& getModel() const;


        /// \brief Set the shader
        ///
        /// This can be used to override the shader fetched from the bound material.
        ///
        /// \param shader Reference to the shader
        ///
        /// \comm setShader
        ///
        Drawable& setShader(Shader& shader);

        /// \brief Remove the shader
        ///
        /// \param loadMaterialShader Set this to true to automatically fetch a new shader
        ///                           from the bound material. If this is false, there won't
        ///                           be a shader bound after this call, and thus, drawing
        ///                           will fail.
        ///
        /// \return Reference to self 
        ///
        Drawable& removeShader(const bool loadMaterialShader = true);

        /// \brief Get the shader
        ///
        /// \return Pointer to the shader. Empty if none bound
        ///
        Shader* getShader() const;


        /// \brief Set whether or not this drawable receives lights
        ///
        /// \param receive True to receive lights
        ///
        /// \comm setReceiveLights
        ///
        Drawable& setReceiveLights(const bool receive);

        /// \brief Check if this drawable receives lights
        ///
        /// \return True if receives lights
        ///
        bool receiveLights() const;


        /// \brief Check if a light affects this drawable
        /// 
        /// \param light The light to check against
        ///
        /// \return True if the light affects
        ///
        bool lightTouches(const LightSource& light) const;


        /// \brief Set the flag to receive shadows
        ///
        /// \param receive True to set this to receive shadows
        ///
        /// \comm setReceiveShadows
        ///
        Drawable& setReceiveShadows(const bool receive);

        /// \brief Check if this drawable receives shadows
        ///
        /// \return True if this drawable receives shadows
        ///
        bool receiveShadows() const;

        /// \brief Set the flag to cast shadows
        /// 
        /// \param cast True to set this to cast shadows
        ///
        /// \comm setCastShadows
        ///
        Drawable& setCastShadows(const bool cast);

        /// \brief Check if this drawable casts shadows
        ///
        /// \return True if this drawable casts shadows
        ///
        bool castShadows() const;


        /// \brief Set whether or not this drawable should be reflected in dynamic environment maps
        ///
        /// \param reflected True to set this to be reflected
        ///
        /// \comm setReflected
        ///
        Drawable& setReflected(const bool reflected);

        /// \brief Check if this drawable is reflected in dynamic environment maps
        ///
        /// \return True if this is reflected
        ///
        bool isReflected() const;


        /// \brief Load the state
        ///
        /// This can be called by the derived class while loading serialized state.
        ///
        /// \param drawable Reference to the drawable to load
        /// \param scene The scene this drawable is bound to
        /// \param val The json value
        ///
        /// \return True if successful
        ///
        static bool loadStateBase(Drawable& drawable, const Scene& scene, const json::Value& val);

        /// \brief Save the state
        ///
        /// This can be called by the derived class while serializing state.
        ///
        /// \param drawable Reference to the drawable to save
        /// \param val The json value
        /// \param alloc The json allocator
        ///
        /// \return True if successful
        ///
        static bool saveStateBase(const Drawable& drawable, json::Value& val, json::Value::AllocatorType& alloc);

    private:

        mutable Model m_model;                  ///< The bound model
        mutable WeakReference<Shader> m_shader; ///< The bound shader
        Renderer& m_rendererRef;                ///< Reference to the renderer
        uint8 m_renderGroup;                    ///< The render group
        unsigned char m_flags;                  ///< Property flags
    };
}

#endif

/// \class Drawable
/// \ingroup graphics
///
/// #TODO Detailed description