#pragma once
using namespace std;
#include "Matrix.h" //includes the header for the Matrix class

template <class T> //uses the template class
class BinaryImage : public Matrix<T> //inherits from the the matrix class
{
public:
	BinaryImage(); //constructor for BinaryImage matrix with no input
	BinaryImage(int sizeR, int sizeC, T* input_data) : Matrix(sizeR, sizeC, input_data) {}; //constructor for BinaryImage matrix with size inputs and data input inherited from the Matrix class constructor
	BinaryImage(int sizeR, int sizeC, T* input_data, T threshold); //constructor for BinaryImage matrix with size inputs, a T* pointer to the input_data and a T type threshold for to create the binaryImage
	BinaryImage(const BinaryImage & x); //copy constructor for BinaryImage
	~BinaryImage(); //BinaryImage destructor

	BinaryImage getBlock(int start_row, int end_row, int start_column, int end_column) const; //getBlock to return a BinaryImage block

	bool operator+(const BinaryImage& referncedMatrix); //boolean overload for + operator to perform a OR operation on two matrices
	bool operator-(const BinaryImage& referncedMatrix); //boolean overload for - operator to perform a XOR operation on two matrices
	bool operator*(const BinaryImage& referncedMatrix); //boolean overload for * operator to perform a AND operation on two matrices
	bool operator/(const BinaryImage& referncedMatrix); //boolean overload for / operator to perform a NOR operation on two matrices
};