#version 330 core

out vec4 FragColor;
uniform float time;

void main()
{
    float red = (sin(time) + 1.0) / 2.0; 
    FragColor = vec4(red, 0.5f, 0.2f, 1.0f);
}
