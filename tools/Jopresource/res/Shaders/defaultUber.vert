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
JOP_ATTRIBUTE(5) vec4 a_Color;

// Matrices
#ifndef JMAT_ENVIRONMENT_RECORD
    uniform mat4 u_PMatrix; // Perspective
    uniform mat4 u_VMatrix; // View
#endif
uniform mat4 u_MMatrix; // Model
uniform mat3 u_NMatrix; // Normal (is transpose(inverse(u_MMatrix)))

// Vertex attributes to fragment/geometry shader
#ifdef GL_ES

    JOP_VARYING_OUT vec3 var_Position;
    JOP_VARYING_OUT vec2 var_TexCoords;
    JOP_VARYING_OUT vec3 var_Normal;
    JOP_VARYING_OUT vec4 var_Color;

    #define OUT_POS var_Position
    #define OUT_TC    var_TexCoords
    #define OUT_NOR var_Normal
    #define OUT_COL    var_Color

#else

    #ifdef JMAT_ENVIRONMENT_RECORD
        #define OUTVERT_NAME inVert
        out VertexData
    #else
        #define OUTVERT_NAME outVert
        out FragVertexData
    #endif
    {
        vec3 Position;
        vec2 TexCoords;
        vec3 Normal;
        vec4 Color;
    
    } OUTVERT_NAME;

    #define OUT_POS OUTVERT_NAME .Position
    #define OUT_TC    OUTVERT_NAME .TexCoords
    #define OUT_NOR OUTVERT_NAME .Normal
    #define OUT_COL    OUTVERT_NAME .Color

#endif

// Fragment position to fragment shader
// If this shader used to record an environment map, geometry shader will
// take care of the fragment position
#ifndef JMAT_ENVIRONMENT_RECORD
    JOP_VARYING_OUT vec3 vgf_FragPosition;
#endif

void main()
{
    vec4 pos = 
        
    #if !defined(JDRW_SKYBOX) && !defined(JDRW_SKYSPHERE)
        u_MMatrix * 
    #endif
        vec4(a_Position, 1.0);

    // Assign attributes
    OUT_POS = pos.xyz;
    OUT_TC = a_TexCoords;
    OUT_NOR = u_NMatrix * a_Normal;
    OUT_COL = a_Color;

    // Calculate and assign fragment position, not used when recording environment map
    #ifndef JMAT_ENVIRONMENT_RECORD
        vgf_FragPosition = pos.xyz;
    #endif

    // Calculate and assign position
    gl_Position = (

    #ifndef JMAT_ENVIRONMENT_RECORD

        u_PMatrix * 
        
        #if defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE)
            mat4(mat3(u_VMatrix))
        #else
            u_VMatrix
        #endif
        *

    #endif
        
    pos)
    
    #if (defined(JDRW_SKYBOX) || defined(JDRW_SKYSPHERE)) && !defined(JMAT_ENVIRONMENT_RECORD)
        .xyww
    #endif
    ;
}