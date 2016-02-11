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

#ifndef JOP_SCENE_HPP
#define JOP_SCENE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <vector>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class Object;
    class Layer;

    class JOP_API Scene 
    {
    public:

        JOP_DISALLOW_COPY_MOVE(Scene);

    public:

        /// \brief Constructor for scene class
        ///
        /// \param ID String holding name for scene
        ///
        Scene(const std::string& ID);

        /// \brief Destructor for scene class
        ///
        virtual ~Scene();


        /// \brief Get an object with the given id
        ///
        /// \param ID Object identifier
        ///
        /// \return Pointer to the object if found, empty otherwise
        ///
        std::weak_ptr<Object> getObject(const std::string& ID);

        /// \brief Method that creates object
        ///
        /// \param ID Object identifier
        ///
        Object& createObject(const std::string& ID);

        /// \brief Clone an object with the given id
        ///
        /// The object, if successfully cloned, will be added to the internal array
        /// and then returned.
        ///
        /// \param ID The id to search with
        ///
        /// \return Pointer to the newly cloned child object if the object was found, nullptr otherwise
        ///
        std::weak_ptr<Object> cloneObject(const std::string& ID);

        /// \brief Method for deleting object 
        ///
        /// \param ID Object identifier 
        ///
        void deleteObject(const std::string& ID);

        /// \brief Method for clearing m_objects 
        ///
        void clearObjects();

        /// \brief Retun the amount of objects
        ///
        unsigned int objectCount() const;

        /// \brief Get a layer with the given id
        ///
        /// \param ID The identifier to search with
        ///
        /// \return Pointer to the layer if found, empty otherwise
        ///
        std::weak_ptr<Layer> getLayer(const std::string& ID);

        /// \brief Get a layer using type info
        ///
        /// \return Pointer to the layer. Empty if not found
        ///
        template<typename T>
        std::weak_ptr<T> getLayer();

        /// \brief Create a new layer
        ///
        /// \param args Arguments to pass into the constructor
        ///
        /// \return Reference to the newly created layer
        ///
        template<typename T, typename ... Args>
        T& createLayer(Args&... args);

        /// \brief Delete a layer with the given id
        ///
        /// \param ID The id to search with
        ///
        void deleteLayer(const std::string& ID);

        /// \brief Delete all layers
        ///
        void clearLayers();


        /// \brief Replace the default layer
        ///
        template<typename T, typename ... Args>
        T& setDefaultLayer(Args&... args);

        /// \brief Get the default layer
        ///
        /// \return Reference to the default layer
        ///
        Layer& getDefaultLayer();


        /// \brief Set the ID of this scene
        ///
        /// \param ID The new identifier
        ///
        void setID(const std::string& ID);

        /// \brief Get the ID of this scene
        ///
        /// \return Reference to the internal string with the id
        ///
        const std::string& getID() const;


        /// \brief Base sendMessage function
        ///
        /// This will handle message filtering and forwarding
        /// to the objects
        ///
        /// \param message String holding message
        ///
        MessageResult sendMessage(const std::string& message);

        /// \brief Base sendMessage function
        ///
        /// This will handle message filtering and forwarding
        /// to the objects
        ///
        /// \param message String holding message
        /// \param returnWrap Pointer to hold extra data
        ///
        MessageResult sendMessage(const std::string& message, Any& returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        MessageResult sendMessage(const Message& message);

        /// \brief Sets active on update functions
        ///
        /// \param active Sets the active
        ///
        void setActive(const bool active);

        /// \brief Returns m_active boolean unit
        ///
        bool isActive();

        /// \brief Update method for scene
        ///
        /// \param deltaTime Double holding time step
        ///
        void updateBase(const float deltaTime);

        /// \brief fixedUpdate method for scene
        ///
        /// \param deltaTime Double holding time step
        ///
        void fixedUpdateBase(const float timeStep);

        /// \brief Method for drawing
        ///
        void drawBase();


        /// \brief Initialize this scene
        ///
        /// This function should be preferred when doing scene initialization.
        ///
        virtual void initialize();
        

        /// \brief Method for pre-updating
        ///
        /// This will be called before objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void preUpdate(const float deltaTime);

        /// \brief Method for post-updating
        ///
        /// This will be called after objects are updated.
        ///
        /// \param deltaTime double holding delta time
        ///
        virtual void postUpdate(const float deltaTime);

        /// \brief Method for pre-fixed updating
        ///
        /// This will be called before objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void preFixedUpdate(const float timeStep);

        /// \brief Method for post-fixed updating
        ///
        /// This will be called after objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void postFixedUpdate(const float timeStep);
        
        /// \brief Method for pre-drawing
        ///
        /// This will be called before objects are drawn.
        ///
        virtual void preDraw();

        /// \brief Method for post drawing
        ///
        /// This will be called after objects are drawn.
        ///
        virtual void postDraw();

    private:

        /// \brief Virtual sendMessage
        ///
        virtual MessageResult sendMessageImpl(const Message& message);


        std::vector<std::shared_ptr<Object>> m_objects; ///< Container holding objects
        std::vector<std::shared_ptr<Layer>> m_layers;   ///< Container holding layers
        std::shared_ptr<Layer> m_defaultLayer;          ///< The default layer
        std::string m_ID;                               ///< String holding scene identifier
        bool m_active;                                  ///< Boolean set to active
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Scene.inl>
}

#endif

/// \class Scene
/// \ingroup core