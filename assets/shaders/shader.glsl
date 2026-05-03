#version 330

uniform sampler2D tex;
uniform sampler2D texture0;
vec2 resolution = vec2(640., 640.);

out vec4 finalColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution;
	finalColor = texture(tex, uv);
}