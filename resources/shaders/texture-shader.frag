#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

const int MAX_DIRECTIONAL_LIGHTS = 4;
const int MAX_POINT_LIGHTS = 12;
const int MAX_SPOT_LIGHTS = 6;

uniform Material material;
uniform vec3 cameraPosition;

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform uint directionalLightArraySize;
uniform uint pointLightArraySize;
uniform uint spotLightArraySize;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoordinates;

out vec4 FragColor;

void ApplyDirectionalLights(inout vec3 result) {
    const vec3 NORM = normalize(Normal);
    
    for (uint i = 0; i < directionalLightArraySize; i++) {
        const DirectionalLight directionalLight = directionalLights[i];

        const vec3 ambient = directionalLight.ambient * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 lightDirection = normalize(-directionalLight.direction);
        const float diff = max(dot(NORM, lightDirection), 0.0f);
        const vec3 diffuse = directionalLight.diffuse * diff * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 viewDirection = normalize(cameraPosition - FragPos);
        // Reflection vector along the normal axis
        const vec3 reflectDirection = reflect(-lightDirection, NORM);
        const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
        const vec3 specular = directionalLight.specular * spec * texture(material.specular, TextureCoordinates).rgb;

        result += ambient + diffuse + specular;
    }
}

void ApplyPointLights(inout vec3 result) {
    const vec3 NORM = normalize(Normal);
    
    for (uint i = 0; i < pointLightArraySize; i++) {
        const PointLight pointLight = pointLights[i];

        vec3 ambient = pointLight.ambient * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 lightDirection = normalize(pointLight.position - FragPos);
        const float diff = max(dot(NORM, lightDirection), 0.0f);
        vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 viewDirection = normalize(cameraPosition - FragPos);
        // Reflection vector along the normal axis
        const vec3 reflectDirection = reflect(-lightDirection, NORM);
        const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
        vec3 specular = pointLight.specular * spec * texture(material.specular, TextureCoordinates).rgb;

        const float DISTANCE = distance(pointLight.position, FragPos);
        const float attenuation = 1.0f / (pointLight.constant + pointLight.linear * DISTANCE + pointLight.quadratic * pow(DISTANCE, 2));

        result += attenuation * (ambient + diffuse + specular);
    }
}

void ApplySpotLights(inout vec3 result) {
    const vec3 NORM = normalize(Normal);
    
    for (uint i = 0; i < spotLightArraySize; i++) {
        const SpotLight spotLight = spotLights[i];

        vec3 ambient = spotLight.ambient * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 lightDirection = normalize(spotLight.position - FragPos);
        const float diff = max(dot(NORM, lightDirection), 0.0f);
        vec3 diffuse = spotLight.diffuse * diff * texture(material.diffuse, TextureCoordinates).rgb;

        const vec3 viewDirection = normalize(cameraPosition - FragPos);
        // Reflection vector along the normal axis
        const vec3 reflectDirection = reflect(-lightDirection, NORM);
        const float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
        vec3 specular = spotLight.specular * spec * texture(material.specular, TextureCoordinates).rgb;

        const float theta = dot(lightDirection, normalize(-spotLight.direction));
        const float epsilon = spotLight.cutoff - spotLight.outercutoff;
        const float intencity = clamp((theta - spotLight.outercutoff) / epsilon, 0.0f, 1.0f);

        const float DISTANCE = distance(spotLight.position, FragPos);
        const float attenuation = 1.0f / (spotLight.constant + spotLight.linear * DISTANCE + spotLight.quadratic * pow(DISTANCE, 2));

        result += (ambient + diffuse + specular) * attenuation * intencity;
    }
}

void ApplyEmission(inout vec3 result) {
    const vec3 emissionFactor = step(vec3(1.0f), vec3(1.0f) - vec3(texture(material.specular, TextureCoordinates)));
    const vec3 emission = vec3(texture(material.emission, TextureCoordinates)) * emissionFactor;

    result += emission;
}

void main() {
    vec3 result = vec3(0.0f);
    
    ApplySpotLights(result);
    ApplyPointLights(result);
    ApplyDirectionalLights(result);

    ApplyEmission(result);

    FragColor = vec4(result, 1.0f);
}
