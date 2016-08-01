// JOPNAL PHYSICS DEBUG VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>
#include <Jopnal/Compat/Varyings>

// Matrices
uniform mat4 u_PVMatrix; // Projection-view

// Vertex attributes
JOP_ATTRIBUTE(0) vec3 a_Position;
JOP_ATTRIBUTE(5) vec3 a_Color;

// Color to fragment shader
JOP_VARYING_OUT vec3 vf_Color;

void main()
{
    // Calculate vertex position, the vertices are pre-transformed
    gl_Position = u_PVMatrix * vec4(a_Position, 1.0);

    // Assign color
    vf_Color = a_Color;
}