
uniform sampler2D tex_Diffuse;

uniform mediump vec3 c_vLightDir;
uniform mediump vec4 c_LightDiffuse;


varying mediump vec2 texUV;
varying mediump vec3 oNormal;
varying mediump vec4 oViewToVertex;

void main ()
{
    mediump vec4 crTexDiffuse = texture2D(tex_Diffuse, texUV.xy);

    mediump vec3 nor = normalize(oNormal);
    mediump vec3 light_dir = c_vLightDir;
    mediump vec4 diffuse_color = vec4(1, 1, 1, 1);
    diffuse_color.xyz = max(dot(light_dir, nor), 0.0) * c_LightDiffuse.xyz;

    mediump vec3 specular_material = vec3(2.0, 2.0, 2.0);
    mediump float temp = 10.0;
    mediump float gloss = max(temp, 0.000001);
	mediump float specular_inten = pow(max(dot(reflect(normalize(oViewToVertex.xyz), nor), light_dir), 0.000001), gloss);
    mediump vec3 specular_color = specular_inten * specular_material.xyz * c_LightDiffuse.xyz;
    diffuse_color.xyz += specular_color.xyz;

    gl_FragColor = crTexDiffuse * diffuse_color;
}