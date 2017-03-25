attribute mediump vec3 iPos;

uniform highp mat4 c_mtxWVP;
void main(void)
{
  vec4 _pos = vec4(iPos, 1);
  gl_Position =  c_mtxWVP * _pos;
}


