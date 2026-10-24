#version 440 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);   // light
    vec3 V = normalize(viewPos - FragPos);    // camera
    vec3 R = reflect(-L, N);                  // mirror direction of the light

    vec3 ambient  = 0.12 * lightColor;
    vec3 diffuse  = max(dot(N, L), 0.0) * lightColor;
    vec3 specular = 0.65 * pow(max(dot(V, R), 0.0), 48.0) * lightColor;

    FragColor = vec4((ambient + diffuse) * objectColor + specular, 1.0);
}