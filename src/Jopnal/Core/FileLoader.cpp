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
}