// JOPNAL PHYSICS DEBUG VERTEX SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>

// Color from vertex shader
in vec3 vf_Color;

// Final color
JOP_COLOR_OUT(0)

void main()
{
    JOP_FRAG_COLOR(0) = vec4(vf_Color, 1.0);
}