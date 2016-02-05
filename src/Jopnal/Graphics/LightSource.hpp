

///////////////////////////////////////////

#ifndef JOP_LIGHTSOURCE_HPP
#define JOP_LIGHTSOURCE_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <glm/vec3.hpp>
#include <Jopnal/MathInclude.hpp>
#include <string>

///////////////////////////////////////////


namespace jop
{
    class JOP_API LightSource : public jop::Component
    {
    protected:
         glm::vec3 diffuse;
         glm::vec3 ambient;
         glm::vec3 specular;
         float shininess;
         glm::mat4 modelViewMatrix;
         glm::mat3 normalMatrix;
         glm::mat4 mvp;
        



    public:
        LightSource();

    LightSource& lightDirection();

    LightSource& lightPoint();
   
    LightSource& spotLight();

        

    };
}

#endif