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

#ifndef JOP_RESOURCEMANAGER_HPP
#define JOP_RESOURCEMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Subsystem.hpp>
#include <unordered_map>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class Resource;

    class JOP_API ResourceManager : public Subsystem
    {
    public:

        /// \brief Default constructor
        ///
        ResourceManager();

        /// \brief Destructor
        ///
        ~ResourceManager() override;


        /// \brief Template function finds resource
        ///
        /// If resource is not found this creates a new one
        ///
        /// \param Name or path for wanted resource
        ///
        template<typename T>
        std::weak_ptr<T> getResource(const std::string& path);

        /// \brief Check if a particular resource exists and is loaded
        ///
        /// \param path Path to resource
        ///
        /// \return True if resource is loaded
        ///
        bool resourceLoaded(const std::string& path);

        /// \brief Deletes resource from memory
        ///
        /// \param Name or path for wanted resource
        ///
        void unloadResource(const std::string& path);

        /// \brief Deletes all resources from memory
        ///
        void unloadResources();

    private:

        std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources; ///< Container holding resources

    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/ResourceManager.inl>
}

#endif

/// \class ResourceManager
/// \ingroup core