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

#ifndef JOP_SUBSYSTEM_HPP
#define JOP_SUBSYSTEM_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Subsystem);

    public:

        /// \brief Constructor
        ///
        /// \param ID Identifier of this subsystem
        ///
        Subsystem(const std::string& ID);

        /// Virtual destructor
        ///
        virtual ~Subsystem() = 0;


        /// \brief Pre-update
        ///
        /// This will be called before the engine calls the scene's update.
        ///
        /// \param dt Delta time
        ///
        virtual void preUpdate(const double dt);

        /// \brief Pre-fixed update
        ///
        /// This will be called before the engine calls the scene's fixedUpdate.
        ///
        /// \param ts Time step
        ///
        virtual void preFixedUpdate(const double ts);

        /// \brief Post-fixed update
        ///
        /// This will be called after the engine calls the scene's fixedUpdate.
        ///
        /// \param ts Time step
        ///
        virtual void postFixedUpdate(const double ts);

        /// \brief Post-update
        ///
        /// This will be called after the engine calls the scene's update.
        /// This function can also be used as a pre-draw function.
        ///
        /// \param dt Delta time
        ///
        virtual void postUpdate(const double dt);

        /// \brief Post-draw
        ///
        /// This will be called after the engine calls the scene's draw.
        ///
        virtual void draw();


        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        /// \param ptr Pointer to hold extra data
        ///
        MessageResult sendMessage(const std::string& message, Any returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        MessageResult sendMessage(const Message& message);


        /// \brief Set the name
        ///
        /// \param ID The new id to be set
        ///
        void setID(const std::string& ID);

        /// \brief Get the name
        ///
        /// \return Reference to the name string
        ///
        const std::string& getID() const;

    protected:

        /// \brief Virtual sendMessage
        ///
        virtual MessageResult sendMessageImpl(const Message& message);

    private:

        std::string m_ID; ///< This subsystem's name

    };
}

#endif

/// \class SubSystem
/// \ingroup core
///
/// #TODO Detailed description