#version 330 

uniform sampler2D tex; 

in vec2 out_texCoords; 
out vec4 final_Color; 

void main() 
{ 
   final_Color = texture2D(tex, out_texCoords); 
}