#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 textCoords;

void main()
{
    textCoords = aTex;
    gl_Position = projection*view*model*vec4(aPos,1.0); 
    
}