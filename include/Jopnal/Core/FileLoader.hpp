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

#ifndef GP_FILELOADER_HPP
#define GP_FILELOADER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Subsystem.hpp>
#include <string>
#include <vector>

//////////////////////////////////////////////


struct PHYSFS_File;
namespace Assimp
{
    class Importer;
}

namespace jop
{
    class JOP_API FileSystemInitializer final : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(FileSystemInitializer);

        friend class ModelLoader;

        static std::unique_ptr<Assimp::Importer> g_Importer;

    public:

        FileSystemInitializer(const char* arg);

        ~FileSystemInitializer();
    };

    class JOP_API FileLoader
    {
    private:

        JOP_DISALLOW_COPY(FileLoader);

    public:

        /// Base directory for writing files
        ///
        enum class Directory
        {
            Executable, ///< Executable directory
            Resource,   ///< Resource folder
            User        ///< User folder
        };

    public:

        FileLoader();

        explicit FileLoader(const std::string& path);

        FileLoader(const Directory dir, const std::string& path, const bool append);

        FileLoader(FileLoader&& other);

        FileLoader& operator =(FileLoader&& other);

        ~FileLoader();


        bool open(const std::string& path);

        bool openWrite(const Directory dir, const std::string& path, const bool append);

        void flush();

        void close();

        bool isValid() const;

        int64 read(void* data, const uint64 size);

        int64 write(const void* data, const uint64 size);

        bool seek(const uint64 position);

        int64 tell();

        int64 getSize();

        operator bool() const;


        static bool fileExists(const std::string& path);

        static void listFiles(const std::string& path, std::vector<std::string>& list);

        static void listFilesRecursive(const std::string& path, std::vector<std::string>& list);

        static bool deleteFile(const std::string& file);

        static bool readTextfile(const std::string& path, std::string& file);

        static bool readBinaryfile(const std::string& path, std::vector<uint8>& buffer);

        static bool writeTextfile(const Directory dir, const std::string& path, const std::string& text, const bool append = false);

        static bool writeBinaryfile(const Directory dir, const std::string& path, const void* data, const std::size_t bytes, const bool append = false);

        static bool makeDirectory(const std::string& path);

        static const std::string& getDirectory(const Directory dir);

        static char getDirectorySeparator();

        static bool readResource(const int id, std::vector<uint8>& buffer);

        static void enableErrorChecks(const bool enable);

        static bool errorChecksEnabled();

    private:

        PHYSFS_File* m_file;    ///< File handle
    };
}

#endif