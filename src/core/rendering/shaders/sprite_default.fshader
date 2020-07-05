#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main() {
	color = vec4(spriteColor, 1.0f) * texture(sprite, texCoord);
}
