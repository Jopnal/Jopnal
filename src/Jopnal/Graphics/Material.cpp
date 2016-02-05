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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{

	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	//////////////////////////////////////////////

	void Material::sendToShader(Shader& shader)
	{
		if (shader.bind())
		{
			shader.setUniform("mat.ambient", m_ambientRef.asFloatVector());
			shader.setUniform("mat.Diffuse", m_diffuseRef.asFloatVector());
			shader.setUniform("mat.specular", m_specularRef.asFloatVector());
			shader.setUniform("mat.shininess", m_shininess);
		}
	}

	//////////////////////////////////////////////

	Material& Material::setAmbient(const Color& color)
	{
		m_ambientRef = color;
		return *this;
	}

	//////////////////////////////////////////////

	const Color& Material::getAmbient()
	{
		return m_ambientRef;
	}

	//////////////////////////////////////////////

	void Material::setDiffuse(const Color& color)
	{
		m_diffuseRef = color;
	}

	//////////////////////////////////////////////

	const Color& Material::getDiffuse()
	{
		return m_diffuseRef;
	}

	//////////////////////////////////////////////

	void Material::setSpecular(const Color& color)
	{
		m_specularRef = color;
	}

	//////////////////////////////////////////////

	const Color& Material::getSpecular()
	{
		return m_specularRef;
	}

	//////////////////////////////////////////////

	void Material::setShininess(const float value)
	{
		m_shininess = value;
	}

	//////////////////////////////////////////////

	const float Material::getShininess()
	{
		return m_shininess;
	}

	//////////////////////////////////////////////



}