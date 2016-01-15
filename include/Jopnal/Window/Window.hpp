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

#ifndef JOP_WINDOW_HPP
#define JOP_WINDOW_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <memory>

//////////////////////////////////////////////


class GLFWwindow;

namespace jop
{
    namespace detail
    {
        class WindowImpl;
    }

    class Window
    {
    public:

        JOP_DISALLOW_COPY(Window);

        enum class DisplayMode
        {
            Windowed,
            Borderless,
            Fullscreen
        };

        struct Settings
        {
            glm::u32vec2 size;
            std::string title;
            DisplayMode displayMode;
            bool visible;
        };

    public:

        Window();

        Window(const Settings& settings);

        Window(Window&& other);

        Window& operator =(Window&& other);


        void open(const Settings& settings);

        void close();

        bool isOpen() const;

        GLFWwindow* getLibraryHandle();

        static void pollEvents();

    private:

        std::unique_ptr<detail::WindowImpl> m_impl;

    };
}

#endif