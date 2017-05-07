
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractal.h"
#include "../public/module.h"

#define MINVALUE 3
#define DEF_HEIGHT_SCALE 1.f

#ifdef _WINDOWS
#define random() rand()
#define srandom(x) srand(x)
#endif

static float randnum (float min, float max)
{
  int r;
  float	x;

  r = random ();
  x = (float)(r & 0x7fff) /
    (float)0x7fff;
  return (x * (max - min) + min);
}

#define fractRand(v) randnum (-v, v)

static float avgSquareVals (int i, int j, int stride, int size, float *fa)
{
    return ((float) (fa[((i-stride)*size) + j-stride] +
		     fa[((i-stride)*size) + j+stride] +
		     fa[((i+stride)*size) + j-stride] +
		     fa[((i+stride)*size) + j+stride]) * .25f);
}

static float avgDiamondVals (int i, int j, int stride,
			     int size, int subSize, float *fa)
{
    if (i == 0)
	return ((float) (fa[(i*size) + j-stride] +
			 fa[(i*size) + j+stride] +
			 fa[((subSize-stride)*size) + j] +
			 fa[((i+stride)*size) + j]) * .25f);
    else if (i == size-1)
	return ((float) (fa[(i*size) + j-stride] +
			 fa[(i*size) + j+stride] +
			 fa[((i-stride)*size) + j] +
			 fa[((0+stride)*size) + j]) * .25f);
    else if (j == 0)
	return ((float) (fa[((i-stride)*size) + j] +
			 fa[((i+stride)*size) + j] +
			 fa[(i*size) + j+stride] +
			 fa[(i*size) + subSize-stride]) * .25f);
    else if (j == size-1)
	return ((float) (fa[((i-stride)*size) + j] +
			 fa[((i+stride)*size) + j] +
			 fa[(i*size) + j-stride] +
			 fa[(i*size) + 0+stride]) * .25f);
    else
	return ((float) (fa[((i-stride)*size) + j] +
			 fa[((i+stride)*size) + j] +
			 fa[(i*size) + j-stride] +
			 fa[(i*size) + j+stride]) * .25f);
}

Fractal* Fractal::NewInstance()
{
  return CORE_NEW(Fractal);
}

Fractal::Fractal()
{
  data = NULL;
  surfaceH = 0.65f;
  size = 0;
}

Fractal::~Fractal()
{
  if (data)
    SAFE_DELETEARR(data);
}

bool IsPowerOf2(int value)
{
  int i, bitcount = 0;
    for (i=0; i<sizeof(int)*8; i++)
	if (value & (1<<i))
	    bitcount++;
    if (bitcount == 1)
	return true;
    else
	return false;
}

void Fractal::SetSize(int w)
{
  if (w < MINVALUE || !IsPowerOf2(w-1))
    return;
  if (data)
    SAFE_DELETEARR(data);
  size = w;
  data = new float[size*size];
  memset(data, 0, size*size*sizeof(float));
}

void Fractal::SetCornerValue(float center, float lefttop, float leftbuttom, float righttop, float rightbuttom)
{
  if (!data)
    return;
  int half = (size - 1)/2;
  float ratio = pow(2, -surfaceH);
  float scale = ratio * DEF_HEIGHT_SCALE;

  int subSize = (size-1);
  int stride = subSize / 2;
  data[(0*size)+0] =
    data[(subSize*size)+0] =
    data[(subSize*size)+subSize] =
    data[(0*size)+subSize] = 0.f;

  while (stride) 
  {
    for (int i=stride; i<subSize; i+=stride) {
      for (int j=stride; j<subSize; j+=stride) {
        data[(i * size) + j] =
          scale * fractRand (.5f) +
          avgSquareVals (i, j, stride, size, data);
        j += stride;
      }
      i += stride;
    }
    int	oddline = 0;
		for (int i=0; i<subSize; i+=stride) {
		    oddline = (oddline == 0);
			for (int j=0; j<subSize; j+=stride) {
				if ((oddline) && !j) j+=stride;

				data[(i * size) + j] =
					scale * fractRand (.5f) +
					avgDiamondVals (i, j, stride, size, subSize, data);

				if (i==0)
					data[(subSize*size) + j] =
						data[(i * size) + j];
				if (j==0)
					data[(i*size) + subSize] =
						data[(i * size) + j];

				j+=stride;
			}
		}
		scale *= ratio;
		stride >>= 1;
  }

  for (int i = 0; i < size*size; i++)
  {
    printf("%f ", data[i]);
    if (i%(size-1) == 0 && i > 0)
      printf("\n");
  }

}

float* Fractal::GetData()
{
  return data;
}

int Fractal::GetSize()
{
  return size;
}
