// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>

// Perspective-view (light space) matrix
uniform mat4 u_PVMatrix;

// Model matrix
uniform mat4 u_MMatrix;

// Position attribute
JOP_ATTRIBUTE(0) vec3 a_Position;

void main()
{
    gl_Position = u_PVMatrix * u_MMatrix * vec4(a_Position, 1.0);
}