#pragma once
#include "Vector.h"
#include <cassert>
namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4
	{
	public:

		Matrix4x4<T>();

		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		inline T& operator()(const int aRow, const int aColumn);
		inline const T& operator()(const int aRow, const int aColumn) const;
				
		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		inline void Matrix4x4<T>::MakeZero();

		void operator=(const Matrix4x4<T>& aMatrix4x4);

	private:
		T myData[4][4];
	};
	//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
	template <typename T>
	inline void  Matrix4x4<T>::MakeZero()
	{
		myData[0][0] = 0;
		myData[0][1] = 0;
		myData[0][2] = 0;
		myData[0][3] = 0;

		myData[1][0] = 0;
		myData[1][1] = 0;
		myData[1][2] = 0;
		myData[1][3] = 0;

		myData[2][0] = 0;
		myData[2][1] = 0;
		myData[2][2] = 0;
		myData[2][3] = 0;

		myData[3][0] = 0;
		myData[3][1] = 0;
		myData[3][2] = 0;
		myData[3][3] = 0;
	}
	template<typename T>
	inline Matrix4x4<T>  GetFastInverse(const Matrix4x4<T> aMatrix)
	{
		Matrix4x4<T> inverseMatrix;
		//Grabs the rotation transpose
		for (size_t i = 1; i <= 3; i++)
		{
			for (size_t j = 1; j <= 3; j++)
			{
				inverseMatrix(i, j) = aMatrix(j, i);

			}
		}
		inverseMatrix(4,1) = (-aMatrix(4, 1) * inverseMatrix(1, 1)) + (-aMatrix(4, 2) * inverseMatrix(2, 1)) + (-aMatrix(4,3) * inverseMatrix(3, 1));
		inverseMatrix(4,2) = (-aMatrix(4, 1) * inverseMatrix(1, 2)) + (-aMatrix(4, 2) * inverseMatrix(2, 2)) + (-aMatrix(4, 3) * inverseMatrix(3, 2));
		inverseMatrix(4,3) = (-aMatrix(4, 1) * inverseMatrix(1, 3)) + (-aMatrix(4, 2) * inverseMatrix(2, 3)) + (-aMatrix(4, 3) * inverseMatrix(3, 3));

		return inverseMatrix;
	}
	template <typename T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow <= 4 && "Selected Row is higher than number of rows");
		assert(aRow >= 1 && "Selected Row is lower than number of rows");

		assert(aColumn <= 4 && "Selected Row is higher than number of rows");
		assert(aColumn >= 1 && "Selected Row is lower than number of rows");

		return myData[aRow - 1][aColumn - 1];
	}
	template <typename T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow <= 4 && "Selected Row is higher than number of rows");
		assert(aRow >= 1 && "Selected Row is lower than number of rows");

		assert(aColumn <= 4 && "Selected Row is higher than number of rows");
		assert(aColumn >= 1 && "Selected Row is lower than number of rows");

		return myData[aRow - 1][aColumn - 1];
	}
	//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
	template <typename T>
	Matrix4x4<T>::Matrix4x4<T>()
	{
		myData[0][0] = 1;
		myData[0][1] = 0;
		myData[0][2] = 0;
		myData[0][3] = 0;

		myData[1][0] = 0;
		myData[1][1] = 1;
		myData[1][2] = 0;
		myData[1][3] = 0;

		myData[2][0] = 0;
		myData[2][1] = 0;
		myData[2][2] = 1;
		myData[2][3] = 0;

		myData[3][0] = 0;
		myData[3][1] = 0;
		myData[3][2] = 0;
		myData[3][3] = 1;
	}
	template <typename T>
	Matrix4x4<T>::Matrix4x4<T>(const Matrix4x4<T>& aMatrix)
	{
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				myData[i - 1][j - 1] = aMatrix(i, j);
			}
		}
	}

	template <typename T>
	Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		temp(1, 1) = 1;
		temp(1, 2) = 0;
		temp(1, 3) = 0;
		temp(1, 4) = 0;

		temp(2, 1) = 0;
		temp(2, 2) = cos(aAngleInRadians);
		temp(2, 3) = sin(aAngleInRadians);
		temp(2, 4) = 0;

		temp(3, 1) = 0;
		temp(3, 2) = -sin(aAngleInRadians);
		temp(3, 3) = cos(aAngleInRadians);
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;

		return temp;
	}
	template <typename T>
	Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> temp;

		temp(1, 1) = cos(aAngleInRadians);
		temp(1, 2) = 0;
		temp(1, 3) = -sin(aAngleInRadians);
		temp(1, 4) = 0;

		temp(2, 1) = 0;
		temp(2, 2) = 1;
		temp(2, 3) = 0;
		temp(2, 4) = 0;

		temp(3, 1) = sin(aAngleInRadians);
		temp(3, 2) = 0;
		temp(3, 3) = cos(aAngleInRadians);
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;



		return temp;
	}
	template <typename T>
	Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = cos(aAngleInRadians);
		temp(1, 2) = sin(aAngleInRadians);
		temp(1, 3) = 0;
		temp(1, 4) = 0;

		temp(2, 1) = -sin(aAngleInRadians);
		temp(2, 2) = cos(aAngleInRadians);
		temp(2, 3) = 0;
		temp(2, 4) = 0;

		temp(3, 1) = 0;
		temp(3, 2) = 0;
		temp(3, 3) = 1;
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;

		return temp;
	}

	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = aMatrixToTranspose(1, 1);
		temp(1, 2) = aMatrixToTranspose(2, 1);
		temp(1, 3) = aMatrixToTranspose(3, 1);
		temp(1, 4) = aMatrixToTranspose(4, 1);

		temp(2, 1) = aMatrixToTranspose(1, 2);
		temp(2, 2) = aMatrixToTranspose(2, 2);
		temp(2, 3) = aMatrixToTranspose(3, 2);
		temp(2, 4) = aMatrixToTranspose(4, 2);

		temp(3, 1) = aMatrixToTranspose(1, 3);
		temp(3, 2) = aMatrixToTranspose(2, 3);
		temp(3, 3) = aMatrixToTranspose(3, 3);
		temp(3, 4) = aMatrixToTranspose(4, 3);

		temp(4, 1) = aMatrixToTranspose(1, 4);
		temp(4, 2) = aMatrixToTranspose(2, 4);
		temp(4, 3) = aMatrixToTranspose(3, 4);
		temp(4, 4) = aMatrixToTranspose(4, 4);
		return temp;
	}
	//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
	template<typename T>
	void Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix4x4)
	{
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				myData[i - 1][j - 1] = aMatrix4x4(i, j);
			}
		}
	}
	template <typename T>
	bool operator==(const Matrix4x4<T> aMatrix4x4, const Matrix4x4<T> bMatrix4x4)
	{
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				if (aMatrix4x4(i, j) != bMatrix4x4(i, j)) { return false; }
			}
		}
		return true;
	}

	template<typename T>
	const Matrix4x4<T> operator+(const Matrix4x4<T> aMatrix4x4, const Matrix4x4<T> bMatrix4x4)
	{
		Matrix4x4<T> temp;
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				temp(i, j) = aMatrix4x4(i, j) + bMatrix4x4(i, j);
			}
		}
		return temp;
	}
	template<typename T>
	Matrix4x4<T> operator+=(Matrix4x4<T>& aMatrix4x4, const Matrix4x4<T> bMatrix4x4)
	{
		//Matrix4x4<T> temp;
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				aMatrix4x4(i, j) += bMatrix4x4(i, j);
			}
		}
		return aMatrix4x4;
	}

	template<typename T>
	Matrix4x4<T> operator-(Matrix4x4<T>& aMatrix4x4, const Matrix4x4<T>& bMatrix4x4)
	{
		Matrix4x4<T> temp;
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				temp(i, j) = aMatrix4x4(i, j) - bMatrix4x4(i, j);
			}
		}
		return temp;
	}
	template<typename T>
	void operator-=(Matrix4x4<T>& aMatrix4x4, const Matrix4x4<T>& bMatrix4x4)
	{
		for (int i = 1; i <= 4; i++)//rows 
		{
			for (int j = 1; j <= 4; j++) // colums
			{
				aMatrix4x4(i, j) -= bMatrix4x4(i, j);
			}
		}
	}

	template<typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix4x4, const Matrix4x4<T>& bMatrix4x4)
	{
		Matrix4x4<T> temp;
		temp.MakeZero();
		for (int a = 1; a <= 4; a++)//Matris A rader
		{
			for (int b = 1; b <= 4; b++) //Matris B column
			{
				for (int c = 1; c <= 4; c++) //Matris A column
				{
					temp(a, b) += aMatrix4x4(a, c) * bMatrix4x4(c, b);
				}
			}
		}
		return temp;
	}
	template<typename T>
	void operator*=(Matrix4x4<T>& aMatrix4x4, const Matrix4x4<T>& bMatrix4x4)
	{
		Matrix4x4<T> temp;
		temp.MakeZero();
		for (int a = 1; a <= 4; a++)//Matris A rader
		{
			for (int b = 1; b <= 4; b++) //Matris B column
			{
				for (int c = 1; c <= 4; c++) //Matris A column
				{
					temp(a, b) += aMatrix4x4(a, c) * bMatrix4x4(c, b);
				}
			}
		}
		aMatrix4x4 = temp;
	}
	template<typename T>
	Vector4<T> operator*(const Matrix4x4<T>& aMatrix4x4, const Vector4<T>& aVector)
	{
		Vector4<T> temp;
		temp.x = (aVector.x * aMatrix4x4(1, 1)) + (aVector.y * aMatrix4x4(1, 2)) + (aVector.z * aMatrix4x4(1, 3)) + (aVector.w * aMatrix4x4(1, 4));
		temp.y = (aVector.x * aMatrix4x4(2, 1)) + (aVector.y * aMatrix4x4(2, 2)) + (aVector.z * aMatrix4x4(2, 3)) + (aVector.w * aMatrix4x4(2, 4));
		temp.z = (aVector.x * aMatrix4x4(3, 1)) + (aVector.y * aMatrix4x4(3, 2)) + (aVector.z * aMatrix4x4(3, 3)) + (aVector.w * aMatrix4x4(3, 4));
		temp.w = (aVector.x * aMatrix4x4(4, 1)) + (aVector.y * aMatrix4x4(4, 2)) + (aVector.z * aMatrix4x4(4, 3)) + (aVector.w * aMatrix4x4(4, 4));
		return temp;
	}
	template<typename T>
	Vector4<T> operator*=(const Matrix4x4<T>& aMatrix4x4, Vector4<T>& aVector)
	{
		Vector4<T> temp;
		temp.x = (aVector.x * aMatrix4x4(1, 1)) + (aVector.y * aMatrix4x4(1, 2)) + (aVector.z * aMatrix4x4(1, 3)) + (aVector.w * aMatrix4x4(1, 4));
		temp.y = (aVector.x * aMatrix4x4(2, 1)) + (aVector.y * aMatrix4x4(2, 2)) + (aVector.z * aMatrix4x4(2, 3)) + (aVector.w * aMatrix4x4(2, 4));
		temp.z = (aVector.x * aMatrix4x4(3, 1)) + (aVector.y * aMatrix4x4(3, 2)) + (aVector.z * aMatrix4x4(3, 3)) + (aVector.w * aMatrix4x4(3, 4));
		temp.w = (aVector.x * aMatrix4x4(4, 1)) + (aVector.y * aMatrix4x4(4, 2)) + (aVector.z * aMatrix4x4(4, 3)) + (aVector.w * aMatrix4x4(4, 4));
		aVector = temp;
		return aVector;
	}
	template<typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix4x4)
	{
		Vector4<T> temp;
		temp.x = (aVector.x * aMatrix4x4(1, 1)) + (aVector.y * aMatrix4x4(2, 1)) + (aVector.z * aMatrix4x4(3, 1)) + (aVector.w * aMatrix4x4(4, 1));
		temp.y = (aVector.x * aMatrix4x4(1, 2)) + (aVector.y * aMatrix4x4(2, 2)) + (aVector.z * aMatrix4x4(3, 2)) + (aVector.w * aMatrix4x4(4, 2));
		temp.z = (aVector.x * aMatrix4x4(1, 3)) + (aVector.y * aMatrix4x4(2, 3)) + (aVector.z * aMatrix4x4(3, 3)) + (aVector.w * aMatrix4x4(4, 3));
		temp.w = (aVector.x * aMatrix4x4(1, 4)) + (aVector.y * aMatrix4x4(2, 4)) + (aVector.z * aMatrix4x4(3, 4)) + (aVector.w * aMatrix4x4(4, 4));
		return temp;
	}
	template<typename T>
	void operator*=(Vector4<T>& aVector, const Matrix4x4<T>& aMatrix4x4)
	{
		Vector4<T> temp;
		temp.x = (aVector.x * aMatrix4x4(1, 1)) + (aVector.y * aMatrix4x4(2, 1)) + (aVector.z * aMatrix4x4(3, 1)) + (aVector.w * aMatrix4x4(4, 1));
		temp.y = (aVector.x * aMatrix4x4(1, 2)) + (aVector.y * aMatrix4x4(2, 2)) + (aVector.z * aMatrix4x4(3, 2)) + (aVector.w * aMatrix4x4(4, 2));
		temp.z = (aVector.x * aMatrix4x4(1, 3)) + (aVector.y * aMatrix4x4(2, 3)) + (aVector.z * aMatrix4x4(3, 3)) + (aVector.w * aMatrix4x4(4, 3));
		temp.w = (aVector.x * aMatrix4x4(1, 4)) + (aVector.y * aMatrix4x4(2, 4)) + (aVector.z * aMatrix4x4(3, 4)) + (aVector.w * aMatrix4x4(4, 4));
		aVector = temp;
	}
	//¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

}
namespace CU = CommonUtilities;

