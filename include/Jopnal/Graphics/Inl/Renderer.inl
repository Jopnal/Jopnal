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


template<typename T, typename ... Args>
T& Renderer::createRenderPass(const RenderPass::Pass pass, const uint32 weight, Args&&... args)
{
    auto ptr = std::make_unique<T>(*this, m_target, pass, weight, std::forward<Args>(args)...);
    auto tempPtr = ptr.get();

    m_passes[static_cast<int>(pass)][weight] = std::move(ptr);

    return *tempPtr;
}

template<typename T>
T* Renderer::getRenderPass(const RenderPass::Pass pass, const uint32 weight)
{
    auto itr = m_passes[static_cast<int>(pass)].find(weight);

    if (itr != m_passes[static_cast<int>(pass)].end())
        return itr->second.get();

    return nullptr;
}