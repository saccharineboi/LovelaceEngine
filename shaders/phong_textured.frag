#version 330 core

////////////////////////////////////////////////////////////////////////////////
struct material_t
{
        sampler2D diffuse;
        sampler2D specular;
        float shininess;

};

struct material_cached_t
{
        vec4 diffuse;
        vec4 specular;
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
in vec2 vTexCoord;
in vec3 vFragPos;

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
        vec3 ambient = light.ambient * material.diffuse.rgb;

        // diffuse light
        float diff = max(dot(-light.direction, normal), 0.0f);
        vec3 diffuse = diff * light.diffuse * material.diffuse.rgb;

        // specular light
        vec3 reflectDir = reflect(light.direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular.rgb;

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
        vec3 diffuse = diff * light.diffuse * material.diffuse.rgb;

        // specular light
        vec3 reflectDir = reflect(direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular.rgb;
        
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
        vec3 diffuse = diff * light.diffuse * material.diffuse.rgb;

        // specular light
        vec3 reflectDir = reflect(direction, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
        vec3 specular = spec * light.specular * material.specular.rgb;

        vec3 result = intensity * attenuation * light.intensity * (diffuse + specular);
        return max(result, vec3(0.0f));
}

////////////////////////////////////////////////////////////////////////////////
material_cached_t genCachedMaterial(material_t material)
{
        material_cached_t material_cached;
        material_cached.diffuse = texture(uMaterial.diffuse, vTexCoord);
        material_cached.specular = texture(uMaterial.specular, vTexCoord);
        material_cached.shininess = uMaterial.shininess;
        return material_cached;
}

////////////////////////////////////////////////////////////////////////////////
out vec4 color;
void main()
{
        vec3 normal = normalize(vNorm);
        vec3 viewDir = normalize(uViewPos - vFragPos);
        material_cached_t material_cached = genCachedMaterial(uMaterial);
        vec3 resultColor = vec3(0.0f);

        // directional lights
        for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
                resultColor += computeDirLight(material_cached, uDirLights[i], normal, viewDir);

        // point lights
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
                resultColor += computePointLight(material_cached, uPointLights[i], normal, viewDir);

        // spot lights
        for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
                resultColor += computeSpotLight(material_cached, uSpotLights[i], normal, viewDir);

        color = vec4(resultColor, 1.0f);
}
