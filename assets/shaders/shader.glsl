#version 330

#define PI 3.1415
uniform float t;
uniform sampler2D tex;
vec2 resolution = vec2(640., 640.);

out vec4 finalColor;

void main()
{
  vec2 uv = gl_FragCoord.xy / resolution;
  uv.x += sin(t / 100.) * 0.2;
  uv.x += sin(uv.y * sin( ( t + PI) / 100.)) * 0.2;
  finalColor = texture(tex, uv);
}