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
        /// \param name Name of this subsystem
        ///
        Subsystem(const std::string& name);

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

        /// \brief Post-update
        ///
        /// This will be called after the engine calls the scene's update.
        /// This function can also be used as pre-draw.
        ///
        /// \param dt Delta time
        ///
        virtual void postUpdate(const double dt);

        /// \brief Post-draw
        ///
        /// This will be called after the engine calls the scene's draw.
        ///
        virtual void postDraw();

        /// \brief Set the name
        ///
        /// \param name The new name to be set
        ///
        void setName(const std::string& name);

        /// \brief Get the name
        ///
        /// \return Reference to the name string
        ///
        const std::string& getName() const;

    private:

        std::string m_name; ///< This subsystem's name

    };
}

#endif

/// \class SubSystem
/// \ingroup core
///
/// #TODO Detailed description