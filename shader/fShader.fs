#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
	vec3 Normal;
	//vec2 TexCoords;
} fs_in;

// struct DirLight {
//     vec3 direction;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
// struct SpotLight {
//     vec3 position;
//     vec3 direction;
//     float cutOff;
//     float outerCutOff;

//     float constant;
//     float linear;
//     float quadratic;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// uniform DirLight dirLight;
// uniform SpotLight spotLight;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

// function prototypes
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
     // ka,kd,ks   ia,id,is
    float ka = 0.4, kd = 0.8, ks = 0.4;
    vec3 Ia = vec3(0.6, 0.6, 0.6);
    vec3 Id = vec3(0.8, 0.8, 0.8);
    vec3 Is = vec3(0.4, 0.4, 0.4);

    // ambient
    vec3 ambient = ka * Ia;
    // diffuse
    vec3 L = normalize(lightPos - fs_in.FragPos);
    vec3 N = normalize(fs_in.Normal);
    vec3 diffuse = kd * Id * max(dot(L, N), 0.0);
    // specular
    vec3 V = normalize(viewPos - fs_in.FragPos);
    vec3 R = reflect(-1*L, N);
    vec3 specular = ks * Is * pow(max(dot(V, R), 0.0), 256.0);


    vec3 I =  vec3(ambient + diffuse + specular)*objectColor;
    FragColor = vec4(I, 1.0);
}

// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = max(dot(viewDir, reflectDir), 0.0);
//     // combine results
//     vec3 ambient = light.ambient;
//     vec3 diffuse = light.diffuse * diff;
//     vec3 specular = light.specular * spec;
//     return (ambient + diffuse + specular);
// }
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = max(dot(viewDir, reflectDir), 0.0);
//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//     // spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction));
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//     // combine results
//     vec3 ambient = light.ambient;
//     vec3 diffuse = light.diffuse * diff;
//     vec3 specular = light.specular * spec;
//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;
//     return (ambient + diffuse + specular);
// }
