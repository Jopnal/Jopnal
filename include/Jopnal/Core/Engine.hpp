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

#ifndef JOP_ENGINE_HPP
#define JOP_ENGINE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <memory>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Engine
    {
    public:

        /// \brief Constructor
        ///
        /// This will initialize the engine and all associated systems.
        /// Project name must not be empty, as it will be used to create
        /// the necessary config directories.
        ///
        /// \param projectName The project name
        ///
        Engine(const std::string& projectName);

        /// \brief Destructor
        ///
        /// This will deinitialize the engine, so that the program can exit.
        /// Alternatively, a new jop::Engine object can be created.
        ///
        ~Engine();


        /// \brief Run the main loop
        ///
        /// The main loop will run until exit() is called.
        ///
        /// \return An integer return value. To be used as the main() return value
        ///
        int runMainLoop();

        /// \brief Create a scene
        ///
        /// \param args The arguments to be used in the scene's construction
        ///
        /// \return A reference to the newly created scene
        ///
        template<typename T, typename ... Args>
        T& createScene(Args... args);

        /// \brief Exit the main loop
        ///
        /// This function will have no effect if no jop::Engine exits at the
        /// time of the call. The main loop returns only after the current
        /// frame has been processed.
        ///
        static void exit();

    private:

        //std::vector<std::unique_ptr<Scene>> m_scenes;
        bool m_running;

    };

    /// \brief Get the project name
    ///
    /// \return A reference to the internal string containing the project name
    ///
    JOP_API const std::string& getProjectName();

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Engine.inl>
}

#endif

/// \class Engine
/// \ingroup core
///
/// #TODO Detailed description