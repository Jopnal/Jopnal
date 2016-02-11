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

#ifndef JOP_MATERIAL_HPP
#define JOP_MATERIAL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <memory>
#include <array>

//////////////////////////////////////////////


namespace jop
{
	class Shader;
    class Texture;

    class JOP_API Material
    {
    public:

        enum class Reflection
        {
            Ambient,
            Diffuse,
            Specular
        };

        enum class Map
        {
            Diffuse
        };

    public:

        Material();
        

        void sendToShader(Shader& shader) const;


        Material& setReflection(const Reflection reflection, const Color color);

        Material& setReflection(const Color ambient, const Color diffuse, const Color specular);

        Color getReflection(const Reflection reflection) const;

        Material& setShininess(const float value);

		float getShininess() const;


        Material& setMap(const Map map, const Texture& tex);


        static const Material& getDefault();

    private:

        std::array<Color, 3> m_reflection;
        float m_shininess;
        std::array<std::weak_ptr<const Texture>, 1> m_maps;
    };
}
#endif