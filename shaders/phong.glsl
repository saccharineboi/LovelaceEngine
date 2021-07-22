#ifdef VERTEX_SHADER

////////////////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

////////////////////////////////////////////////////////////////////////////////
uniform mat4 uPVM;
uniform mat4 uModel;
uniform mat4 uNormal;
uniform vec2 uTexMultiplier;

////////////////////////////////////////////////////////////////////////////////
out vec3 vNorm;
out vec2 vTexCoord;
out vec3 vFragPos;

////////////////////////////////////////////////////////////////////////////////
void main()
{
        gl_Position = uPVM * vec4(aPos, 1.0f);
        vNorm = vec3(uNormal * vec4(aNorm, 0.0f));
        vFragPos = vec3(uModel * vec4(aPos, 1.0f));
        vTexCoord = uTexMultiplier * aTexCoord;
}

#elif defined(FRAGMENT_SHADER)

////////////////////////////////////////////////////////////////////////////////
struct material_t
{
        vec3 ambientColor;
        vec3 diffuseColor;
        vec3 specularColor;
        float shininess;

        bool enableAmbientTexture;
        sampler2D ambientTexture;

        bool enableDiffuseTexture;
        sampler2D diffuseTexture;

        bool enableSpecularTexture;
        sampler2D specularTexture;
};

struct material_cached_t
{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
};

struct dirLight_t
{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        vec3 direction;
        float intensity;
};

struct pointLight_t
{
        vec3 diffuse;
        vec3 specular;

        vec3 position;
        float range;
        float intensity;

        float constant;
        float linear;
        float quadratic;
};

struct spotLight_t
{
        vec3 diffuse;
        vec3 specular;

        vec3 position;
        vec3 direction;
        float range;
        float intensity;

        float cutoff;
        float outerCutoff;

        float constant;
        float linear;
        float quadratic;
};

////////////////////////////////////////////////////////////////////////////////
#define NUM_DIR_LIGHTS          2
#define NUM_POINT_LIGHTS        4
#define NUM_SPOT_LIGHTS         2

////////////////////////////////////////////////////////////////////////////////
in vec3 vNorm;
in vec3 vFragPos;
in vec2 vTexCoord;

////////////////////////////////////////////////////////////////////////////////
uniform dirLight_t      uDirLights      [NUM_DIR_LIGHTS];
uniform pointLight_t    uPointLights    [NUM_POINT_LIGHTS];
uniform spotLight_t     uSpotLights     [NUM_SPOT_LIGHTS];

uniform material_t uMaterial;
uniform vec3 uViewPos;

////////////////////////////////////////////////////////////////////////////////
vec3 computeDirLight(material_cached_t material, dirLight_t light, vec3 normal, vec3 viewDir)
{
        // ambient light
        vec3 ambient = light.ambient * material.diffuse;

        // diffuse light
        float diff = max(dot(-light.direction, normal), 0.0f);
        vec3 diffuse = diff * light.diffuse * material.diffuse;

        // specular light
        vec3 reflectDir = reflect(light.direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular;

        vec3 result = ambient + light.intensity * (diffuse + specular);
        return max(result, vec3(0.0f));
}

////////////////////////////////////////////////////////////////////////////////
vec3 computePointLight(material_cached_t material, pointLight_t light, vec3 normal, vec3 viewDir)
{
        // attenuation
        float distance = length(light.position - vFragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        // diffuse light
        vec3 direction = normalize(vFragPos - light.position);
        float diff = max(dot(-direction, normal), 0.0f);
        vec3 diffuse = diff * light.diffuse * material.diffuse;

        // specular light
        vec3 reflectDir = reflect(direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular;
        
        vec3 result = attenuation * light.intensity * (diffuse + specular);
        return max(result, vec3(0.0f));
}

////////////////////////////////////////////////////////////////////////////////
vec3 computeSpotLight(material_cached_t material, spotLight_t light, vec3 normal, vec3 viewDir)
{
        // cutoff
        vec3 direction = normalize(vFragPos - light.position);
        float theta = dot(-direction, normalize(-light.direction));
        float epsilon = light.cutoff - light.outerCutoff;
        float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

        // attenuation
        float distance = length(light.position - vFragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        // diffuse light
        float diff = max(dot(-direction, normal), 0.0f);
        vec3 diffuse = diff * light.diffuse * material.diffuse;

        // specular light
        vec3 reflectDir = reflect(direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular;

        vec3 result = intensity * attenuation * light.intensity * (diffuse + specular);
        return max(result, vec3(0.0f));
}

////////////////////////////////////////////////////////////////////////////////
material_cached_t cacheMaterial(material_t material)
{
        material_cached_t cachedMaterial;

        cachedMaterial.ambient = (material.enableAmbientTexture
                               ? texture(material.ambientTexture, vTexCoord).rgb
                               * material.ambientColor
                               : material.ambientColor);

        cachedMaterial.diffuse = (material.enableDiffuseTexture
                               ? texture(material.diffuseTexture, vTexCoord).rgb
                               * material.diffuseColor
                               : material.diffuseColor);

        cachedMaterial.specular = (material.enableSpecularTexture
                                ? texture(material.specularTexture, vTexCoord).rgb
                                * material.specularColor
                                : material.specularColor);

        cachedMaterial.shininess = material.shininess;
        return cachedMaterial;
}

////////////////////////////////////////////////////////////////////////////////
out vec4 color;
void main()
{
        vec3 normal = normalize(vNorm);
        vec3 viewDir = normalize(uViewPos - vFragPos);
        vec3 resultColor = vec3(0.0f);
        material_cached_t cached_material = cacheMaterial(uMaterial);
 
        for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
                resultColor += computeDirLight(cached_material, uDirLights[i], normal, viewDir);
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
                resultColor += computePointLight(cached_material, uPointLights[i], normal, viewDir);
        for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
                resultColor += computeSpotLight(cached_material, uSpotLights[i], normal, viewDir);

        color = vec4(resultColor, 1.0f);
}

#endif
