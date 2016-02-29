// JOPNAL DEFAULT VERTEX UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Vertex attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normal;

// Matrices
uniform mat4 u_PMatrix; // Perspective
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model
uniform mat3 u_NMatrix; // Normal (is transpose(inverse(u_MMatrix)))

// Vertex attributes to fragment shader
out vec2 vf_TexCoords;
out vec3 vf_Normal;

// Fragment position to fragment shader
out vec3 vf_FragPosition;

void main()
{
    // Assign attributes
    vf_TexCoords = a_TexCoords;
    vf_Normal = u_NMatrix * a_Normal;

    // Calculate and assign fragment position
    vf_FragPosition = vec3(u_MMatrix * vec4(a_Position, 1.0));

    // Calculate and assign position
    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0);
}