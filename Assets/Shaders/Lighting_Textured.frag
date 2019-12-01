#version 330 core

#define MAX_NUM_POINT_LIGHTS 8
#define MAX_NUM_SPOT_LIGHTS 64
#define MAX_NUM_DIR_LIGHTS 4


struct Material {
	float ambient;
	float specular;
	float shininess;
};

struct PointLight {
	vec3 color;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	

	float constant;
	float linear;
	float quadratic;
};

struct DirLight {
	vec3 color;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

uniform PointLight pointLights[MAX_NUM_POINT_LIGHTS];
uniform int numPointLights;
uniform SpotLight spotLights[MAX_NUM_SPOT_LIGHTS];
uniform int numSpotLights;
uniform DirLight dirLights[MAX_NUM_DIR_LIGHTS];
uniform int numDirLights;
uniform Material material;
uniform sampler2D textureSampler;

uniform vec3 viewPos;

out vec4 color;

in vec3 v_color;
in vec3 v_normal;
in vec3 v_pos;
in vec2 v_uv;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 v_pos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 v_pos);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(v_normal);
	vec3 viewDir = normalize(viewPos - v_pos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for (int i=0; i<numPointLights; i++) {
		result += CalcPointLight(pointLights[i], norm, viewDir, v_pos);
	}

	for (int i=0; i<numSpotLights; i++) {
		result += CalcSpotLight(spotLights[i], norm, viewDir, v_pos);
	}

	for (int i=0; i<numDirLights; i++) {
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}

	vec4 textureColor = texture( textureSampler, v_uv );
	color = vec4(result, 1.0f) * textureColor;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 v_pos) {
	vec3 lightDir = normalize(light.position - v_pos);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	float distance = length(light.position - v_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	return (ambient + diffuse + specular) * attenuation * light.color;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 v_pos) {
	vec3 lightDir = normalize(light.position - v_pos);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	float distance = length(light.position - v_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		
	return (ambient + diffuse + specular) * attenuation * intensity * light.color;
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	
	vec3 ambient = light.ambient * material.ambient;
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	
	return (ambient + diffuse + specular) * light.color;
}