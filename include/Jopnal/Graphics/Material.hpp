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
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
	class Shader;

    class JOP_API Material : public Resource
    {
    public:
        Material();
        ~Material();

        void sendToShader(Shader& shader);

        Material& setAmbient(const Color& color);

		const Color& getAmbient();

        void setDiffuse(const Color& color);

		const Color& getDiffuse();

        void setSpecular(const Color& color);

		const Color& getSpecular();

        void setShininess(const float value);

		const float getShininess();


    private:

        Color m_ambientRef;
        Color m_diffuseRef;
        Color m_specularRef;
        float m_shininess;
    };
}
#endif