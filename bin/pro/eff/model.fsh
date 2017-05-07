
uniform sampler2D tex_Diffuse;
uniform sampler2D tex_Shadow;

uniform mediump vec3 c_vLightDir;
uniform mediump vec4 c_LightDiffuse;


varying mediump vec2 texUV;
varying mediump vec3 oNormal;
varying mediump vec4 oViewToVertex;
#ifdef SHADOWMAP
    varying mediump vec4 oTexCoordShadow;
#endif


void main ()
{
    mediump vec4 shadow_Inten = vec4(1.0, 1.0, 1.0, 1.0);
	#ifdef SHADOWMAP
        mediump vec2 tex_uv = (vec2(0.5, 0.5) * (oTexCoordShadow.xy / oTexCoordShadow.w)) + vec2(0.5, 0.5);
        mediump float  z = oTexCoordShadow.z;
        shadow_Inten = texture2D(tex_Shadow, tex_uv);
	#endif
	
    mediump vec4 crTexDiffuse = texture2D(tex_Diffuse, texUV.xy);

    mediump vec3 nor = normalize(oNormal);
    mediump vec3 light_dir = c_vLightDir;
    mediump vec4 diffuse_color = vec4(1, 1, 1, 1);
    #ifdef SHADOWMAP
        diffuse_color.xyz = max(dot(light_dir, nor), 0.0) * c_LightDiffuse.xyz * shadow_Inten.x;
    #else 
        diffuse_color.xyz = max(dot(light_dir, nor), 0.0) * c_LightDiffuse.xyz;
    #endif

    mediump vec3 specular_material = vec3(2.0, 2.0, 2.0);
    mediump float temp = 10.0;
    mediump float gloss = max(temp, 0.000001);
	mediump float specular_inten = pow(max(dot(reflect(normalize(oViewToVertex.xyz), nor), light_dir), 0.000001), gloss);
    #ifdef SHADOWMAP
        mediump vec3 specular_color = specular_inten * specular_material.xyz * c_LightDiffuse.xyz * shadow_Inten.x;
    #else 
        mediump vec3 specular_color = specular_inten * specular_material.xyz * c_LightDiffuse.xyz;
    #endif
    diffuse_color.xyz += specular_color.xyz;
    
    gl_FragColor = crTexDiffuse * diffuse_color;

    #ifdef SHADOWMAP
        //gl_FragColor = vec4(shadow_Inten.xyz, 1.0);
    #endif
}