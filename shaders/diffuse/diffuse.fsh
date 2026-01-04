#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 vWorldPos;

void main() {
    float ambient = 0.1;
    vec3 base = vec3(1.0, 0.2, 0.6);
    vec3 light_position = vec3(2.0, 3.0, 2.0);
    vec3 n = normalize(vNormal);
    vec3 l = normalize(light_position - vWorldPos);
    float d = max(dot(n, l), 0.0);
    vec3 color = base * (ambient + d);
    FragColor = vec4(color, 1.0);
}
