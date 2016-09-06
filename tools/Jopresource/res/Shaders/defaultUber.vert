// JOPNAL DEFAULT VERTEX UBERSHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>
#include <Jopnal/Compat/Varyings>

// Vertex attributes
JOP_ATTRIBUTE(0) vec3 a_Position;
JOP_ATTRIBUTE(1) vec2 a_TexCoords;
JOP_ATTRIBUTE(2) vec3 a_Normal;
JOP_ATTRIBUTE(3) vec4 a_Color;

// Matrices
uniform mat4 u_PMatrix; // Perspective
uniform mat4 u_VMatrix; // View
//JOP_ATTRIBUTE(4) mat4 a_MMatrix;
uniform mat4 a_MMatrix;
uniform mat3 u_NMatrix;

// Vertex attributes to fragment/geometry shader
JOP_VARYING_OUT vec3 vf_Position;
JOP_VARYING_OUT vec2 vf_TexCoords;
JOP_VARYING_OUT vec3 vf_Normal;
JOP_VARYING_OUT vec4 vf_Color;

void main()
{
    vec4 pos = 
        
    #if !defined(JDRW_SKYBOX) && !defined(JDRW_SKYSPHERE)
        a_MMatrix * 
    #endif
    vec4(a_Position, 1.0);

    // Assign attributes
    vf_Position     = pos.xyz;
    vf_TexCoords    = a_TexCoords;
    vf_Normal       = u_NMatrix * a_Normal;
    vf_Color        = a_Color;

    // Calculate and assign position
    gl_Position = (

        u_PMatrix * 
        
        #if defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE)
            mat4(mat3(u_VMatrix))
        #else
            u_VMatrix
        #endif
        
    * pos)
    
    #if (defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE))
        .xyww
    #endif
    ;
}