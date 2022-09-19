#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 modelView;

out vec2 TexCoord;
void main()
{
    gl_Position = modelView * vec4(aPos,1.0);
    TexCoord = vec2(aTexCoord.x,aTexCoord.y);
}