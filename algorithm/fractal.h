
//分形算法，用于地形生成，动态云层等，暂时只做地形

#ifndef _FRACTAL_H_
#define _FRACTAL_H_

class Fractal
{
private:
  int size;
  float surfaceH;
  float *data;
public:
  static Fractal* NewInstance();
public:
  Fractal();
  void SetSize(int w);
  void SetCornerValue(float center, float lefttop, float leftbuttom, float righttop, float rightbuttom);
  virtual ~Fractal();
  float* GetData();
  int GetSize();
};

#endif
