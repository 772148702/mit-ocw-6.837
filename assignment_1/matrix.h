#ifndef _MATRIX_H_
#define _MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <math.h>
#include <assert.h>

#include "vectors.h"

// ====================================================================
// ====================================================================

class Matrix {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Matrix() { Clear(); }
  Matrix(const Matrix& m);
  ~Matrix() {}
  
  // ACCESSOR
  float Get(int x, int y) const { 
    assert (x >= 0 && x < 4);
    assert (y >= 0 && y < 4);
    return data[y][x]; }
  
  // MODIFIERS
  void Set(int x, int y, float v) {
    assert (x >= 0 && x < 4);
    assert (y >= 0 && y < 4);
    data[y][x] = v; }
  void SetToIdentity();
  void Clear();

  void Transpose(Matrix &m) const;
  int Inverse(Matrix &m, float epsilon = 1e-08) const;
  int Inverse(float epsilon = 1e-08) { return Inverse(*this,epsilon); }

  // OVERLOADED OPERATORS
  Matrix& operator=(const Matrix& m);
  int operator==(const Matrix& m) const;
  int operator!=(const Matrix &m) const { return !(*this==m); }
  friend Matrix operator+(const Matrix &m1, const Matrix &m2);
  friend Matrix operator-(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, float f);
  friend Matrix operator*(float f, const Matrix &m) { return m * f; }
  Matrix& operator+=(const Matrix& m) { *this = *this + m; return *this; }
  Matrix& operator-=(const Matrix& m) { *this = *this - m; return *this; }
  Matrix& operator*=(const float f)   { *this = *this * f; return *this; }
  Matrix& operator*=(const Matrix& m) { *this = *this * m; return *this; }

  // TRANSFORMATIONS
  void Translate(float x, float y, float z);
  void Scale(float x, float y, float z);
  void Scale(float s) { Scale(s, s, s); }
  void XRotate(float theta);
  void YRotate(float theta);
  void ZRotate(float theta);
  void Transform(Vec4f &v);
  void Transform(Vec3f &v) {
    Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),1);
    Transform(v2);
    v.Set(v2.x(),v2.y(),v2.z()); }
  void Transform(Vec2f &v) {
    Vec4f v2 = Vec4f(v.x(),v.y(),1,1);
    Transform(v2);
    v.Set(v2.x(),v2.y()); }

  // INPUT / OUTPUT
  void Write(FILE *F = stdout) const;
  void Write3x3(FILE *F = stdout) const;
  void Read(FILE *F);
  void Read3x3(FILE *F);
  
private:

  // HELPER FUNCTIONS
  void MultRow(int row, float row_factor);
  void SubtractRow(int row1, int row2);


  // REPRESENTATION
  float	data[4][4];

};

// ====================================================================
// ====================================================================

#endif
