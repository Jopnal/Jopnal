

///////////////////////////////////////////

#ifndef JOP_LIGHTSOURCE_HPP
#define JOP_LIGHTSOURCE_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <glm/vec3.hpp>
#include <string>

///////////////////////////////////////////


namespace jop
{
    class JOP_API LightSource : public jop::Component
    {
    private:


    public:
        LightSource();

        void lightDirection();

        void lightPoint();

        void spotLight();

        

    };
}

#endif