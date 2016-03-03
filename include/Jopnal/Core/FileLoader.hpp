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

#ifndef JOP_FILELOADER_HPP
#define JOP_FILELOADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <string>
#include <vector>

/////////////////////////////////////////////


namespace jop
{
    class JOP_API FileLoader : public Subsystem
    {
    public:

        /// Base directory
        ///
        enum class Directory
        {
            Resources, ///< EXERoot/Resources
            Home       ///< User/Documents/ProjectName
        };

    public:

        /// \brief Constructor
        ///
        /// Initializes the PhysicsFs file system
        ///
        /// \param argv The first element of the char* array passed from main()
        ///
        FileLoader(const char* argv);

        /// \brief Destructor
        ///
        ~FileLoader() override;


        /// \brief Reads data from file into a buffer
        ///
        /// \param filePath Path to the file to load
        /// \param buffer Reference to a data vector
        /// 
        /// \return True if successful
        ///
        static bool read(const std::string& filePath, std::vector<unsigned char>& buffer);

        /// \brief Reads data from file into a string
        ///
        /// \param filePath Path to the file to load
        /// \param buffer Reference to a string
        ///
        /// \return True if successful
        ///
        static bool read(const std::string& filePath, std::string& buffer);

        /// \brief Load a resource from a dll
        ///
        /// This is for internal use only.
        /// Windows: The resource needs be of type RCDATA.
        ///
        static bool readFromDll(const int id, std::vector<unsigned char>& buffer);


        /// \brief Write data into a file
        ///
        /// \param dir The base directory
        /// \param file The file path
        /// \param data The data to write
        /// \param size The size of the data buffer in bytes
        ///
        /// \return True if successful
        ///
        static bool write(const Directory dir, const std::string& file, const void* data, const unsigned int size);

    };
}
#endif

/// \class FileLoader
/// \ingroup core