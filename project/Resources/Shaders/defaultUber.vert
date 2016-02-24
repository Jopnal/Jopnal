// JOPNAL DEFAULT VERTEX UBERSHADER

//////////////////////////////////////////////

// Perspective-view-model matrix
uniform mat4 u_PVMMatrix;

// Vertex attributes
layout(location = 0)in vec3 a_Position; 
layout(location = 1)in vec2 a_TexCoords;
layout(location = 2)in vec3 a_Normal;

// Texture coordinates to fragment shader
out vec2 out_TexCoords;

void main()
{
    gl_Position = u_PVMMatrix * vec4(a_Position, 1.0);

    out_TexCoords = a_TexCoords;
}