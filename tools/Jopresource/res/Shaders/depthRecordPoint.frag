// JOPNAL DEPTH RECORD SHADER
//
// Jopnal license applies

//////////////////////////////////////////////


// Fragment position from geometry shader
in vec3 vgf_FragPosition;

// Light position
uniform vec3 u_LightPosition;

// Far clipping plane
uniform float u_FarClippingPlane;

void main()
{
    // Get the distance between fragment and light source
    float lightDistance = length(vgf_FragPosition - u_LightPosition);

    // Map to [0,1] range
    lightDistance = lightDistance / u_FarClippingPlane;

    // Write as depth
    gl_FragDepth = lightDistance;
}