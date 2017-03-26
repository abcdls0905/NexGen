
uniform sampler2D tex_Diffuse;
varying mediump vec2 texUV;

void main ()
{
  mediump vec4 crTexDiffuse = texture2D(tex_Diffuse, texUV.xy);
  gl_FragColor = crTexDiffuse;
}