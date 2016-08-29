// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>

// Model matrix
JOP_ATTRIBUTE(4) mat4 a_MMatrix;

// Position attribute
JOP_ATTRIBUTE(0) vec3 a_Position;

void main()
{
    gl_Position = a_MMatrix * vec4(a_Position, 1.0);
}