// JOPNAL DEFAULT FRAGMENT UBERSHADER

//////////////////////////////////////////////

// Diffuse map
#ifdef JMAT_DIFFUSEMAP
    uniform sampler2D u_DiffuseMap;
#endif

// Specular map
#ifdef JMAT_SPECULARMAP
    uniform sampler2D u_SpecularMap;
#endif

// Surface material
#ifdef JMAT_MATERIAL
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    uniform Material u_Material;

// If material is not used, the have a solid color
#else
    uniform vec3 u_SolidColor;
#endif

// Texture coordinates from vertex shader
in vec2 out_TexCoords;

// Final fragment color
out vec4 final_Color; 

void main() 
{
    vec4 tempColor;

    // Sample textures
#ifdef JMAT_DIFFUSEMAP
    tempColor = texture2D(u_DiffuseMap, out_TexCoords);
#endif

    
#ifdef JMAT_MATERIAL
    
#else
    tempColor *= vec4(u_SolidColor.xyz, 1.0);
#endif

    final_Color = tempColor;
}