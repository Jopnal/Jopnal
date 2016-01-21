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


template<typename T, typename ... Args>
T& Engine::createScene(Args&... args)
{
    static_assert(std::is_base_of<Scene, T>::value, "jop::Engine::createScene(): Attempted to create an object which is not derived from jop::Scene");

    m_currentScene = std::make_unique<T>(args...);
    return static_cast<T&>(*m_currentScene);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Engine::createSubsystem(Args&... args)
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::createSubsystem(): Attempted to create an object which is not derived from jop::Subsystem");

    m_subsystems.emplace_back(std::make_unique<T>(args...));
    return static_cast<T&>(*m_subsystems.back());
}