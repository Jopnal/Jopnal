// JOPNAL PHYSICS DEBUG VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

// Color from vertex shader
in vec3 vf_Color;

// Final color
out vec4 out_FinalColor;

void main()
{
    out_FinalColor = vec4(vf_Color, 1.0);
}