#version 330 core
out vec4 FragColor;

in vec2 vUV;

uniform sampler2D brickTexture;

void main()
{
  vec4 texColor = texture(baseColorTexture, vUV);
  FragColor = texColor;
}