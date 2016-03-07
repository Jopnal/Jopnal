// JOPNAL DEFAULT FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Diffuse map
uniform sampler2D u_DiffuseMap;

// Emission color
uniform vec3 u_Emission;

// Texture coordinates from vertex shader
in vec2 vf_TexCoords;

// Final color
out vec4 out_FinalColor; 

void main() 
{ 
    out_FinalColor = texture2D(u_DiffuseMap, vf_TexCoords) + vec4(u_Emission, 1.0);
}