#pragma once
#include "math.h" //includes math.h to be used in NNCScore to calculate mean and perform pow function
using namespace std;

template <class T> //uses template of class T
class Matrix //matrix class
{
protected: //data of the matrix class is stored as protected as it needs to be accessed by other classes
	int _M, _N; //_M stores the rows and _N stores the columns
	T* _data; //stores value for the elements of type T
public:
	Matrix(); //constructor for a matrix with no input
	Matrix(int sizeR, int sizeC, T input_val); //constructor for a matrix with size inputs and data val which is set to every element in the matrix
	Matrix(int sizeR, int sizeC, T* input_data); //constructor for a matrix with size inputs, a T* pointer to the input_data
	Matrix(const Matrix & copy); //matrix copy constructor
	~Matrix(); //matrix destructor

	int Matrix::get_M() const { return _M; }; //returns the value of rows of a matrix
	int Matrix::get_N() const { return _N; }; //returns the value of columns of a matrix
	double* Matrix::get_Data() { return _data; }; //returns the _data of the matrix
	double Matrix::get(int i, int j) const { return _data[(i * _N) + j]; }; //returns a element given by position of any given row and column location (i = row, j = column)
	void Matrix::set(int i, int j, T val) { _data[(i * _N) + j] = val; }; // sets a element given by position of any given row and column location (i = row, j = column) to the value of "val"
	Matrix getBlock(int start_row, int end_row, int start_column, int end_column) const; //gets a block of and returns it as a matrix. The dimentions of the matrix are specified for the block to be taken from
	void setBlock(int start_row, int start_column, Matrix referncedMatrix); //sets a block in a given position of row and column to the passed matrix

	Matrix operator+(const Matrix & referncedMatrix); //operator overload to add each element of two matrices together
	Matrix operator-(const Matrix & referncedMatrix); //operator overload to minus each element of one matrix from another
	Matrix operator*(const Matrix & referncedMatrix); //operator overload to times each element of two matrices together
	Matrix operator/(const Matrix & referncedMatrix); //operator overload to divide each element of one matrix by another
	Matrix operator=(const Matrix & referncedMatrix); //operator overload to set each element of matrix equal to another
	void operator++(int); //operator overload to increment every element by one of a matrix
	void operator--(); //operator overload to decrement every element by one of a matrix
	double operator()(int i, int j); //operator overload for () will return the value at that position

	double mean(); //finds the mean of a matrix
	double NNCScore(Matrix inputMatrix); //performs the calculation for the normalised correlation value between two matrices
	double SSDScore(Matrix inputMatrix); //performs the calculation for the sum of the squared differences between two matrices
};