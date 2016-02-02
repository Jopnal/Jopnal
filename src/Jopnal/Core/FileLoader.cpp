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

        JOP_DEBUG_ERROR("File not found: " << filePath);
        return false;
    }
}