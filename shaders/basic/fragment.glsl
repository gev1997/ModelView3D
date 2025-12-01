#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // Super simple unlit bright magenta
    FragColor = vec4(1.0, 0.3, 0.3, 1.0);
}