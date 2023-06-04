#include "Matrix.h"
#include <sstream> // stringstream
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <istream>

template<class T>
Matrix<T>::Matrix()  //constructor for a matrix with no input
{
};

template <class T> Matrix<T>::Matrix(int sizeR, int sizeC, T input_val) //constructor for a matrix with size inputs and data val which is set to every element in the matrix
{
	_M = sizeR; //assigns _M with the number of rows
	_N = sizeC; //assigns _N with the number of columns
	_data = new T[_M*_N]; //gives _data storage of type T with _M*_N number of elements
	for (int i = 0; i < _M*_N; i++) //loop through _data elements and assign them with the passed value
		_data[i] = input_val;
	//cout << "Matrix::Matrix(int sizeR, int sizeC, double input_val) is invoked." << endl;
};

template <class T> Matrix<T>::Matrix(int sizeR, int sizeC, T* input_data) //constructor for a matrix with size inputs, a T* pointer to the input_data
{
	_M = sizeR; //assigns _M with the number of rows
	_N = sizeC; //assigns _N with the number of columns
	_data = new T[_M*_N]; //gives _data storage of type T with _M*_N number of elements
	for (int i = 0; i < _M*_N; i++) //loops through the _data elements and assign them to the inputted data elements
		_data[i] = input_data[i];
	//cout << "Matrix::Matrix(int sizeR, int sizeC, double* input_data) is invoked." << endl;
};

template <class T> Matrix<T>::Matrix(const Matrix& copy) //matrix copy constructor
{
	_M = copy.get_M();
	_N = copy.get_N();
	_data = new T[_M*_N];
	for (int i = 0; i < _M*_N; i++)
		_data[i] = copy._data[i];
	//cout << "Matrix::Matrix(const Matrix&) is invoked.... " << endl;
}

template <class T> Matrix<T>::~Matrix() //matrix destructor
{
	/*std::cout << "Matrix:~Matrix() is invoked." << std::endl;*/
	delete[] _data; //deletes matrix data
	_data = 0;
};

template <class T> Matrix<T> Matrix<T>::getBlock(int start_row, int end_row, int start_column, int end_column) const //gets a block of and returns it as a matrix. The dimentions of the matrix are specified for the block to be taken from
{
	int x = end_row - start_row + 1;  //x is the length of the rows
	int y = end_column - start_column + 1; //y is the length of the columns
	T* block = new T[x*y]; //creates a block of the size passed
	for (int i = 0; i < x; i++) //assignes all the block elements with the corresponding element of the block being retrieved
		for (int j = 0; j < y; j++)
			block[i*y + j] = get((i + start_row), (j + start_column));

	Matrix<T> returnedBlock(x, y, block); //creates a matrix of type T from that block
	delete[] block; //deletes the block data
	block = 0;
	return returnedBlock; //returns the matrix created
}

template <class T> void Matrix<T>::setBlock(int start_row, int start_column, Matrix referncedMatrix) //sets a block in a given position of row and column to the passed matrix
{
	for (int i = 0; i < referncedMatrix._M; i++) //loops through the entire passed matrix
		for (int j = 0; j < referncedMatrix._N; j++)
			_data[(start_row + i) * _N + (start_column + j)] = referncedMatrix.get(i, j); //sets the new block with the refrencedMatrix passed to the appropraite location in the matrix object calling the function
}

template <class T> Matrix<T> virtual Matrix<T>::operator+(const Matrix& referncedMatrix) //operator overload to add each element of two matrices together
{
	//std::cout << "Matrix::operator+ is invoked...." << std::endl;
	Matrix<T> temp; //creates a temporary matrix
	temp._M = referncedMatrix._M;
	temp._N = referncedMatrix._N;
	temp._data = new T[temp._M*temp._N];
	for (int x = 0; x < (temp._M*temp._N); x++)
		temp._data[x] = this->_data[x] + referncedMatrix._data[x];
	return temp; //returns this matrix
}

template <class T> Matrix<T> virtual Matrix<T>::operator-(const Matrix& referncedMatrix) //operator overload to minus each element of one matrix from another
{
	//std::cout << "Matrix::operator- is invoked...." << std::endl;
	Matrix<T> temp;
	temp._M = referncedMatrix._M;
	temp._N = referncedMatrix._N;
	temp._data = new T[temp._M*temp._N];
	for (int x = 0; x < (temp._M*temp._N); x++)
		temp._data[x] = this->_data[x] - referncedMatrix._data[x];
	return temp;
}

template <class T> Matrix<T> virtual Matrix<T>::operator*(const Matrix& referncedMatrix) //operator overload to times each element of two matrices together
{
	//std::cout << "Matrix::operator* is invoked...." << std::endl;
	Matrix<T> temp;
	temp._M = referncedMatrix._M;
	temp._N = referncedMatrix._N;
	temp._data = new T[temp._M*temp._N];
	for (int x = 0; x < (temp._M*temp._N); x++)
		temp._data[x] = this->_data[x] * referncedMatrix._data[x];
	return temp;
}

template <class T> Matrix<T> virtual Matrix<T>::operator/(const Matrix& referncedMatrix) //operator overload to divide each element of one matrix by another
{
	//std::cout << "Matrix::operator/ is invoked...." << std::endl;
	Matrix<T> temp;
	temp._M = referncedMatrix._M;
	temp._N = referncedMatrix._N;
	temp._data = new T[temp._M*temp._N];
	for (int x = 0; x < (temp._M*temp._N); x++)
		temp._data[x] = this->_data[x] / referncedMatrix._data[x];
	return temp;
}

template <class T> Matrix<T> Matrix<T>::operator=(const Matrix& referncedMatrix) //operator overload to set each element of matrix equal to another
{
	//std::cout << "Matrix::operator= is invoked...." << std::endl;
	//delete existing _data information - as we are going to replace it with 'other._data'
	delete[] _data;
	_M = referncedMatrix._M;
	_N = referncedMatrix._N;
	//reserve memory for new array
	_data = new T[_M*_N];
	//'this' pointer refers to the current object
	for (int x = 0; x < (_M*_N); x++)
		this->_data[x] = referncedMatrix._data[x];
	return *this;
}

template <class T> void Matrix<T>::operator++(int) //operator overload to increment every element by one of a matrix
{
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
			this->set(i, j, get(i, j) + 1); //sets all element to the element + 1
}

template <class T> void Matrix<T>::operator--() //operator overload to decrement every element by one of a matrix
{
	for (int i = 0; i < _M; i++)
		for (int j = 0; j < _N; j++)
			this->set(i, j, get(i, j) - 1); //sets all element to the element - 1
}

template <class T> double Matrix<T>::operator()(int i, int j) //operator overload for () will return the value at that position
{
	//std::cout << "Matrix::operator() is invoked...." << std::endl;
	return _data[i*_N + j]; //returns the data value
}

template <class T> double Matrix<T>::mean() //finds the mean of a matrix
{
	double total = 0; //creates a variable to hold the total of all the data elements
	for (int i = 0; i < (_M*_N); i++) //loops through all the matrix elements
		total += _data[i]; //adds each element to the value of total
	return total / (_M*_N); //calculates the mean and returns the value
}

template <class T> double Matrix<T>::NNCScore(Matrix inputMatrix) //performs the calculation for the normalised correlation value between two matrices
{
	double meanA = mean(), meanB = inputMatrix.mean();
	double partA = 0, partB = 0, partC = 0; //initialises variables to hold values calculating during the for loop
	for (int i = 0; i < _M*_N; i++) //loops through every element in the matrix calculating the Normalised Correlation value for each element and totalling them in three parts of the algorithm
	{
		double A = _data[i] - meanA;
		double B = inputMatrix._data[i] - meanB;
		partA += A * B;
		partB += pow(A, 2);
		partC += pow(B, 2);
	}
	return partA / sqrt(partB*partC); //performs the final part of the algorithm and returns the normalised correlation value for the whole matrix
}

template <class T> double Matrix<T>::SSDScore(Matrix inputMatrix) //performs the calculation for the sum of the squared differences between two matrices
{
	double SSD = 0; //creates a variable to store the value of the sum of the squared differences
	for (int i = 0; i < _M*_N; i++) //loops through every element
		SSD += (inputMatrix._data[i] - _data[i]) * (inputMatrix._data[i] - _data[i]); //calculates sum of the squared differences of each element and adds it to the variable SSD 
	return SSD;
}
