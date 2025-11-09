// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 lightPos;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

uniform vec3 lightColor;
uniform vec3 objectColor;

// second light source
uniform vec3 lightPos2;
uniform vec3 lightColor2;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;   
    vec3 ambient2 = ambientStrength * lightColor2; // second light source
    
     // diffuse 
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);

    // both light sources
    vec3 lightDir1 = normalize(lightPos - FragPos);
    vec3 lightDir2 = normalize(lightPos2 - FragPos);

    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;

    // second light source
    float diff1 = max(dot(norm, lightDir1), 0.0);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse = diff1 * lightColor + diff2 * lightColor2;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor; 

    // both light sources
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32);
    vec3 specular = specularStrength * (spec1 * lightColor + spec2 * lightColor2);
    
    vec3 result = (ambient + ambient2 + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}