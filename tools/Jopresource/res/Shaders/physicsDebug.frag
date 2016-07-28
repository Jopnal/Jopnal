// JOPNAL PHYSICS DEBUG VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>
#include <Jopnal/Compat/Varyings>

// Color from vertex shader
JOP_VARYING_IN vec3 vf_Color;

// Final color
JOP_COLOR_OUT(0)

void main()
{
    JOP_FRAG_COLOR(0) = vec4(vf_Color, 1.0);
}