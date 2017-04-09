attribute mediump vec3 iPos;

attribute mediump vec2 iTex0;
attribute mediump vec3 iNormal;

uniform highp mat4 c_mtxWVP;
uniform mediump vec4 c_vViewPos;
uniform highp mat4 c_mtxWorld;

varying mediump vec2 texUV;
varying mediump vec3 oNormal;
varying mediump vec4 oViewToVertex;

void main(void)
{
    vec4 _pos = vec4(iPos, 1);
    texUV = iTex0;
    gl_Position =  c_mtxWVP * _pos;
    mediump vec3 _nor = iNormal;
    oNormal = (c_mtxWorld * vec4(_nor, 0.0)).xyz;
    mediump vec4 pos_in_world = c_mtxWorld * _pos;
    vec3 view_vec = pos_in_world.xyz - c_vViewPos.xyz;
    oViewToVertex = vec4(view_vec, length(view_vec));
}