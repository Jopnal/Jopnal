// JOPNAL POST PROCESS VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>

JOP_ATTRIBUTE(0) vec3 a_Position;
JOP_ATTRIBUTE(1) vec2 a_TexCoords;

out vec2 vf_TexCoords;

void main()
{
    vf_TexCoords = a_TexCoords;

    gl_Position = vec4(a_Position, 1.0);
}