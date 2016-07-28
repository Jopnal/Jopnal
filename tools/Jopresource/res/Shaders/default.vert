// JOPNAL DEFAULT VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>

// Matrices
uniform mat4 u_PMatrix; // Projection
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model

// Vertex attributes
JOP_ATTRIBUTE(0) vec3 a_Position;

void main() 
{
    // Calculate vertex position 
    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0);
}