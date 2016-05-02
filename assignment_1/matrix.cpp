//
// originally implemented by Justin Legakis
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matrix.h"
#include "vectors.h"

// ===================================================================
// ===================================================================
// COPY CONSTRUCTOR

Matrix::Matrix(const Matrix& m) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      data[y][x] = m.data[y][x]; }
  }
}

// ===================================================================
// ===================================================================
// MODIFIERS

void Matrix::SetToIdentity() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      data[y][x] = (x == y); 
    }
  }
}

void Matrix::Clear() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      data[y][x] = 0; 
    }
  }
}

void Matrix::Transpose(Matrix &m) const {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      m.data[y][x] = data[x][y];
    }
  }
}

int Matrix::Inverse(Matrix &m, float epsilon) const {
  // by Gauss-Jordan Elimination
  Matrix tmp; tmp = *this;
  m.SetToIdentity();
  int i,j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (i == j) continue;
      if (fabs(tmp.Get(i,j)) < epsilon) continue;
      if (fabs(tmp.Get(i,i)) < epsilon) {
	printf ("WARNING! Not invertible\n");
	return 0;
      }
      float row_factor = tmp.Get(i,i) / tmp.Get(i,j);
      tmp.MultRow(j,row_factor);
      m.MultRow(j,row_factor);
      tmp.SubtractRow(j,i);
      m.SubtractRow(j,i);
    }
  }
  for (i = 0; i < 4; i++) {
    if (fabs(tmp.Get(i,i)) < 0) {
      printf ("WARNING! Not invertible\n");
      assert(0);
    }
    float row_factor = 1 / tmp.Get(i,i);
    tmp.MultRow(i,row_factor);
    m.MultRow(i,row_factor);
  }
  return 1;
}

// HELPERS
void Matrix::MultRow(int row, float row_factor) {
  for (int i = 0; i < 4; i++) {
    this->data[row][i] *= row_factor;
  }
}

void Matrix::SubtractRow(int row1, int row2) {
  for (int i = 0; i < 4; i++) {
    this->data[row1][i] -= this->data[row2][i];
  }
}

// ===================================================================
// ====================================================================
// OVERLOADED OPERATORS

Matrix& Matrix::operator=(const Matrix& m) {
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      data[y][x] = m.data[y][x]; 
    }
  }
  return (*this); 
}

int Matrix::operator==(const Matrix& m) const {
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      if (this->data[y][x] != m.data[y][x]) {
	return 0; 
      }
    }
  }
  return 1; 
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
  Matrix answer;
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      answer.data[y][x] = m1.data[y][x] + m2.data[y][x];
    }
  }
  return answer; 
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
  Matrix answer;
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      answer.data[y][x] = m1.data[y][x] - m2.data[y][x];
    }
  }
  return answer; 
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
  Matrix answer;
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      for (int i=0; i<4; i++) {
	answer.data[y][x] 
	  += m1.data[y][i] * m2.data[i][x];
      }
    }
  }
  return answer;
}

Matrix operator*(const Matrix& m, float f) {
  Matrix answer;
  for (int y=0; y<4; y++) {
    for (int x=0; x<4; x++) {
      answer.data[y][x] = m.data[y][x] * f;
    }
  }
  return answer;
}

// ====================================================================
// ====================================================================
// TRANSFORMATIONS

void Matrix::Translate(float t_x, float t_y, float t_z) {
  Matrix t;
  t.SetToIdentity();
  t.data[0][3] = t_x;
  t.data[1][3] = t_y;
  t.data[2][3] = t_z;
  t *= *this;
  *this = t;
}

void Matrix::Scale(float s_x, float s_y, float s_z) {
  Matrix s; 
  s.SetToIdentity();
  s.data[0][0] = s_x;
  s.data[1][1] = s_y;
  s.data[2][2] = s_z;
  s.data[3][3] = 1;
  s *= *this;
  *this = s;
}

void Matrix::XRotate(float theta) {
  Matrix rx;
  rx.SetToIdentity();
  rx.data[1][1]= (float)cos((float)theta);
  rx.data[1][2]=-(float)sin((float)theta);
  rx.data[2][1]= (float)sin((float)theta);
  rx.data[2][2]= (float)cos((float)theta);
  rx *= *this;
  *this = rx;
}

void Matrix::YRotate(float theta) {
  Matrix ry;
  ry.SetToIdentity();
  ry.data[0][0]= (float)cos((float)theta);
  ry.data[0][2]= (float)sin((float)theta);
  ry.data[2][0]=-(float)sin((float)theta);
  ry.data[2][2]= (float)cos((float)theta);
  ry *= *this;
  *this = ry;
}

void Matrix::ZRotate(float theta) {
  Matrix rz;
  rz.SetToIdentity();
  rz.data[0][0]= (float)cos((float)theta);
  rz.data[0][1]=-(float)sin((float)theta);
  rz.data[1][0]= (float)sin((float)theta);
  rz.data[1][1]= (float)cos((float)theta);
  rz *= *this;
  *this = rz;
}

// ====================================================================
// ====================================================================

void Matrix::Transform(Vec4f &v) {
  Vec4f answer;
  for (int y=0; y<4; y++) {
    answer.data[y] = 0;
    for (int i=0; i<4; i++) {
      answer.data[y] += data[y][i] * v[i];
    }
  }
  v = answer;
}

// ====================================================================
// ====================================================================

void Matrix::Write(FILE *F) const {
  assert (F != NULL);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      fprintf (F, "%12.6f ", data[y][x]); 
    }
    fprintf (F,"\n"); 
  } 
}

void Matrix::Write3x3(FILE *F) const {
  assert (F != NULL);
  for (int y = 0; y < 4; y++) {
    if (y == 2) continue;
    for (int x = 0; x < 4; x++) {
      if (x == 2) continue;
      fprintf (F, "%12.6f ", data[y][x]); 
    }
    fprintf (F,"\n"); 
  } 
}

void Matrix::Read(FILE *F) {
  assert (F != NULL);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      int scanned = fscanf (F,"%f",&data[y][x]);
      assert (scanned == 1); 
    }
  } 
}

void Matrix::Read3x3(FILE *F) {
  assert (F != NULL);
  Clear();
  for (int y = 0; y < 4; y++) {
    if (y == 2) continue;
    for (int x = 0; x < 4; x++) {
      if (x == 2) continue;
      int scanned = fscanf (F,"%f",&data[y][x]);
      assert (scanned == 1); 
    } 
  } 
}

// ====================================================================
// ====================================================================
