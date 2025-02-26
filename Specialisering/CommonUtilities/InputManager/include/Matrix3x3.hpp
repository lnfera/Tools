#pragma once
#include <cassert>
#include "Vector3.h"
#include "Matrix4x4.hpp"


namespace CommonUtilities
{
	template <typename T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix. 
		Matrix3x3<T>();
		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);
		inline void TotalReset();

		inline Matrix3x3<T> GetFastInverse();
		void operator=(const Matrix3x3<T>& aMatrix3x3);

	private:
		T myData[3][3];
	};
	//いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
	template <typename T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow <= 3 && "Selected Row is higher than number of rows");
		assert(aRow >= 1 && "Selected Row is lower than number of rows");

		assert(aColumn <= 3 && "Selected Row is higher than number of rows");
		assert(aColumn >= 1 && "Selected Row is lower than number of rows");

		return myData[aRow - 1][aColumn - 1];
	}
	template <typename T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow <= 3 && "Selected Row is higher than number of rows");
		assert(aRow >= 1 && "Selected Row is lower than number of rows");

		assert(aColumn <= 3 && "Selected Row is higher than number of rows");
		assert(aColumn >= 1 && "Selected Row is lower than number of rows");

		return myData[aRow - 1][aColumn - 1];
	}
	//いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい
	template <typename T>
	Matrix3x3<T>::Matrix3x3<T>()
	{
		myData[0][0] = 1;
		myData[0][1] = 0;
		myData[0][2] = 0;

		myData[1][0] = 0;
		myData[1][1] = 1;
		myData[1][2] = 0;

		myData[2][0] = 0;
		myData[2][1] = 0;
		myData[2][2] = 1;

	}
	template <typename T>
	Matrix3x3<T>::Matrix3x3<T>(const Matrix3x3<T>& aMatrix)
	{
		myData[0][0] = aMatrix.myData[0][0];
		myData[0][1] = aMatrix.myData[0][1];
		myData[0][2] = aMatrix.myData[0][2];

		myData[1][0] = aMatrix.myData[1][0];
		myData[1][1] = aMatrix.myData[1][1];
		myData[1][2] = aMatrix.myData[1][2];

		myData[2][0] = aMatrix.myData[2][0];
		myData[2][1] = aMatrix.myData[2][1];
		myData[2][2] = aMatrix.myData[2][2];
	}
	template <typename T>
	Matrix3x3<T>::Matrix3x3<T>(const Matrix4x4<T>& aMatrix)
	{
		myData[0][0] = aMatrix(1, 1);
		myData[0][1] = aMatrix(1, 2);
		myData[0][2] = aMatrix(1, 3);

		myData[1][0] = aMatrix(2, 1);
		myData[1][1] = aMatrix(2, 2);
		myData[1][2] = aMatrix(2, 3);

		myData[2][0] = aMatrix(3, 1);
		myData[2][1] = aMatrix(3, 2);
		myData[2][2] = aMatrix(3, 3);
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp(1,1) = 1;
		temp(1,2) = 0;
		temp(1,3) = 0;

		temp(2,1) = 0;
		temp(2,2) = cos(aAngleInRadians);
		temp(2,3) = sin(aAngleInRadians);

		temp(3,1) = 0;
		temp(3,2) = -sin(aAngleInRadians);
		temp(3,3) = cos(aAngleInRadians);

		return temp;
	}
	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp(1, 1) =  cos(aAngleInRadians);
		temp(1, 2) = 0;
		temp(1, 3) =  -sin(aAngleInRadians);

		temp(2, 1) = 0;
		temp(2, 2) = 1;
		temp(2, 3) = 0; 

		temp(3, 1) = sin(aAngleInRadians);
		temp(3, 2) = 0;
		temp(3, 3) = cos(aAngleInRadians);

		return temp;
	}
	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix3x3<T> temp;

		temp(1, 1) = cos(aAngleInRadians);
		temp(1, 2) = sin(aAngleInRadians);
		temp(1, 3) = 0;

		temp(2, 1) = -sin(aAngleInRadians);
		temp(2, 2) = cos(aAngleInRadians);
		temp(2, 3) = 0;

		temp(3, 1) = 0;
		temp(3, 2) = 0;
		temp(3, 3) = 1;

		return temp;
	}

	template <typename T>
	Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> temp;

		temp(1, 1) = aMatrixToTranspose(1, 1);
		temp(1, 2) = aMatrixToTranspose(2, 1);
		temp(1, 3) = aMatrixToTranspose(3, 1);

		temp(2, 1) = aMatrixToTranspose(1, 2);
		temp(2, 2) = aMatrixToTranspose(2, 2);
		temp(2, 3) = aMatrixToTranspose(3, 2);

		temp(3, 1) = aMatrixToTranspose(1, 3);
		temp(3, 2) = aMatrixToTranspose(2, 3);
		temp(3, 3) = aMatrixToTranspose(3, 3);

		return temp;
	}

	template <typename T>
	inline void  Matrix3x3<T>::TotalReset()
	{
		myData[0][0] = 0;
		myData[0][1] = 0;
		myData[0][2] = 0;

		myData[1][0] = 0;
		myData[1][1] = 0;
		myData[1][2] = 0;

		myData[2][0] = 0;
		myData[2][1] = 0;
		myData[2][2] = 0;
	}

	//いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい

	template<typename T>
	const Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		Matrix3x3<T> temp;
		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				temp(i, j) = aMatrix3x3(i, j) + bMatrix3x3(i, j);
			}
		}
		return temp;
	}
	template<typename T>
	void operator+=(Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{

		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				aMatrix3x3(i, j) += bMatrix3x3(i, j);
			}
		}
	}

	template<typename T>
	const Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		Matrix3x3<T> temp;
		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				temp(i, j) = aMatrix3x3(i, j) - bMatrix3x3(i, j);
			}
		}
		return temp;
	}
	template<typename T>
	void operator-=(Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				aMatrix3x3(i, j) -= bMatrix3x3(i, j);
			}
		}
	}


	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetFastInverse()
	{
		Matrix3x3<T> A = aMatrix;
		double determinant = +A(0, 0) * (A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2))
			- A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0))
			+ A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));
		double invdet = 1 / determinant;
		Matrix3x3<T> result;
		result(0, 0) = (A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2)) * invdet;
		result(1, 0) = -(A(0, 1) * A(2, 2) - A(0, 2) * A(2, 1)) * invdet;
		result(2, 0) = (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) * invdet;
		result(0, 1) = -(A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0)) * invdet;
		result(1, 1) = (A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0)) * invdet;
		result(2, 1) = -(A(0, 0) * A(1, 2) - A(1, 0) * A(0, 2)) * invdet;
		result(0, 2) = (A(1, 0) * A(2, 1) - A(2, 0) * A(1, 1)) * invdet;
		result(1, 2) = -(A(0, 0) * A(2, 1) - A(2, 0) * A(0, 1)) * invdet;
		result(2, 2) = (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) * invdet;
		return result;
	}

	template<typename T>
	void Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix3x3)
	{
		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				myData[i - 1][j - 1] = aMatrix3x3.myData[i - 1][j - 1];
			}
		}
	}
	template <typename T>
	bool operator==(const Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		for (int i = 1; i <= 3; i++)//rows 
		{
			for (int j = 1; j <= 3; j++) // colums
			{
				if (aMatrix3x3(i, j) != bMatrix3x3(i, j)) { return false; }
			}
		}
		return true;
	}


	template<typename T>
	const Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		Matrix3x3<T> temp;
		temp.TotalReset();
		for (int a = 1; a <= 3; a++)//Matris A rader
		{
			for (int b = 1; b <= 3; b++) //Matris B column
			{
				for (int c = 1; c <= 3; c++) //Matris A column
				{
					temp(a, b) += aMatrix3x3(a, c) * bMatrix3x3(c, b);
				}
			}
		}
		return temp;
	}
	template<typename T>
	void operator*=(Matrix3x3<T>& aMatrix3x3, const Matrix3x3<T>& bMatrix3x3)
	{
		Matrix3x3<T> temp;
		temp.TotalReset();
		for (int a = 1; a <= 3; a++)//Matris A rader
		{
			for (int b = 1; b <= 3; b++) //Matris B column
			{
				for (int c = 1; c <= 3; c++) //Matris A column
				{
					temp(a, b) += aMatrix3x3(a, c) * bMatrix3x3(c, b);
				}
			}
		}
		aMatrix3x3 = temp;
	}
	template<typename T>
	const Vector3<T> operator*(const Matrix3x3<T>& aMatrix3x3, const Vector3<T>& aVector)
	{
		Vector3<T> temp;
		temp.x = (aVector.x * aMatrix3x3(1, 1)) + (aVector.y * aMatrix3x3(1, 2)) + (aVector.z * aMatrix3x3(1, 3));
		temp.y = (aVector.x * aMatrix3x3(2, 1)) + (aVector.y * aMatrix3x3(2, 2)) + (aVector.z * aMatrix3x3(2, 3));
		temp.z = (aVector.x * aMatrix3x3(3, 1)) + (aVector.y * aMatrix3x3(3, 2)) + (aVector.z * aMatrix3x3(3, 3));
		return temp;
	}
	template<typename T>
	Vector3<T> operator*=(const Matrix3x3<T>& aMatrix3x3,  Vector3<T>& aVector)
	{
		Vector3<T> temp;
		temp.x = (aVector.x * aMatrix3x3(1, 1)) + (aVector.y * aMatrix3x3(1, 2)) + (aVector.z * aMatrix3x3(1, 3));
		temp.y = (aVector.x * aMatrix3x3(2, 1)) + (aVector.y * aMatrix3x3(2, 2)) + (aVector.z * aMatrix3x3(2, 3));
		temp.z = (aVector.x * aMatrix3x3(3, 1)) + (aVector.y * aMatrix3x3(3, 2)) + (aVector.z * aMatrix3x3(3, 3));
		aVector = temp;
		return aVector;

	}
	template<typename T>
	const Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T> aMatrix3x3)
	{
		Vector3<T> temp;
		temp.x = (aVector.x * aMatrix3x3(1, 1)) + (aVector.y * aMatrix3x3(2, 1)) + (aVector.z * aMatrix3x3(3, 1));
		temp.y = (aVector.x * aMatrix3x3(1, 2)) + (aVector.y * aMatrix3x3(2, 2)) + (aVector.z * aMatrix3x3(3, 2));
		temp.z = (aVector.x * aMatrix3x3(1, 3)) + (aVector.y * aMatrix3x3(2, 3)) + (aVector.z * aMatrix3x3(3, 3));
		return temp;
	}
	template<typename T>
	void operator*=(Vector3<T>& aVector, const Matrix3x3<T> aMatrix3x3)
	{
		Vector3<T> temp;
		temp.x = (aVector.x * aMatrix3x3(1, 1)) + (aVector.y * aMatrix3x3(2, 1)) + (aVector.z * aMatrix3x3(3, 1));
		temp.y = (aVector.x * aMatrix3x3(1, 2)) + (aVector.y * aMatrix3x3(2, 2)) + (aVector.z * aMatrix3x3(3, 2));
		temp.z = (aVector.x * aMatrix3x3(1, 3)) + (aVector.y * aMatrix3x3(2, 3)) + (aVector.z * aMatrix3x3(3, 3));
		aVector = temp;
	}
}
namespace CU = CommonUtilities;

