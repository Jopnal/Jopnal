// JOPNAL DEFAULT FRAGMENT SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/FragmentColor>

// Final color
JOP_COLOR_OUT(0)

void main() 
{ 
    JOP_FRAG_COLOR(0) = vec4(1.0, 0.0, 0.0, 1.0);
}