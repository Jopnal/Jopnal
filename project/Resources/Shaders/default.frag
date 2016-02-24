uniform sampler2D u_DiffuseMap; 

in vec2 out_TexCoords; 
out vec4 final_Color; 

void main() 
{ 
   final_Color = texture2D(u_DiffuseMap, out_TexCoords);
}