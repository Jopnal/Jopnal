// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_SCENE_HPP
#define JOP_SCENE_HPP

// Headers
#include <Jopnal/Header.hpp>
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


        /// \brief Method for checking if object with 'ID' exists
        ///
        /// \param ID Object identifier  
        ///
        Object* getObject(const std::string& ID);

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
        Object* cloneObject(const std::string& ID);

        /// \brief Method for deleting object 
        ///
        /// \param ID Object identifier 
        ///
        void deleteObject(const std::string& ID); 

        /// \brief Method for clearing m_objects 
        ///
        void clearObjects(); 

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
        /// \param ptr Pointer to hold extra data
        ///
        void sendMessage(const std::string& message, void* ptr);

        /// \brief Update method for scene
        ///
        /// \param deltaTime Double holding time step
        ///
        void updateBase(const double timeStep);

        /// \brief fixedUpdate method for scene
        ///
        /// \param deltaTime Double holding time step
        ///
        void fixedUpdateBase(const double timeStep);

        /// \brief Method for drawing
        ///
        void drawBase();
        
        /// \brief Method for pre-updating
        ///
        /// This will be called before objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void preUpdate(const double deltaTime);

        /// \brief Method for post-updating
        ///
        /// This will be called after objects are updated.
        ///
        /// \param deltaTime double holding delta time
        ///
        virtual void postUpdate(const double deltaTime);

        /// \brief Method for pre-fixed updating
        ///
        /// This will be called before objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void preFixedUpdate(const double timeStep);

        /// \brief Method for post-fixed updating
        ///
        /// This will be called after objects are updated.
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void postFixedUpdate(const double timeStep);
        
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

        /// \brief Send a message to this scene
        ///
        /// \param message String holding message
        /// \param ptr Pointer to hold extra data
        ///
        virtual void sendMessageImpl(const std::string& message, void* ptr);


        std::vector<std::shared_ptr<Object>> m_objects; ///< Container holding objects
        std::vector<std::shared_ptr<Layer>> m_layers;   ///< Container holding layers
        std::string m_ID;                               ///< String holding scene identifier
    };
}

#endif

/// \class Scene
/// \ingroup core