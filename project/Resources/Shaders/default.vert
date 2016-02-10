#version 330

uniform mat4 u_PVMMatrix; 

layout(location = 0)in vec3 a_Position; 
layout(location = 1)in vec2 a_TexCoords; 

out vec2 out_texCoords; 

void main() 
{ 
   gl_Position = u_PVMMatrix * vec4(a_Position, 1.0); 

   out_texCoords = a_TexCoords; 
}