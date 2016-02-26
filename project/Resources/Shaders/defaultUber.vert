// JOPNAL DEFAULT VERTEX UBERSHADER

//////////////////////////////////////////////

// Vertex attributes
layout(location = 0)in vec3 a_Position;
layout(location = 1)in vec2 a_TexCoords;
layout(location = 2)in vec3 a_Normal;

// Matrices
uniform mat4 u_PMatrix; // Perspective
uniform mat4 u_VMatrix; // View
uniform mat4 u_MMatrix; // Model

#ifdef JMAT_PHONG
    uniform mat3 u_NMatrix; // Normal
#endif

// Ambient constant
#ifdef JMAT_AMBIENT
    uniform vec3 u_AmbientLight;
#endif

// Surface material
#ifdef JMAT_MATERIAL
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    uniform Material u_Material;

    out vec3 out_MatAmbient;
    out vec3 out_MatDiffuse;
    out vec3 out_MatSpecular;
    out float out_MatShininess;

// If material is not used, then have a solid color
#else
    uniform vec3 u_SolidColor;
    out vec3 out_SolidColor;
#endif

// Light info
#ifdef JMAT_PHONG
    // Point lights
    struct PointLightInfo
    {
        // Position
        vec3 position;

        // Intensities
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        // Attenuation
        // x = constant
        // y = linear
        // z = quadratic
        vec3 attenuation;
    };
    uniform PointLightInfo u_PointLights[JMAT_MAX_POINT_LIGHTS];
    uniform uint u_NumPointLights;

    // Directional lights
    struct DirectionalLightInfo
    {
        // Direction
        vec3 direction;

        // Intensities
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        // No attenuation for directional lights
    };
    uniform DirectionalLightInfo u_DirectionalLights[JMAT_MAX_DIRECTIONAL_LIGHTS];
    uniform uint u_NumDirectionalLights;

    // Light calculation functions
    vec3 calculatePoint(const in uint index)
    {
        PointLightInfo l = u_PointLights[index];

        vec3 tnorm = normalize(u_NMatrix * a_Normal);
        vec3 eyeCoords = (u_VMatrix * u_MMatrix * vec4(a_Position, 1.0)).xyz;
        vec3 s = normalize(l.position - eyeCoords);
        vec3 v = normalize(-eyeCoords);
        vec3 r = reflect(-s, tnorm);

        // Ambient
        vec3 ambient =
        #ifdef JMAT_MATERIAL
            l.ambient * u_Material.ambient;
        #else
            l.ambient;
        #endif

        // Diffuse
        float sDotN = max(dot(s, tnorm), 0.0);
        vec3 diffuse =
        #ifdef JMAT_MATERIAL
            l.diffuse * u_Material.diffuse * sDotN;
        #else
            l.diffuse * sDotN;
        #endif

        // Specular
        vec3 specular = vec3(0.0);
        if (sDotN > 0.0)
            #ifdef JMAT_MATERIAL
                specular = l.specular * u_Material.specular * pow(max(dot(r, v), 0.0), u_Material.shininess);
            #else
                specular = l.specular * max(dot(r, v), 0.0);
            #endif

        // Attenuation
        float dist = length(l.position - a_Position);
        float attenuation = 1.0f / (l.attenuation.x + l.attenuation.y * dist + l.attenuation.z * (dist * dist));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        return ambient + diffuse + specular;
    }
    vec3 calculateDirectional(const in uint index)
    {
        DirectionalLightInfo l = u_DirectionalLights[index];

        // Common
        vec3 norm = normalize(a_Normal);
        vec3 lightDir = normalize(-l.direction);

        // Ambient
        vec3 ambient =
        #ifdef JMAT_MATERIAL
            l.ambient * u_Material.ambient;
        #else
            l.ambient;
        #endif

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =
        #ifdef JMAT_MATERIAL
            l.diffuse * diff * u_Material.diffuse;
        #else
            l.diffuse * diff;
        #endif

        // Specular
        vec3 eyeCoords = (u_VMatrix * u_MMatrix * vec4(a_Position, 1.0)).xyz;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec =
        #ifdef JMAT_MATERIAL
            pow(max(dot(eyeCoords, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = l.specular * spec * u_Material.specular;
        #else
            max(dot(eyeCoords, reflectDir), 0.0);
            vec3 specular = l.specular * spec;
        #endif

        return ambient * diffuse * specular;
    }
#endif

// Vertex attributes for fragment shader
out vec3 out_Position;
out vec2 out_TexCoords;
#ifdef JMAT_PHONG
    out vec3 out_Normal;
#endif

// Temporary color
out vec3 out_TempColor;

void main()
{
    vec3 tempColor =
    #ifdef JMAT_AMBIENT
        u_AmbientLight;
    #else
        vec3(0.0, 0.0, 0.0);
    #endif

    #ifdef JMAT_PHONG
        for (uint i = 0u; i < u_NumPointLights; i++)
            tempColor += calculatePoint(i);
        for (uint i = 0u; i < u_NumDirectionalLights; i++)
            tempColor += calculateDirectional(i);
    #endif

    // Send vertex attributes
    out_Position = a_Position;
    out_TexCoords = a_TexCoords;
    #ifdef JMAT_PHONG
        out_Normal = a_Normal;
    #endif

    // Send material
    #ifdef JMAT_MATERIAL
        out_MatAmbient = u_Material.ambient;
        out_MatDiffuse = u_Material.diffuse;
        out_MatSpecular = u_Material.specular;
        out_MatShininess = u_Material.shininess;
    #else
        out_SolidColor = u_SolidColor;
    #endif

    // Send temporary color
    out_TempColor = tempColor;

    // Assign position
    gl_Position = u_PMatrix * u_VMatrix * u_MMatrix * vec4(a_Position, 1.0);
}