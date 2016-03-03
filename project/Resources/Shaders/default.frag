uniform sampler2D u_DiffuseMap; 

in vec2 out_TexCoords; 
out vec4 out_FinalColor; 

void main() 
{ 
    out_FinalColor = vec4(1.0, 1.0, 1.0, texture2D(u_DiffuseMap, out_TexCoords).r);
}