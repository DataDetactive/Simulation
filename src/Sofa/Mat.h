/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_DEFAULTTYPE_MAT_H
#define SOFA_DEFAULTTYPE_MAT_H

#include <Sofa/Vec.h>
#include <assert.h>
#include <iostream>

namespace sofa
{

namespace defaulttype
{

using std::cerr;
using std::endl;
template <int L, int C, class real=double>
class Mat : public helper::fixed_array<VecNoInit<C,real>,L>
{
 public:

  enum { N = L*C };

  typedef real Real;
  typedef Vec<C,real> Line;
  typedef VecNoInit<C,real> LineNoInit;
  typedef Vec<L,real> Col;

    Mat()
    {
	clear();
    }


  /// Specific constructor with 2 lines.
  Mat(Line r1, Line r2)
  {
    this->elems[0]=r1;
    this->elems[1]=r2;
  }

  /// Specific constructor with 3 lines.
  Mat(Line r1, Line r2, Line r3)
  {
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
  }

  /// Specific constructor with 4 lines.
  Mat(Line r1, Line r2, Line r3, Line r4)
  {
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
    this->elems[3]=r4;
  }

  /// Specific constructor with 5 lines.
  Mat(Line r1, Line r2, Line r3, Line r4, Line r5)
  {
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
    this->elems[3]=r4;
    this->elems[4]=r5;
  }

  /// Specific constructor with 6 lines.
  Mat(Line r1, Line r2, Line r3, Line r4, Line r5, Line r6)
  {
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
    this->elems[3]=r4;
    this->elems[4]=r5;
    this->elems[5]=r6;
  }

  /// Constructor from an element
  explicit Mat(const real& v)
  {
      for( int i=0; i<L; i++ )
          for( int j=0; j<C; j++ )
              this->elems[i][j] = v;
  }

  /// Constructor from another matrix
  template<typename real2>
  Mat(const Mat<L,C,real2>& m)
  {
    std::copy(m.begin(), m.begin()+L, this->begin());
  }

  /// Constructor from an array of elements (stored per line).
  template<typename real2>
  explicit Mat(const real2* p)
  {
    std::copy(p, p+N, this->begin()->begin());
  }

  /// number of lines
  int getNbLines() const
  {
	  return L;
  }

  /// number of colums
  int getNbCols() const
  {
	  return C;
  }


  /// Assignment from an array of elements (stored per line).
  void operator=(const real* p)
  {
    std::copy(p, p+N, this->begin()->begin());
  }

  /// Assignment from another matrix
  template<typename real2> void operator=(const Mat<L,C,real2>& m)
  {
    std::copy(m.begin(), m.begin()+L, this->begin());
  }

  /// Assignment from a matrix of different size.
  template<int L2, int C2> void operator=(const Mat<L2,C2,real>& m)
  {
    std::copy(m.begin(), m.begin()+(L>L2?L2:L), this->begin());
  }

  template<int L2, int C2> void getsub(int L0, int C0, Mat<L2,C2,real>& m) const
  {
    for (int i=0;i<L2;i++)
      for (int j=0;j<C2;j++)
        m[i][j] = this->elems[i+L0][j+C0];
  }

  template<int L2, int C2> void setsub(int L0, int C0, const Mat<L2,C2,real>& m)
  {
    for (int i=0;i<L2;i++)
      for (int j=0;j<C2;j++)
        this->elems[i+L0][j+C0] = m[i][j];
  }

  /// Sets each element to 0.
  void clear()
  {
    for (int i=0;i<L;i++)
      this->elems[i].clear();
  }

  /// Sets each element to r.
  void fill(real r)
  {
    for (int i=0;i<L;i++)
      this->elems[i].fill(r);
  }

  /// Read-only access to line i.
  const Line& line(int i) const
  {
    return this->elems[i];
  }

  /// Copy of column j.
  Col col(int j) const
  {
    Col c;
    for (int i=0;i<L;i++)
      c[i]=this->elems[i][j];
    return c;
  }

  /// Write acess to line i.
  LineNoInit& operator[](int i)
  {
    return this->elems[i];
  }

  /// Read-only access to line i.
  const LineNoInit& operator[](int i) const
  {
    return this->elems[i];
  }

  /// Write acess to line i.
  LineNoInit& operator()(int i)
  {
    return this->elems[i];
  }

  /// Read-only access to line i.
  const LineNoInit& operator()(int i) const
  {
    return this->elems[i];
  }

  /// Write access to element (i,j).
  real& operator()(int i, int j)
  {
    return this->elems[i][j];
  }

  /// Read-only access to element (i,j).
  const real& operator()(int i, int j) const
  {
    return this->elems[i][j];
  }

  /// Cast into a standard C array of lines (read-only).
  const Line* lptr() const
  {
    return this->elems;
  }

  /// Cast into a standard C array of lines.
  Line* lptr()
  {
    return this->elems;
  }

  /// Cast into a standard C array of elements (stored per line) (read-only).
  const real* ptr() const
  {
	  return this->elems[0].ptr();;
  }

  /// Cast into a standard C array of elements (stored per line).
  real* ptr()
  {
	  return this->elems[0].ptr();
  }

  /// Special access to first line.
  Line& x() { return this->elems[0]; }
  /// Special access to second line.
  Line& y() { return this->elems[1]; }
  /// Special access to third line.
  Line& z() { return this->elems[2]; }
  /// Special access to fourth line.
  Line& w() { return this->elems[3]; }

  /// Special access to first line (read-only).
  const Line& x() const { return this->elems[0]; }
  /// Special access to second line (read-only).
  const Line& y() const { return this->elems[1]; }
  /// Special access to thrid line (read-only).
  const Line& z() const { return this->elems[2]; }
  /// Special access to fourth line (read-only).
  const Line& w() const { return this->elems[3]; }

  /// Set matrix to identity.
  void identity()
  {
    clear();
    for (int i=0;i<L;i++)
      this->elems[i][i]=1;
  }

  /// Set matrix as the transpose of m.
  void transpose(const Mat<C,L,real> &m)
  {
    for (int i=0;i<L;i++)
      for (int j=0;j<C;j++)
        this->elems[i][j]=m[j][i];
  }

  /// Return the transpose of m.
  Mat<C,L,real> transpose() const
  {
    Mat<C,L,real> m(NOINIT);
    for (int i=0;i<L;i++)
      for (int j=0;j<C;j++)
        m[j][i]=this->elems[i][j];
    return m;
  }

//  /// Transpose current matrix.
//  void transposed()
//  {
//    for (int i=0;i<L;i++)
//      for (int j=i+1;j<C;j++)
//      {
//	real t = this->elems[i][j];
//	this->elems[i][j] = this->elems[j][i];
//	this->elems[j][i] = t;
//      }
//  }

  Mat<L,C,real> inverse() {
      Mat<L,C,real> res;
      invertMatrix(res,*this);
      return res;
  }

    /// @name Tests operators
    /// @{

    bool operator==(const Mat<L,C,real>& b) const
    {
        for (int i=0;i<L;i++)
            if (!(this->elems[i]==b[i])) return false;
        return true;
    }

    bool operator!=(const Mat<L,C,real>& b) const
    {
        for (int i=0;i<L;i++)
            if (this->elems[i]!=b[i]) return true;
        return false;
    }


	bool isSymetric() const
	{
		for (int i=0;i<L;i++)
			for (int j=i+1;j<C;j++)
				if( fabs( this->elems[i][j] - this->elems[j][i] ) > EQUALITY_THRESHOLD ) return false;
	   return true;
	}


    /// @}

  // LINEAR ALGEBRA

  /// Matrix multiplication operator.
  template <int P>
  Mat<L,P,real> operator*(const Mat<C,P,real>& m) const
  {
    Mat<L,P,real> r(NOINIT);
    for(int i=0;i<L;i++)
      for(int j=0;j<P;j++)
      {
	r[i][j]=(*this)[i][0] * m[0][j];
	for(int k=1;k<C;k++)
	  r[i][j] += (*this)[i][k] * m[k][j];
      }
    return r;
  }

  /// Matrix addition operator.
  Mat<L,C,real> operator+(const Mat<L,C,real>& m) const
  {
    Mat<L,C,real> r(NOINIT);
    for(int i = 0; i < L; i++)
      r[i] = (*this)[i] + m[i];
    return r;
  }

  /// Matrix subtraction operator.
  Mat<L,C,real> operator-(const Mat<L,C,real>& m) const
  {
    Mat<L,C,real> r(NOINIT);
    for(int i = 0; i < L; i++)
      r[i] = (*this)[i] - m[i];
    return r;
  }

  /// Multiplication operator Matrix * Line.
  Col operator*(const Line& v) const
  {
    Col r(NOINIT);
    for(int i=0;i<L;i++)
    {
      r[i]=(*this)[i][0] * v[0];
      for(int j=1;j<C;j++)
	r[i] += (*this)[i][j] * v[j];
    }
    return r;
  }

  /// Multiplication of the transposed Matrix * Column
  Line multTranspose(const Col& v) const
  {
      Line r(NOINIT);
      for(int i=0;i<C;i++)
      {
          r[i]=(*this)[0][i] * v[0];
          for(int j=1;j<L;j++)
              r[i] += (*this)[j][i] * v[j];
      }
      return r;
  }


  /// Transposed Matrix multiplication operator.
  template <int P>
  Mat<C,P,real> multTranspose(const Mat<L,P,real>& m) const
  {
    Mat<C,P,real> r(NOINIT);
    for(int i=0;i<C;i++)
      for(int j=0;j<P;j++)
      {
	r[i][j]=(*this)[0][i] * m[0][j];
	for(int k=1;k<L;k++)
	  r[i][j] += (*this)[k][i] * m[k][j];
      }
    return r;
  }


  /// Scalar multiplication operator.
  Mat<L,C,real> operator*(real f) const
  {
    Mat<L,C,real> r(NOINIT);
    for(int i=0;i<L;i++)
      for(int j=0;j<C;j++)
	r[i][j] = (*this)[i][j] * f;
    return r;
  }

  /// Scalar matrix multiplication operator.
  friend Mat<L,C,real> operator*(real r, const Mat<L,C,real>& m)
  {
    return m*r;
  }

  /// Scalar division operator.
  Mat<L,C,real> operator/(real f) const
  {
    Mat<L,C,real> r(NOINIT);
	  for(int i=0;i<L;i++)
		  for(int j=0;j<C;j++)
			  r[i][j] = (*this)[i][j] / f;
	  return r;
  }

  /// Scalar multiplication assignment operator.
  void operator *=(real r)
  {
    for(int i=0;i<L;i++)
      this->elems[i]*=r;
  }

  /// Scalar division assignment operator.
  void operator /=(real r)
  {
    for(int i=0;i<L;i++)
      this->elems[i]/=r;
  }

  /// Addition assignment operator.
  void operator +=(const Mat<L,C,real>& m)
  {
    for(int i=0;i<L;i++)
      this->elems[i]+=m[i];
  }

  /// Addition of the transposed of m (possible for square matrices only)
  void addTransposed(const Mat<L,C,real>& m)
  {
      for(int i=0;i<L;i++)
          for(int j=0;j<L;j++)
              (*this)[i][j] += m[j][i];
  }

  /// Substraction of the transposed of m (possible for square matrices only)
  void subTransposed(const Mat<L,C,real>& m)
  {
      for(int i=0;i<L;i++)
          for(int j=0;j<L;j++)
              (*this)[i][j] += m[j][i];
  }

  /// Substraction assignment operator.
  void operator -=(const Mat<L,C,real>& m)
  {
    for(int i=0;i<L;i++)
      this->elems[i]-=m[i];
  }

  /// Invert matrix m and return determinant
  real invert(Mat<L,C,real>& m) const
  {
    return invertMatrix(m, *this);
  }



  /// Multiplication operator Matrix * Vector considering the matrix as a transformation.
  Vec<C-1,real> transform(const Vec<C-1,real>& v) const
  {
	Vec<C-1,real> r(NOINIT);
    for(int i=0;i<C-1;i++)
    {
      r[i]=(*this)[i][0] * v[0];
      for(int j=1;j<C-1;j++)
    	  r[i] += (*this)[i][j] * v[j];
	  r[i] += (*this)[i][C-1];
    }
    return r;
  }
};


/// Determinant of a 3x3 matrix.
template<class real>
inline real determinant(const Mat<3,3,real>& m)
{
	return m(0,0)*m(1,1)*m(2,2)
	     + m(1,0)*m(2,1)*m(0,2)
	     + m(2,0)*m(0,1)*m(1,2)
	     - m(0,0)*m(2,1)*m(1,2)
	     - m(1,0)*m(0,1)*m(2,2)
	     - m(2,0)*m(1,1)*m(0,2);
}

/// Determinant of a 2x2 matrix.
template<class real>
inline real determinant(const Mat<2,2,real>& m)
{
	return m(0,0)*m(1,1)
	     - m(1,0)*m(0,1);
}

#define MIN_DETERMINANT  1.0e-100

template<int S, class real>
real invertMatrix(Mat<S,S,real>& dest, const Mat<S,S,real>& from) {
    invertMatrix(dest, from);
    return (determinant(dest));
}

/// Matrix inversion (general case).
template<int S, class real>
bool invertMatrix(Mat<S,S,real>& dest, const Mat<S,S,real>& from)
{
  int i, j, k;
  Vec<S,int> r, c, row, col;

  Mat<S,S,real> m1 = from;
  Mat<S,S,real> m2;
  m2.identity();

  for ( k = 0; k < S; k++ )
  {
    // Choosing the pivot
    real pivot = 0;
    for (i = 0; i < S; i++)
    {
      if (row[i])
	continue;
      for (j = 0; j < S; j++)
      {
	if (col[j])
	  continue;
	real t = m1[i][j]; if (t<0) t=-t;
	if ( t > pivot)
	{
	  pivot = t;
	  r[k] = i;
	  c[k] = j;
	}
      }
    }

    if (pivot <= (real) MIN_DETERMINANT)
    {
      cerr<<"Warning: invertMatrix finds too small determinant, matrix = "<<from<<endl;
      return false;
    }

    row[r[k]] = col[c[k]] = 1;
    pivot = m1[r[k]][c[k]];

    // Normalization
    m1[r[k]] /= pivot; m1[r[k]][c[k]] = 1;
    m2[r[k]] /= pivot;

    // Reduction
    for (i = 0; i < S; i++)
    {
      if (i != r[k])
      {
	real f = m1[i][c[k]];
	m1[i] -= m1[r[k]]*f; m1[i][c[k]] = 0;
	m2[i] -= m2[r[k]]*f;
      }
    }
  }

  for (i = 0; i < S; i++)
    for (j = 0; j < S; j++)
      if (c[j] == i)
	row[i] = r[j];

  for ( i = 0; i < S; i++ )
    dest[i] = m2[row[i]];

  return true;
}

/// Matrix inversion (special case 3x3).
template<class real>
real invertMatrix(Mat<3,3,real>& dest, const Mat<3,3,real>& from)
{
  real det=determinant(from);

  if ( -(real) MIN_DETERMINANT<=det && det<=(real) MIN_DETERMINANT){
    cerr<<"Warning: invertMatrix finds too small determinant, matrix = "<<from<<endl;
    return false;
  }

  dest(0,0)= (from(1,1)*from(2,2) - from(2,1)*from(1,2))/det;
  dest(1,0)= (from(1,2)*from(2,0) - from(2,2)*from(1,0))/det;
  dest(2,0)= (from(1,0)*from(2,1) - from(2,0)*from(1,1))/det;
  dest(0,1)= (from(2,1)*from(0,2) - from(0,1)*from(2,2))/det;
  dest(1,1)= (from(2,2)*from(0,0) - from(0,2)*from(2,0))/det;
  dest(2,1)= (from(2,0)*from(0,1) - from(0,0)*from(2,1))/det;
  dest(0,2)= (from(0,1)*from(1,2) - from(1,1)*from(0,2))/det;
  dest(1,2)= (from(0,2)*from(1,0) - from(1,2)*from(0,0))/det;
  dest(2,2)= (from(0,0)*from(1,1) - from(1,0)*from(0,1))/det;

  return det;
}

template<class real>
real invertMatrix(Mat<4,4,real>& dest, const Mat<4,4,real>& from) {
#define MAT(m,r,c) (m)[c][r]
#define m11 MAT(from,0,0)
#define m12 MAT(from,0,1)
#define m13 MAT(from,0,2)
#define m14 MAT(from,0,3)
#define m21 MAT(from,1,0)
#define m22 MAT(from,1,1)
#define m23 MAT(from,1,2)
#define m24 MAT(from,1,3)
#define m31 MAT(from,2,0)
#define m32 MAT(from,2,1)
#define m33 MAT(from,2,2)
#define m34 MAT(from,2,3)
#define m41 MAT(from,3,0)
#define m42 MAT(from,3,1)
#define m43 MAT(from,3,2)
#define m44 MAT(from,3,3)

    real det;
    real d12, d13, d23, d24, d34, d41;
    real tmp[4]; /* Allow out == in. */

    /* Inverse = adjoint / det. (See linear algebra texts.)*/

    /* pre-compute 2x2 dets for last two rows when computing */
    /* cofactors of first two rows. */
    d12 = (m31*m42-m41*m32);
    d13 = (m31*m43-m41*m33);
    d23 = (m32*m43-m42*m33);
    d24 = (m32*m44-m42*m34);
    d34 = (m33*m44-m43*m34);
    d41 = (m34*m41-m44*m31);

    tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    /* Compute determinant as early as possible using these cofactors. */
    det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    /* Run singularity test. */
    if (det == 0.0) {
        std::cout << "invert_matrix: Warning: Singular matrix.\n" << std::endl;
        //memcpy(out,_identity,16*sizeof(TReal));
    }
    else {
        real invDet = real(1.0) / det;
        /* Compute rest of inverse. */
        dest[0][0] = tmp[0] * invDet;
        dest[0][1] = tmp[1] * invDet;
        dest[0][2] = tmp[2] * invDet;
        dest[0][3] = tmp[3] * invDet;

        dest[1][0] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
        dest[1][1] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
        dest[1][2] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
        dest[1][3] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

        /* Pre-compute 2x2 dets for first two rows when computing */
        /* cofactors of last two rows. */
        d12 = m11*m22-m21*m12;
        d13 = m11*m23-m21*m13;
        d23 = m12*m23-m22*m13;
        d24 = m12*m24-m22*m14;
        d34 = m13*m24-m23*m14;
        d41 = m14*m21-m24*m11;

        dest[2][0] =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
        dest[2][1] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
        dest[2][2] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
        dest[2][3] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
        dest[3][0] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
        dest[3][1] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
        dest[3][2] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
        dest[3][3] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

    }

#undef MAT
#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44

    return(det);
}

/// Matrix inversion (special case 2x2).
template<class real>
real invertMatrix(Mat<2,2,real>& dest, const Mat<2,2,real>& from)
{
  real det=determinant(from);

  if ( -(real) MIN_DETERMINANT<=det && det<=(real) MIN_DETERMINANT){
    cerr<<"Warning: invertMatrix finds too small determinant, matrix = "<<from<<endl;
    return false;
  }

  dest(0,0)=  from(1,1)/det;
  dest(0,1)= -from(0,1)/det;
  dest(1,0)= -from(1,0)/det;
  dest(1,1)=  from(0,0)/det;

  return det;
}
#undef MIN_DETERMINANT

template <int L, int C, typename real>
std::ostream& operator<<(std::ostream& o, const Mat<L,C,real>& m)
{
  o << '[' << m[0];
  for (int i=1; i<L; i++)
    o << ',' << m[i];
  o << ']';
  return o;
}

} // namespace defaulttype

} // namespace sofa

#endif
