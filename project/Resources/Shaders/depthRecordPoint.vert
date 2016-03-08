// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Perspective-view-model matrix
uniform mat4 u_PVMMatrix;

// Position attribute
layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = u_PVMMatrix * vec4(a_Position, 1.0);
}