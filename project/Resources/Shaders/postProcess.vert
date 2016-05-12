// JOPNAL POST PROCESS VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;



out vec2 vf_TexCoords;

void main()
{
    vf_TexCoords = a_TexCoords;

    gl_Position = vec4(a_Position, 1.0);
}