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

#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    bool ns_init = false;

#ifdef JOP_OS_WINDOWS

    HINSTANCE ns_resourceDll = nullptr;

#endif
}

namespace jop
{
    FileLoader::FileLoader(const char* argv)
        : Subsystem("File Loader")
    {
        JOP_ASSERT(!ns_init, "There must not be more than one jop::FileLoader!");

        // PhysFS init is called from SettingManager but just to make sure
        if (!PHYSFS_isInit())
            PHYSFS_init(argv);

        static const std::string folder = SettingManager::getString("sResourceDirectory", "Resources");

        PHYSFS_setWriteDir(".");
        PHYSFS_mkdir(folder.c_str());
        
        JOP_ASSERT_EVAL(PHYSFS_mount(folder.c_str(), NULL, 0) != 0, "Failed create/mount the resource directory! Try to create a folder named \"" + folder + "\" in the working directory");
    }

    FileLoader::~FileLoader()
    {
        // SettingManager will deinitialize PhysFS

    #ifdef JOP_OS_WINDOWS
        if (ns_resourceDll)
            FreeLibrary(ns_resourceDll);
    #endif
    }

    //////////////////////////////////////////////

    bool FileLoader::read(const std::string& filePath, std::vector<unsigned char>& buffer)
    {
        if (PHYSFS_exists(filePath.c_str()))
        {
            if (PHYSFS_file* file = PHYSFS_openRead(filePath.c_str()))
            {
                auto size = PHYSFS_fileLength(file);

                if (size > 0)
                {
                    buffer.resize(static_cast<std::size_t>(size));

                    if (PHYSFS_read(file, buffer.data(), 1, static_cast<PHYSFS_uint32>(size)) != size)
                        JOP_DEBUG_WARNING("Amount of bytes read from file not matched with size: " << filePath);

                    if (!PHYSFS_close(file))
                        JOP_DEBUG_ERROR("Couldn't close file: " << filePath);

                    return true;
                }

                JOP_DEBUG_ERROR("Couldn't determine file size: " << filePath);
                return false;
            }
        }

        return false;
    }

    //////////////////////////////////////////////

    bool FileLoader::read(const std::string& filePath, std::string& buffer)
    {
        std::vector<unsigned char> buf;
        if (!read(filePath, buf))
            return false;

        buf.push_back(static_cast<unsigned char>('\0'));
        buffer = reinterpret_cast<const char*>(buf.data());

        return true;
    }

    //////////////////////////////////////////////

    bool FileLoader::readFromDll(const int id, std::vector<unsigned char>& buffer)
    {
        if (!ns_resourceDll)
        {
            ns_resourceDll = LoadLibrary("Jopnal Resources.dll");
            JOP_ASSERT(ns_resourceDll != nullptr, "Failed to load the Jopnal resource dll!");
        }

        bool success = false;

        auto handle = FindResource(ns_resourceDll, MAKEINTRESOURCE(id), RT_RCDATA);

        if (handle)
        {
            auto res = LoadResource(ns_resourceDll, handle);

            if (res)
            {
                auto size = SizeofResource(ns_resourceDll, handle);
                auto locked = LockResource(res);

                if (locked && size)
                {
                    buffer.resize(size);
                    std::memcpy(buffer.data(), locked, size);
                    success = true;
                }

                UnlockResource(locked);
            }

            FreeResource(res);
        }

        return success;
    }

    //////////////////////////////////////////////

    bool FileLoader::write(const Directory dir, const std::string& file, const void* data, const unsigned int size)
    {
        static const std::string resFolder = SettingManager::getString("sResourceDirectory", "Resources");
        static const std::string docDir("Documents");

        std::string writeDir;

        if (dir == Directory::Resources)
            writeDir = resFolder;
        else
        {
            writeDir = PHYSFS_getUserDir();
            writeDir += docDir + PHYSFS_getDirSeparator() + getProjectName();
        }

        auto pos = file.find_last_of("/\\");

        if (!PHYSFS_setWriteDir(writeDir.c_str()) || !PHYSFS_mkdir(file.substr(0, pos).c_str()))
        {
            JOP_DEBUG_ERROR("Couldn't write file, failed to create directory: " << file);
            return false;
        }

        if (pos != std::string::npos)
        {
            writeDir += PHYSFS_getDirSeparator() + file.substr(0, pos);

            if (!PHYSFS_setWriteDir(writeDir.c_str()))
            {
                JOP_DEBUG_ERROR("Couldn't write file, failed to set write directory: " << file);
                return false;
            }
        }

        if (data && size)
        {
            if (PHYSFS_file* fileHandle = PHYSFS_openWrite(pos != std::string::npos ? file.substr(pos + 1).c_str() : file.c_str()))
            {
                if (PHYSFS_write(fileHandle, data, 1, size) != size)
                    JOP_DEBUG_WARNING("Failed to write the specified amount of bytes: " << file);

                if (!PHYSFS_close(fileHandle))
                {
                    JOP_DEBUG_ERROR("Couldn't close file handle: " << file);
                    return false;
                }

                return true;
            }

            JOP_DEBUG_ERROR("Failed to write file, couldn't open for writing: " << file);
        }

        return false;
    }
}