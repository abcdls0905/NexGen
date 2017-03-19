
#ifndef _RENDER_HEADER_H
#define _RENDER_HEADER_H

#include "../public/macros.h"

#ifdef RENDER_ES_3_0
	#ifdef FX_SYSTEM_IOS
	#include <OpenGLES/ES3/gl.h>
	#include <OpenGLES/ES3/glext.h>
	#else
	#include <gles3/gl3.h>
	#include <gles3/gl3ext.h>
	#include <egl/egl.h>
	#endif   
#else

	#ifdef FX_SYSTEM_IOS
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
	#else
	#include <gles2/gl2.h>
	#include <gles2/gl2ext.h>
	#include <egl/egl.h>
	#endif

#endif

#include "../math/fm_math.h"

#ifdef FX_SYSTEM_WINDOWS
#pragma comment(lib, "../extern/ogles2/lib/libegl.lib")
#pragma comment(lib, "../extern/ogles2/lib/libglesv2.lib")
#endif // FX_SYSTEM_WINDOWS


#ifndef FX_SYSTEM_WINDOWS
#define _SHADER_OPTIMIZER_
#endif


#endif
