# version 330 core

in vec2 texCoord;
out vec4 FragColor;


uniform sampler2D block_face;

void main()
{
    FragColor = texture(block_face,texCoord);
}
