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