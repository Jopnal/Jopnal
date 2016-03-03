// JOPNAL DEFAULT VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Matrices
uniform mat4 u_PMatrix; // Projection
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model

// Vertex attributes
layout(location = 0) in vec3 a_Position; 
layout(location = 1) in vec2 a_TexCoords; 

// Texture coordinates to fragment shader
out vec2 vf_TexCoords;

void main() 
{
    // Calculate vertex position 
    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0); 

    // Assign texture coordinates
    vf_TexCoords = a_TexCoords; 
}