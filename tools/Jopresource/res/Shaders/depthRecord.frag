// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////

#ifdef JOP_PACK_DEPTH

	varying vec4 vf_Position;

#endif

void main()
{
    // Depth recording happens automatically, unless depth textures are unsupported,
	// in which case we'll need to pack the texture into a regular RGBA texture.

#ifdef JOP_PACK_DEPTH
		
	const vec4 bitShift = vec4
	(
		256.0 * 256.0 * 256.0,
        256.0 * 256.0,
        256.0,
        1.0
	);
    const vec4 bitMask = vec4
	(
		0,
        1.0 / 256.0,
        1.0 / 256.0,
        1.0 / 256.0
	);

	vec4 comp = fract((vPosition.z / vPosition.w + 1.0) * 0.5 * bitShift);
	comp -= comp.xxyz * bitMask;
	
	gl_FragColor = comp;

#endif
}