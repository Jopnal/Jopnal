// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#include <Jopnal/Compat/Attributes>

// Perspective-view (light space) matrix
uniform mat4 u_PVMatrix;

// Model matrix
uniform mat4 a_MMatrix;

// Position attribute
JOP_ATTRIBUTE(0) vec3 a_Position;

// No need for compatibility layer, this will only happen
// on GLES 2.0
#ifdef JOP_PACK_DEPTH
	varying vec4 vf_Position;
#endif

void main()
{
    gl_Position = u_PVMatrix * a_MMatrix * vec4(a_Position, 1.0);

	#ifdef JOP_PACK_DEPTH
		vf_Position = gl_Position;
	#endif
}