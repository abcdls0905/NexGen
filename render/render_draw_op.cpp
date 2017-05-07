
#include "render_draw_op.h"
#include "render.h"
#include "render_cmd_op.h"

IRenderDrawOp* CRenderDrawOp::NewInstance(Render* pRender)
{
	IRenderDrawOp* p = (IRenderDrawOp*)CORE_ALLOC(sizeof(CRenderDrawOp));

	new (p) CRenderDrawOp(pRender);

	return p;
}
// 释放
void CRenderDrawOp::Release()
{
	CORE_DELETE(this);
}

CRenderDrawOp::CRenderDrawOp(Render* pRender):m_pRender(pRender)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	m_cDrawOp.SetIB(0);
	m_cDrawOp.SetVB(0);

	FmVec4 vec4Color = FmVec4(0.0,0.0,0.0,0.0);
	glClearColor(vec4Color.x,vec4Color.y,vec4Color.z,vec4Color.w);
	m_cClearBufferOp.SetClearColor(vec4Color);

}

// clear buffer op
void CRenderDrawOp::SetClearColor(const FmVec4& vec4Color)
{
	if(m_cClearBufferOp.EqualClearColor(vec4Color))
	{
		return;
	}

	glClearColor(vec4Color.x,vec4Color.y,vec4Color.z,vec4Color.w);
	m_cClearBufferOp.SetClearColor(vec4Color);
}

void CRenderDrawOp::SetClearDepth(float fDepth)
{
	if(m_cClearBufferOp.EqualClearDepth(fDepth))
	{
		return;
	}

	glClearDepthf(fDepth);
    m_cClearBufferOp.SetClearDepth(fDepth);
}

void CRenderDrawOp::SetClearStencil(int s)
{
	if(m_cClearBufferOp.EqualClearStencil(s))
	{
		return;
	}
	glClearStencil(s);
    m_cClearBufferOp.SetClearStencil(s);
}

void CRenderDrawOp::SetClear(fm_uint Flat)
{
	glClear( ClearBufferFlatToGLClearBufferFlat(Flat) );
	m_cClearBufferOp.SetClear(Flat);

	if(m_pRender->IsCaptureFrame())
	{
        CRenderCmdClear cls(m_cClearBufferOp);
        //fm_tracelog((const char*)&m_cClearBufferOp,LOG_FRAME);
		(*m_pRender->GetfmxStreamFileOp())<<(&cls);
	}
}

unsigned int CRenderDrawOp::ClearBufferFlatToGLClearBufferFlat(fm_uint Flat)
{
	unsigned int nTemp = 0;
	if(Flat & IRenderDrawOp::CLEAR_COLOR_BUFFER)
	{
		nTemp |= GL_COLOR_BUFFER_BIT;
	}

	if(Flat & IRenderDrawOp::CLEAR_DEPTH_BUFFER )
	{
		nTemp |= GL_DEPTH_BUFFER_BIT;
	}

	if(Flat & IRenderDrawOp::CLEAR_STENCIL_BUFFER )
	{
		nTemp |= GL_STENCIL_BUFFER_BIT;
	}
	return nTemp;
}

//提供一次draw的数据内容
void  CRenderDrawOp::CaptureDrawCall()
{
    //判读是否有数出渲染相关数据标记
	if(m_pRender->IsCaptureFrame())
	{
        CRenderCmdDraw cmd_draw(m_cDrawOp);
        //fm_tracelog((const char*)&cmd_draw,LOG_FRAME);
		(*m_pRender->GetfmxStreamFileOp())<<(&cmd_draw);

//		CRenderCmdDraw cmd_draw_rest(m_cDrawOp);
//		(*m_pRender->GetfmxStreamFileOp())>>(&cmd_draw_rest);

		//下面进行一些渲染相关资源的数据同步

		//渲染状态数据
		IRenderStateOp* pDrawStateOp = m_pRender->GetRenderStateOp();
		CRenderStateOp* pStateOp = (CRenderStateOp*) pDrawStateOp;
	 
		CRenderCmdResStateOp cmd_draw_state(*pStateOp);
		//fm_tracelog((const char*)&cmd_draw_state,LOG_FRAME);
		(*m_pRender->GetfmxStreamFileOp())<<(&cmd_draw_state);

		//渲染模式数据
		CRenderCmdResDrawOp cmd_draw_op(*this);
		//fm_tracelog((const char*)&cmd_draw_op,LOG_FRAME);
		(*m_pRender->GetfmxStreamFileOp())<<(&cmd_draw_op);

		//调整一下，写入当前shader相关数据
		CRenderCmdResShaderOp cmd_shader_op(m_pRender->GetUsedShader());
		(*m_pRender->GetfmxStreamFileOp())<<(&cmd_shader_op);
	}

}

// vertex op
// Set Vertex Attrib Format Info
void CRenderDrawOp::SetVertexAttribPointer(unsigned int index,int size,IRenderDrawOp::VERTEX_DATA_TYPE type, fm_sizei stride, const void* ptr )
{
 	if(!m_bSetVB && m_cVertexOp.EqualVertexAttribPointer(index,size,type,stride,ptr))
 		return;

    glVertexAttribPointer(index,size,VertexDataTypeToGLVertexDataType(type),GL_FALSE,stride,ptr);
	m_cVertexOp.SetVertexAttribPointer(index,size,type,stride,ptr,m_cDrawOp.GetVB());
}

void CRenderDrawOp::EnableVertexAttribArray(unsigned int index,bool bEnable)
{
	if( IsEnableVertexAttribArray(index) == bEnable)
	{
		return;
	}

	if(bEnable)
	{
		glEnableVertexAttribArray(index);
	}
	else
	{
		glDisableVertexAttribArray(index);
	}
	m_cVertexOp.EnableVertexAttribArray(index,bEnable);
}

bool CRenderDrawOp::IsEnableVertexAttribArray(unsigned int index)
{
	return m_cVertexOp.IsEnableVertexAttribArray(index);
}

// draw call func
void CRenderDrawOp::DrawArrays(IRenderDrawOp::DRAW_MODE draw_mode,int first,fm_sizei count)
{
	glDrawArrays( DrawModeToGLDrawMode(draw_mode) , first ,count );
	m_cDrawOp.DrawArrays(draw_mode,first,count);

    CaptureDrawCall();

#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderDrawArrayNum++;

	switch(draw_mode)
	{
	case IRenderDrawOp::DRAW_TRIANGLES:
		m_pRender->GetPerformance()->nDrawTriangleNum+=count;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_STRIP:
		m_pRender->GetPerformance()->nDrawTriangleStripNum+=count;
		break;
	case IRenderDrawOp::DRAW_POINTS:
		m_pRender->GetPerformance()->nDrawPointNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINE_STRIP:
		m_pRender->GetPerformance()->nDrawLineStripNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINE_LOOP:
		m_pRender->GetPerformance()->nDrawLineLoopNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINES:
		m_pRender->GetPerformance()->nDrawLineNum+=count;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_FAN:
		m_pRender->GetPerformance()->nDrawTriangleFanNum+=count;
		break;
	default:
		break;
	};

#endif
}

void CRenderDrawOp::DrawIndex(IRenderDrawOp::DRAW_MODE draw_mode,fm_sizei count,IRenderDrawOp::VERTEX_INDEX_TYPE index_type,void* indices)
{
	glDrawElements( DrawModeToGLDrawMode(draw_mode),count, VertexIndexTypeToGLVertexIndexType(index_type),indices );
	m_cDrawOp.DrawIndex(draw_mode,count,index_type,indices);

    CaptureDrawCall();

#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderDrawIndexNum++;
	switch(draw_mode)
	{
	case IRenderDrawOp::DRAW_TRIANGLES:
		m_pRender->GetPerformance()->nDrawTriangleNum+=count;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_STRIP:
		m_pRender->GetPerformance()->nDrawTriangleStripNum+=count;
		break;
	case IRenderDrawOp::DRAW_POINTS:
		m_pRender->GetPerformance()->nDrawPointNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINE_STRIP:
		m_pRender->GetPerformance()->nDrawLineStripNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINE_LOOP:
		m_pRender->GetPerformance()->nDrawLineLoopNum+=count;
		break;
	case IRenderDrawOp::DRAW_LINES:
		m_pRender->GetPerformance()->nDrawLineNum+=count;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_FAN:
		m_pRender->GetPerformance()->nDrawTriangleFanNum+=count;
		break;
	default:
		break;
	};
#endif
}

// set vb
void CRenderDrawOp::SetVB(fm_uint vb)
{
#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderSumVBNum++;
#endif	 

	if( m_cDrawOp.GetVB() == vb )
	{ 
        m_bSetVB = false;
		return;
	}

#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderUsedVBNum++;
#endif	

	glBindBuffer(GL_ARRAY_BUFFER, vb);
	m_cDrawOp.SetVB(vb);
	m_bSetVB = true;
}

// set ib
void CRenderDrawOp::SetIB(fm_uint ib)
{
#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderSumIBNum++;
#endif	 
	if( m_cDrawOp.GetIB() == ib )
	{
		return;
	}

#ifdef PERFORMANCE_DEBUG
	m_pRender->GetPerformance()->nRenderUsedIBNum++;
#endif	
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ib);
	m_cDrawOp.SetIB(ib);

}

void CRenderDrawOp::Print()
{

}

unsigned int CRenderDrawOp::DrawModeToGLDrawMode(IRenderDrawOp::DRAW_MODE draw_mode)
{
	unsigned int tDrawMode = 0;
	switch(draw_mode)
	{
	case IRenderDrawOp::DRAW_TRIANGLES:
		tDrawMode = GL_TRIANGLES;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_STRIP:
		tDrawMode = GL_TRIANGLE_STRIP;
		break;
	case IRenderDrawOp::DRAW_POINTS:
		tDrawMode = GL_POINTS;
		break;
	case IRenderDrawOp::DRAW_LINE_STRIP:
		tDrawMode = GL_LINE_STRIP;
		break;
	case IRenderDrawOp::DRAW_LINE_LOOP:
		tDrawMode = GL_LINE_LOOP;
		break;
	case IRenderDrawOp::DRAW_LINES:
		tDrawMode = GL_LINES;
		break;
	case IRenderDrawOp::DRAW_TRIANGLE_FAN:
		tDrawMode = GL_TRIANGLE_FAN;
		break;
	default:
		break;
	};
	return tDrawMode;
}

unsigned int CRenderDrawOp::VertexIndexTypeToGLVertexIndexType(IRenderDrawOp::VERTEX_INDEX_TYPE index_type)
{
	unsigned int VertexIndexType = 0;
	switch(index_type)
	{
	case IRenderDrawOp::VERTEX_INDEX_UNSIGNED_SHORT:
		VertexIndexType = GL_UNSIGNED_SHORT;
		break;
	case IRenderDrawOp::VERTEX_INDEX_UNSIGNED:
		VertexIndexType = GL_UNSIGNED_INT;
		break;
	default:
		break;
	};
	return VertexIndexType;
}

unsigned int CRenderDrawOp::VertexDataTypeToGLVertexDataType(IRenderDrawOp::VERTEX_DATA_TYPE data_type)
{
	unsigned int VertexDataType = 0;
	switch(data_type)
	{
	case IRenderDrawOp::VERTEX_DATA_FLOAT:
		VertexDataType =  GL_FLOAT;
		break;
	case IRenderDrawOp::VERTEX_DATA_UNSIGNED_BYTE:
		VertexDataType =  GL_UNSIGNED_BYTE;
		break;
	case IRenderDrawOp::VERTEX_DATA_BYTE:
		VertexDataType =  GL_BYTE;
		break;
	case IRenderDrawOp::VERTEX_DATA_SHORT:
		VertexDataType = GL_SHORT;
		break;
	case IRenderDrawOp::VERTEX_DATA_UNSIGNED_SHORT:
		VertexDataType = GL_UNSIGNED_SHORT;
		break;
	case IRenderDrawOp::VERTEX_DATA_FIXED:
		VertexDataType = GL_FIXED;
		break;
	case IRenderDrawOp::VERTEX_DATA_HALF_FLOAT:
#ifdef RENDER_ES_3_0
		VertexDataType = GL_HALF_FLOAT;
#else
        VertexDataType = GL_HALF_FLOAT_OES;
#endif
		break;
	default:
		break;
	};
	return VertexDataType;
}

static const float g_vertexColors[] = {
  1.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 1.0f,
};

static const float g_vertexTexcoord[] = {
  0.f, 0.f,    
  1.f, 0.f,    
  0.0f, 1.f,    
  1.0f, 1.f,    
};

static const unsigned short g_indices[] = {
  0, 1, 2, 1, 2, 3,
};

static const float g_vertexPositions[] = {
  -0.5f, -0.5f,   0.0f,
  0.5f, -0.5f,   0.0f,
  -0.5f,  0.5f,   0.0f,
  0.5f,  0.5f,   0.0f,
};

GLuint progHandle = 0;
GLuint verShader = 0;
GLuint fraShader = 0;

GLuint vBuffer = 0;
GLuint iBuffer = 0;

char* verFile = "E:\\SVNPro\\NexGen\\bin\\pro\\eff\\vertex.vsh";
char* fraFile = "E:\\SVNPro\\NexGen\\bin\\pro\\eff\\fragment.fsh";

char* ReadFileData(char* file)
{
  FILE* fp = fopen(file, "r");
  if (!fp)
    return nullptr;
  int len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char* data = new char[len+1];
  memset(data, 0, len+1);
  fread(data, sizeof(char), len, fp);
  fclose(fp);
  return data;
}

void CRenderDrawOp::DrawTest()
{
  if (!progHandle)
  {
    char* verData = ReadFileData(verFile);
    char* fraData = ReadFileData(fraFile);

    verShader = glCreateShader(GL_VERTEX_SHADER);
    fraShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(verShader, 1, &verData, 0);
    glShaderSource(fraShader, 1, &fraData, 0);

    glCompileShader ( verShader );
    glCompileShader ( fraShader );
    int compiled = 0;
    glGetShaderiv ( verShader, GL_COMPILE_STATUS, &compiled );
    if (!compiled)
    {
      char log[128];
      int len = 0;
      glGetShaderInfoLog(fraShader, 128, &len, log);
      len = 0;
    }
    compiled = 0;
    glGetShaderiv ( fraShader, GL_COMPILE_STATUS, &compiled );
    if (!compiled)
    {
      char log[128];
      int len = 0;
      glGetShaderInfoLog(fraShader, 128, &len, log);
      len = 0;
    }

    progHandle = glCreateProgram ( );

    glAttachShader ( progHandle, verShader );
    glAttachShader ( progHandle, fraShader );

    glLinkProgram ( progHandle );
    int linked = 0;
    glGetProgramiv ( progHandle, GL_LINK_STATUS, &linked );

    glDeleteShader(verShader);
    glDeleteShader(fraShader);

    if ( !linked ) 
    {
      GLint infoLen = 0;
      glGetProgramiv ( progHandle, GL_INFO_LOG_LENGTH, &infoLen );
      if ( infoLen > 1 )
      {
        char infoLog[128];
        glGetProgramInfoLog ( progHandle, infoLen, NULL, infoLog );
        return;
      }
    }

    delete []verData;
    delete []fraData;

    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPositions), g_vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return;
  }

  EnableCullFace(false);
  glUseProgram( progHandle );

  int loc = glGetAttribLocation(progHandle, "position");
  glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
  glEnableVertexAttribArray( loc );
  glVertexAttribPointer( loc, 3, GL_FLOAT, false, 0, 0 );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
  glDrawElements( GL_TRIANGLES, 3 * 2, GL_UNSIGNED_SHORT, 0 );

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram( 0 );
}

void CRenderDrawOp::EnableCullFace(bool enable)
{
  if (enable)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
}