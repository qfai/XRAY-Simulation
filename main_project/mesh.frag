// for render mesh
#version 400

in vec3 Color;
in vec3 normal;
in vec3 FragPos;
//in vec4 gl_Position;

layout (location = 0) out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// debug test:
	// lightColor, lightPos correct
	// Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

	vec3 rgbColor = (ambient + diffuse + specular) * vec3(1.0, 0.50, 0.31);
    //FragColor = vec4(Color, 1.0);
    FragColor = vec4(rgbColor, 1.0);
	//FragColor = vec4(rgbColor, 0.5);
}
