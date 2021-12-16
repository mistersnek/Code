 #version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec4 colorLight;
in vec2 texCoord;
uniform sampler2D tex0;

vec4 spotLight()
{
    
    float outerCone = 0.95f;
    float innerCone = 0.999f;
    
    
     // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8);
    float specular = specularStrength * spec; 

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f); 
        
    vec3 result = (diffuse * inten + ambient) + (specular * inten ) * lightColor;
    return texture(tex0, texCoord) * vec4(result, 1.0f);
}

void main()
{
    color = spotLight();
} 
