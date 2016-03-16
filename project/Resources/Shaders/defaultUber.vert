// JOPNAL DEFAULT VERTEX UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Vertex attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normal;

// Matrices
#ifndef JMAT_ENVIRONMENT_RECORD
    uniform mat4 u_PMatrix; // Perspective
    uniform mat4 u_VMatrix; // View
#endif
uniform mat4 u_MMatrix; // Model
uniform mat3 u_NMatrix; // Normal (is transpose(inverse(u_MMatrix)))

// Vertex attributes to fragment shader
#ifdef JMAT_ENVIRONMENTMAP
    out vec3 vf_Position;
#endif
out vec2 vf_TexCoords;
out vec3 vf_Normal;

// Fragment position to fragment shader
// If this shader used to record an environment map, geometry shader will
// take care if the fragment position
#ifndef JMAT_ENVIRONMENT_RECORD
    out vec3 vgf_FragPosition;
#endif

void main()
{
    // Assign attributes
    #ifdef JMAT_ENVIRONMENTMAP
        vf_Position = vec3(u_MMatrix * vec4(a_Position, 1.0));
    #endif
    vf_TexCoords = a_TexCoords;
    vf_Normal = u_NMatrix * a_Normal;

    // Calculate and assign fragment position, not used when recording environment map
    #ifndef JMAT_ENVIRONMENT_RECORD
        vgf_FragPosition = vec3(u_MMatrix * vec4(a_Position, 1.0));
    #endif

    // Calculate and assign position
    gl_Position =

    #ifndef JMAT_ENVIRONMENT_RECORD
        u_PMatrix * u_VMatrix *
    #endif
        
    u_MMatrix * vec4(a_Position, 1.0);
}