// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////


// Model matrix
uniform mat4 u_MMatrix;

// Position attribute
layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = u_MMatrix * vec4(a_Position, 1.0);
}