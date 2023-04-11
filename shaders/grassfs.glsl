# version 330 core

in vec2 textCoords;
out vec4 FragColor;

uniform sampler2D grass;

void main()
{
    if(texture(window,textCoords).a < 0.1){discard;}
    FragColor = texture(window,textCoords);
}
