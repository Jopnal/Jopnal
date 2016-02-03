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
#include <Jopnal/Utility/Message.hpp>
#include <memory>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class Subsystem;
    class Scene;

    class JOP_API Engine final
    {
    public:

        /// \brief Constructor
        ///
        /// This will initialize the engine and all associated systems.
        /// Project name must not be empty, as it will be used to create
        /// the necessary config directories.
        ///
        /// \param projectName The project name
        /// \param argc Number of arguments passed from main()
        /// \param argv The argument array passed from main()
        ///
        Engine(const std::string& projectName, int argc, char* argv[]);

        /// \brief Destructor
        ///
        /// This will deinitialize the engine, so that the program can exit.
        /// Alternatively, a new jop::Engine object can be created.
        ///
        ~Engine();


        /// \brief Load the default subsystem configuration
        ///
        void loadDefaultConfiguration();

        /// \brief Run the main loop
        ///
        /// The main loop will run until exit() is called.
        ///
        /// \return An integer return value. To be used as the main() return value
        ///
        int runMainLoop();


        /// \brief Create a scene
        ///
        /// This function will construct the scene and then set it as active.
        /// The previously active scene will be discarded.
        ///
        /// #TODO: Rethink the scene system so that it becomes more flexible (threaded loading?)
        ///
        /// \param args The arguments to be used in the scene's construction
        ///
        /// \return A reference to the newly created scene
        ///
        template<typename T, typename ... Args>
        T& createScene(Args&... args);

        /// \brief Get the current scene
        ///
        /// This function asserts that the scene has been loaded first with createScene().
        ///
        /// \return Reference to the scene
        ///
        static Scene& getCurrentScene();


        /// \brief Create a subsystem
        ///
        /// \param args The arguments to be used in the subsystem's construction
        ///
        /// \return A reference to the newly created subsystem
        ///
        template<typename T, typename ... Args>
        T& createSubsystem(Args&... args);

        /// \brief Get a subsystem using type info
        ///
        /// \return Pointer to the subsystem. Nullptr if not found
        ///
        template<typename T>
        T* getSubsystem();

        /// \brief Get a subsystem
        ///
        /// \param ID Identifier of the subsystem
        ///
        /// \return Pointer to the subsystem. Nullptr if not found
        ///
        Subsystem* getSubsystem(const std::string& ID);

        /// \brief Remove a subsystem
        ///
        /// \param ID Identifier of the subsystem to be removed
        ///
        bool removeSubsystem(const std::string& ID);


        /// \brief Check if the engine is running
        ///
        /// \return True if an engine object exists & m_running is true
        ///
        static bool isRunning();

        /// \brief Exit the main loop
        ///
        /// This function will have no effect if no jop::Engine exits at the
        /// time of the call. The main loop returns only after the current
        /// frame has been processed.
        ///
        static void exit();


        /// \brief Send a message to the whole engine
        ///
        /// \param message String holding message
        /// \param ptr Pointer to hold extra data
        ///
        static MessageResult sendMessage(const std::string& message, Any returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        static MessageResult sendMessage(const Message& message);


        /// \brief Get the shared scene
        ///
        /// The shared scene exists for the purpose of being able to have layers & objects
        /// that are shared between scenes. This makes it possible to have general-purpose
        /// functionality without having to take care of it in every scene separately.
        ///
        /// The existence of the shared scene will not be checked. Only
        /// call this function when you know there exists a valid jop::Engine
        /// object. If necessary, you can check by using jop::Engine::isRunning().
        ///
        /// \return Reference to the shared scene
        ///
        static Scene& getSharedScene();

        /// \brief Set the shared scene
        ///
        /// This will replace the previous shared scene with a new one of the given type.
        /// This function is not static, as you're encouraged to set the shared scene
        /// in the very beginning of the program.
        ///
        /// \param args Arguments to use in the scene's construction
        ///
        /// \return Reference to the new scene
        ///
        template<typename T, typename ... Args>
        T& setSharedScene(Args&... args);


    private:

        std::vector<std::unique_ptr<Subsystem>> m_subsystems; ///< A vector containing the subsystems
        std::unique_ptr<Scene> m_currentScene;                ///< The current scene
        std::unique_ptr<Scene> m_sharedScene;                 ///< The shared scene
        bool m_running;                                       ///< A boolean telling if the engine is running

    };

    /// \brief Get the project name
    ///
    /// \return A reference to the internal string containing the project name
    ///
    JOP_API const std::string& getProjectName();

    /// \brief Broadcast a message to the whole engine
    ///
    /// This is the same as calling jop::Engine::sendMessage
    ///
    /// \param message String holding message
    /// \param ptr Pointer to hold extra data
    ///
    JOP_API MessageResult broadcast(const std::string& message, Any returnWrap);

    /// \brief Broadcast a message to the whole engine
    ///
    /// This is the same as calling jop::Engine::sendMessage
    ///
    /// \param message The message
    ///
    JOP_API MessageResult broadcast(const Message& message);

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Engine.inl>
}

#endif

/// \class Engine
/// \ingroup core
///
/// #TODO Detailed description