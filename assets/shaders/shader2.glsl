

void main(){

vec2 uv = gl_FragCoord.xy/vec2(640.,320.);
vec3 c1 = vec3(0., 1., 1.);
vec3 c2 = vec3(1., 1., 0.);
vec3 c = mix(c1, c2, uv.x);
gl_FragColor = vec4(c, 1.);

}