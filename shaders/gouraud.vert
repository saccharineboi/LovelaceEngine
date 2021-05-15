#version 330 core

////////////////////////////////////////////////////////////////////////////////
struct material_t
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
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

////////////////////////////////////////////////////////////////////////////////
uniform mat4 uPVM;
uniform mat4 uModel;
uniform mat4 uNormal;

////////////////////////////////////////////////////////////////////////////////
uniform dirLight_t      uDirLights      [NUM_DIR_LIGHTS];
uniform pointLight_t    uPointLights    [NUM_POINT_LIGHTS];
uniform spotLight_t     uSpotLights     [NUM_SPOT_LIGHTS];

uniform material_t uMaterial;
uniform vec3 uViewPos;

////////////////////////////////////////////////////////////////////////////////
vec3 computeDirLight(material_t material, dirLight_t light, vec3 normal, vec3 viewDir)
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

        vec3 result = light.intensity * (ambient + diffuse + specular);
        return max(result, vec3(0.0f));
}

////////////////////////////////////////////////////////////////////////////////
vec3 computePointLight(material_t material, pointLight_t light, vec3 normal, vec3 viewDir, vec3 vertPos)
{
        // attenuation
        float distance = length(light.position - vertPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        // diffuse light
        vec3 direction = normalize(vertPos - light.position);
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
vec3 computeSpotLight(material_t material, spotLight_t light, vec3 normal, vec3 viewDir, vec3 vertPos)
{
        // cutoff
        vec3 direction = normalize(vertPos - light.position);
        float theta = dot(-direction, normalize(-light.direction));
        float epsilon = light.cutoff - light.outerCutoff;
        float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

        // attenuation
        float distance = length(light.position - vertPos);
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
out vec3 vColor;
void main()
{
        gl_Position = uPVM * vec4(aPos, 1.0f);

        vec3 normal = normalize(mat3(uNormal) * aNorm);
        vec3 vertPos = vec3(uModel * vec4(aPos, 1.0f));
        vec3 viewDir = normalize(uViewPos - vertPos);
        vec3 resultColor = vec3(0.0f);
 
        // directional lights
        for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
                resultColor += computeDirLight(uMaterial, uDirLights[i], normal, viewDir);

        // point lights
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
                resultColor += computePointLight(uMaterial, uPointLights[i], normal, viewDir, vertPos);

        // spot lights
        for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
                resultColor += computeSpotLight(uMaterial, uSpotLights[i], normal, viewDir, vertPos);

        vColor = resultColor;
}
