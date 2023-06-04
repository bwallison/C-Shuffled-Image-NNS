#include "BinaryImage.h"

template <class T>
BinaryImage<T>::BinaryImage() //constructor for BinaryImage matrix with no input
{};

template <class T> BinaryImage<T>::BinaryImage(int sizeR, int sizeC, T* input_data, T threshold) : Matrix(sizeR, sizeC, input_data) //constructor for BinaryImage matrix with size inputs and data input inherited from the Matrix class constructor
{
	for (int i = 0; i < _M*_N; i++) //loops through the entire BinaryImage matrix
		if (_data[i] < threshold) //if the data value is below the threshold
			_data[i] = 0; //sets any data value below threshold to 0
		else
			_data[i] = 1; //sets any data above the threshold to 1
};
template <class T> BinaryImage<T>::BinaryImage(const BinaryImage& x) : Matrix(x) {}; //constructor for BinaryImage matrix with size inputs, a T* pointer to the input_data and a T type threshold for to create the binaryImage
template <class T> BinaryImage<T>::~BinaryImage() //BinaryImage destructor
{
	delete[] _data; //deletes any data in the matrix
	_data = 0;
};

template <class T> BinaryImage<T> BinaryImage<T>::getBlock(int start_row, int end_row, int start_column, int end_column) const //getBlock to return a BinaryImage block
{
	int x = end_row - start_row + 1; //x is the length of the rows
	int y = end_column - start_column + 1; //y is the length of the columns
	T* block = new T[x*y]; //creates a block of the size passed
	for (int i = 0; i < x; i++) //assignes all the block elements with the corresponding element of the block being retrieved
		for (int j = 0; j < y; j++)
			block[i*y + j] = get((i + start_row), (j + start_column));

	BinaryImage returnedBlock(x, y, block); //creates a BinaryImage from that block
	delete[] block; //deletes the block data
	block = 0;
	return returnedBlock; //returns the BinaryImage object
}

template <class T> bool BinaryImage<T>::operator+(const BinaryImage& referncedMatrix) //boolean overload for + operator to perform a OR operation on two matrices
{
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
			if (get(i, j) == referncedMatrix.get(i, j))
				return true;
	return false;
}

template <class T> bool BinaryImage<T>::operator-(const BinaryImage& referncedMatrix) //boolean overload for - operator to perform a XOR operation on two matrices
{
	bool one = false, two = false;
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
		{
			if (get(i, j) == referncedMatrix.get(i, j))
				one = true;
			else
				two = true;
		}
	if (one&&two)
		return true;
	else
		return false;
}

template <class T> bool BinaryImage<T>::operator*(const BinaryImage& referncedMatrix) //boolean overload for * operator to perform a AND operation on two matrices
{
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
			if (get(i, j) != referncedMatrix.get(i, j))
				return false;
	return true;
}

template <class T> bool BinaryImage<T>::operator/(const BinaryImage& referncedMatrix) //boolean overload for / operator to perform a NOR operation on two matrices
{
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
			if (get(i, j) == referncedMatrix.get(i, j))
				return false;
	return true;
}