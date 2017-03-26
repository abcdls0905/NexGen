attribute mediump vec3 iPos;
attribute mediump vec2 iTex0;
varying mediump vec2 texUV;

uniform highp mat4 c_mtxWVP;

void main(void)
{
  vec4 _pos = vec4(iPos, 1);
  texUV = iTex0;
  gl_Position =  c_mtxWVP * _pos;
}