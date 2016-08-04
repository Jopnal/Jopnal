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
struct AAsset;

namespace jop
{
    namespace detail
    {
        class FileSystemInitializer final : public Subsystem
        {
            JOP_DISALLOW_COPY_MOVE(FileSystemInitializer);

            friend class ModelLoader;

        public:

            FileSystemInitializer(const char* arg);

            ~FileSystemInitializer();
        };
    }

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
            Resource,   ///< Resource folder. On Android this is the memory card directory
            User        ///< User folder. On Android this is the same directory as Executable
        };

    public:

        /// \brief Default constructor
        ///
        /// Doesn't initialize any file handles.
        ///
        FileLoader();

        /// \brief Overloaded constructor
        ///
        /// This will open the file for reading if found
        ///
        /// \param path Path to file to open
        ///
        /// \see isValid()
        ///
        explicit FileLoader(const std::string& path);

        /// \brief Overloaded constructor
        ///
        /// This will open the file for writing if found.
        ///
        /// \param dir Base write directory
        /// \param path Path to file to open
        /// \param append Append to the file. False to clear the file before writing
        ///
        /// \see isValid()
        /// 
        FileLoader(const Directory dir, const std::string& path, const bool append);

        /// \brief Move constructor
        ///
        FileLoader(FileLoader&& other);

        /// \brief Move assignment operator
        ///
        /// \return Reference to self
        ///
        FileLoader& operator =(FileLoader&& other);

        /// \brief Destructor
        ///
        /// Will close the file handle if open.
        ///
        ~FileLoader();


        /// \brief Open a file for reading
        ///
        /// \param path Path to the file
        ///
        /// \return True if opened successfully
        ///
        bool open(const std::string& path);

        /// \brief Open a file for writing
        ///
        /// \copydetails FileLoader(const Directory, const std::string&, const bool)
        ///
        /// \return True if opened successfully
        ///
        bool open(const Directory dir, const std::string& path, const bool append);

        /// \brief Flush the file stream
        ///
        /// This function only has an effect when writing to a file. Once called
        /// all buffered file output will be flushed and written to disk.
        ///
        /// If no file handle is open, this will have no effect.
        ///
        void flush();

        /// \brief Close the file handle if open
        ///
        /// If no file handle is open, this will have no effect.
        ///
        /// When writing, calling this means saving the file.
        ///
        void close();

        /// \brief Check if a file handle is open
        ///
        /// \return True if a valid file handle exists
        ///
        bool isValid() const;

        /// \brief Read data
        ///
        /// \param data Pointer to a pre-allocated data buffer
        /// \param size Amount of data to read
        ///
        /// \return Amount of data read in bytes
        ///
        /// \see getSize()
        ///
        int64 read(void* data, const uint64 size);

        /// \brief Write data
        ///
        /// \param data Data to write
        /// \param size Amount of data to write in bytes
        ///
        /// \return Amount of data written in bytes
        ///
        int64 write(const void* data, const uint64 size);

        /// \brief Move the file cursor to the given position
        ///
        /// \param position The cursor position to set in bytes
        ///
        /// \return True if successful
        ///
        bool seek(const uint64 position);

        /// \brief Get the current position of the cursor
        ///
        /// \return Current position of the file read/write cursor
        ///
        int64 tell() const;

        /// \brief Get the size of the opened file
        ///
        /// \return Size of the file
        ///
        int64 getSize() const;

        /// \copydoc isValid()
        ///
        operator bool() const;

        /// \brief Check if a file exists
        ///
        /// \param path Path to the file to check
        ///
        /// \return True if the file exists
        ///
        static bool fileExists(const std::string& path);

        /// \brief Enumerate all files within a directory
        ///
        /// \param path Path to a directory
        /// \param list Reference to a list to fill with the file paths found
        ///
        /// \see listFilesRecursive
        ///
        static void listFiles(const std::string& path, std::vector<std::string>& list);

        /// \brief Enumerate all files within a directory recursively
        ///
        /// \warning On Android, files cannot be enumerated recursively from the .apk,
        ///
        /// \param path Path to a directory
        /// \param list Reference to a list to fill with the file paths found
        ///
        /// \see listFiles
        ///
        static void listFilesRecursive(const std::string& path, std::vector<std::string>& list);

        /// \brief Delete a file
        ///
        /// \param file Path to the file
        ///
        /// \return True if file was deleted
        ///
        static bool deleteFile(const Directory dir, const std::string& file);

        /// \brief Read a text file
        ///
        /// \param path Path to the file to read
        /// \param file Reference to a string to fill with the data
        ///
        /// \return True if successful
        ///
        static bool readTextfile(const std::string& path, std::string& file);

        /// \brief Read a binary file
        ///
        /// \param path Path to the file to read
        /// \param buffer Reference to a buffer to fill with the data
        ///
        /// \return True if successful
        ///
        static bool readBinaryfile(const std::string& path, std::vector<uint8>& buffer);

        /// \brief Write a text file
        ///
        /// \param dir The base write directory
        /// \param path The file path
        /// \param text The text to write
        /// \param append Append to file?
        ///
        /// \return True if successful
        ///
        static bool writeTextfile(const Directory dir, const std::string& path, const std::string& text, const bool append = false);

        /// \brief Write a binary file
        ///
        /// \param dir The base write directory
        /// \param path The file path
        /// \param data The binary data to write
        /// \param bytes amount of bytes to write
        /// \param append Append to file?
        ///
        /// \return True if successful
        ///
        static bool writeBinaryfile(const Directory dir, const std::string& path, const void* data, const std::size_t bytes, const bool append = false);

        /// \brief Create a directory
        ///
        /// If the directory already exists, this has no effect.
        ///
        /// \param dir The base write directory
        /// \param path The directory to create
        ///
        /// \return True if successful
        ///
        static bool makeDirectory(const Directory dir, const std::string& path);

        /// \brief Get a base directory as string
        ///
        /// \param dir The base directory
        ///
        /// \return Internal reference to the directory string
        ///
        static const std::string& getDirectory(const Directory dir);

        /// \brief Get the OS-specific directory separator
        ///
        /// \return The directory separator
        ///
        static char getDirectorySeparator();

        /// \brief Enable/disable file system error checks
        ///
        /// \param enable True to enable
        ///
        static void enableErrorChecks(const bool enable);

        /// \brief Check if file system error checks are enabled
        ///
        /// \return True if enabled
        ///
        static bool errorChecksEnabled();

    private:

        union
        {
            PHYSFS_File* m_file;    ///< File handle
            AAsset* m_asset;        ///< Android asset handle
        };
        bool m_isAsset;             ///< Is the current open file an Android asset?
    };
}

#endif

/// \class jop::FileLoader
/// \ingroup core