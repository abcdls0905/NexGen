attribute mediump vec3 iPos;
attribute mediump vec3 iPosAni;
attribute mediump vec3 iNormal;
attribute mediump vec3 iNormalAni;
#ifdef VERTEX_TEXTURE0
attribute mediump vec2 iTex0;
#endif
#ifdef VERTEX_TEXTURE1
attribute mediump vec2 iTex1;
#endif
#ifdef FILTERMAP
attribute mediump vec2 iTexFilter;
#endif
#ifdef	VERTEX_SKELETON
attribute mediump vec4 iBone;
attribute mediump vec4 iWeight;
#endif
#ifdef VERTEX_TANGENT
attribute mediump vec3 iTangent;
#endif
#ifdef VERTEX_BINORMAL
attribute mediump vec3 iBinormal;
#endif
attribute mediump vec4 iDiffuse;

#if defined VERTEX_TEXTURE0 && defined ALPHATEST
varying mediump vec2 oTex0;
#endif

//varying mediump float oDepth;

//cbPerScene
uniform mediump mat4 c_mtxWVP;

#ifdef VERTEX_SKELETON
uniform mediump vec4 c_BoneList[234];
#endif

void main(void)
{
	mediump vec4 _pos;
	
	_pos = vec4(iPos, 1.0);

	mediump vec4 oPos = c_mtxWVP * _pos; 
	gl_Position = oPos;

#if defined VERTEX_TEXTURE0 && defined ALPHATEST
	oTex0 = iTex0;
#endif

	//oDepth = max(oPos.z, 0.0);
}


