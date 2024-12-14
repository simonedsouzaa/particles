#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

namespace Matrices
{
    class Matrix
    {
        public:
            ///Construct a matrix of the specified size.
            ///Initialize each element to 0.
            Matrix(int _rows, int _cols) : rows(_rows), cols(_cols)
            {
                a.resize(rows, vector<double>(cols, 0));
            }
            ///************************************
            ///inline accessors / mutators, these are done:

            ///Read element at row i, column j
            ///usage:  double x = a(i,j);
            const double& operator()(int i, int j) const
            {
                return a.at(i).at(j);
            }

            ///Assign element at row i, column j
            ///usage:  a(i,j) = x;
            double& operator()(int i, int j)
            {
                return a.at(i).at(j);
            }

            int getRows() const{return rows;}
            int getCols() const{return cols;}
            ///************************************
        protected:
            ///changed to protected so sublasses can modify
            vector<vector<double>> a;
        private:
            int rows;
            int cols;
    };

    ///Add each corresponding element.
    ///usage:  c = a + b;
    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
            throw invalid_argument("Matrix dimensions must match for addition.");

        Matrix result(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); ++i)
            for (int j = 0; j < a.getCols(); ++j)
                result(i, j) = a(i, j) + b(i, j);
        return result;
    }

    ///Matrix multiply.  See description.
    ///usage:  c = a * b;
    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getRows())
            throw invalid_argument("Matrix dimensions must match for multiplication.");

        Matrix result(a.getRows(), b.getCols());
        for (int i = 0; i < a.getRows(); ++i)
        {
            for (int j = 0; j < b.getCols(); ++j)
            {
                for (int k = 0; k < a.getCols(); ++k)
                {
                    result(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        return result;
    }

    ///Matrix comparison.  See description.
    ///usage:  a == b
    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
            return false;

        for (int i = 0; i < a.getRows(); ++i)
            for (int j = 0; j < a.getCols(); ++j)
                if (a(i, j) != b(i, j))
                    return false;
        return true;
    }

    ///Matrix comparison.  See description.
    ///usage:  a != b
    bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

    ///Output matrix.
    ///Separate columns by ' ' and rows by '\n'
    ostream& operator<<(ostream& os, const Matrix& m)
    {
        for (int i = 0; i < m.getRows(); ++i)
        {
            for (int j = 0; j < m.getCols(); ++j)
                os << m(i, j) << " ";
            os << "\n";
        }
        return os;
    }

    /*******************************************************************************/

    ///2D rotation matrix
    ///usage:  A = R * A rotates A theta radians counter-clockwise
    class RotationMatrix : public Matrix
    {
        public:
            RotationMatrix(double theta) : Matrix(2, 2)
            {
                a[0][0] = cos(theta);
                a[0][1] = -sin(theta);
                a[1][0] = sin(theta);
                a[1][1] = cos(theta);
            }
    };

    ///2D scaling matrix
    ///usage:  A = S * A expands or contracts A by the specified scaling factor
    class ScalingMatrix : public Matrix
    {
        public:
           ScalingMatrix(double scale) : Matrix(2, 2)
            {
                a[0][0] = scale;
                a[0][1] = 0;
                a[1][0] = 0;
                a[1][1] = scale;
            }
    };

    ///2D Translation matrix
    ///usage:  A = T + A will shift all coordinates of A by (xShift, yShift)
    class TranslationMatrix : public Matrix
    {
        public:
            TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
            {
                for (int i = 0; i < nCols; ++i)
                {
                    a[0][i] = xShift;
                    a[1][i] = yShift;
                }
            }
    };
}

#endif // MATRIX_H_INCLUDED
