#include <sstream> // stringstream
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <istream> //used by readTXT
#include "Matrix.h" //Matrix header file
#include "BinaryImage.h" //BinaryImage header file
#include "Matrix.cpp" //Matrix cpp file included because a template is implemented
#include "BinaryImage.cpp" //BinaryImage cpp file included because a template is implemented

double* readTXT(char *fileName, int sizeR, int sizeC); //declare readTXT before main function
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q); //declare WritePGM before main function
void normalisedCorrelation(double* shuffledLogo, double* noisyLogo); //declare normalisedCorrelation before main function
void sumSquaredDifference(double* shuffledLogo, double* noisyLogo); //declare sumSquaredDifference before main function

void main()
{
	std::cout << "Reading the data from the given text file..." << std::endl << std::endl; //text letting the user know the text file is being read
	// Read .txt file with image of size RxC, and convert to an array of doubles
	double *shuffledLogo = readTXT("..\\Files\\shuffled_logo.txt", 512, 512); //reads in the shuffled logo if placed in the files folder of the project
	double *noisyLogo = readTXT("..\\Files\\unshuffled_logo_noisy.txt", 512, 512); //reads in the noisy logo if placed in the files folder of the project
	int userInput = 0; //sets a value to userInput used for the user selection console interface
	bool flag = true; //flag for while loop to use
	while (flag)
	{
		std::cout << "1: Clean and Unshuffled using normilised correlation nearest neighbor search" << std::endl << "2: Clean and Unshuffled using sum of the squared difference nearest neighbor search" << std::endl << "3: To Clean and unshuffled using both methods"<< std::endl << "0: to exit the program" << std::endl << std::endl;
		std::cout << "What method do you want to run?" << std::endl;
		std::cin >> userInput;
		system("cls"); //clears the console after the user enters a input
		switch (userInput) //switch for the user input
		{
		case 0:
			flag = false; //flag set to false the program leaves the while loop
			break;
		case 1: //performs the normalised correlatio nearest neighbor search
			normalisedCorrelation(shuffledLogo, noisyLogo); //calls the normalised correlation method and passes the data from the two read in logo's
			break;
		case 2: //performs the sum of the squared difference nearest neighbor search
			sumSquaredDifference(shuffledLogo, noisyLogo); //calls the sum of the squared differences method and passes the data from the two read in logo's
			break;
		case 3: //performs both searches
			normalisedCorrelation(shuffledLogo, noisyLogo); //calls the normalised correlation method and passes the data from the two read in logo's
			sumSquaredDifference(shuffledLogo, noisyLogo);  //calls the sum of the squared differences method and passes the data from the two read in logo's
			break;
		default:
			std::cout << "It seems your input was not valid, Please try again." << std::endl << std::endl;
			break;
		}
	}
}

// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(char *fileName, int sizeR, int sizeC)
{
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName); //opens a ifstream for reading in the data
	if (myfile.is_open())
	{
		while (myfile.good())
		{
			if (i > (sizeR*sizeC - 1)) break;
			myfile >> *(data + i);
			i++;
		}
		myfile.close();
	}
	else std::cout << "Unable to open file: " << fileName << ".Please make sure it is in the correct directory" << std::endl; //if the files is not found it tells the user
	return data; //returns the data read in
}

void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q) //Q is set to 1 for BinaryImages and 255 for greyscale, this method writes the .PGM file in output files as that is the filename location passed
{
	unsigned char *image;
	ofstream myfile;
	image = (unsigned char *) new unsigned char[sizeR * sizeC];
	for (int i = 0; i < (sizeR * sizeC); i++)
		image[i] = (unsigned char)data[i];
	myfile.open(filename, ios::out | ios::binary | ios::trunc);
	if (!myfile)
	{
		std::cout << "Can't open file:  " << filename << std::endl;
		exit(1);
	}
	myfile << "P5" << std::endl << sizeC << " " << sizeR << std::endl << Q << std::endl;
	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));
	if (myfile.fail())
	{
		std::cout << "Can't write image " << filename << std::endl;
		exit(0);
	}
	myfile.close();
	delete[] image;
}

void normalisedCorrelation(double* shuffledLogo, double* noisyLogo) //normalised correlation nearest neighbor search method using BinaryImages
{
	Matrix<double> shuffledBlock; //creates a empty matrix for holding the current block of the shuffled image
	Matrix<double> noisyBlock; //creates a empty matrix for holding the current block of the noisy image
	Matrix<double> greyScaleShuffled(512, 512, shuffledLogo); //creates a matrix from the shuffled image data passed of 512 x 512 pixels and is used for setting to the correct location in the printMatrix
	Matrix<double> printMatrix(512, 512, 1); //creates printMatrix, a matrix with all values of 1 used to be assigned the blocks from the greyScaleShuffled matrix
	BinaryImage<double> shuffled(512, 512, shuffledLogo, 85); //creates a BinaryImage from the shuffled image data passed of 512 x 512 pixels 
	BinaryImage<double> noisy(512, 512, noisyLogo, 85); //creates a BinaryImage from the noisy image data passed of 512 x 512 pixels
	double NC = -1, currentNC = 0; //initialises variables for NC and currentNC used in the nearest neighbor search
	int bestX = 0, bestY = 0; //integers for holding the best start row and column for the 32x32 block in the search for the best match

	for (int i = 0; i < 512; i += 32) //loops through all of the shuffled image blocks by 32 as the image is shuffled in 32x32 blocks
		for (int j = 0; j < 512; j += 32)
		{
			shuffledBlock = shuffled.getBlock(i, i + 31, j, j + 31); //gets a block of the current shuffled elements
			for (int x = 0; x < 512; x += 32) //loops through all of the noisy image blocks by 32 as the image is shuffled in 32x32 blocks
				for (int y = 0; y < 512; y += 32)
				{
					noisyBlock = noisy.getBlock(x, x + 31, y, y + 31); //gets a block of the current noisy elements
					currentNC = shuffledBlock.NNCScore(noisyBlock); //calls the SSDScore method to calculate the normalised correlation value between the two matrices
					if (currentNC > NC) //if the value of this block is bigger than NC it is a better match
					{
						bestX = x; //the row start position is taken
						bestY = y; //the column start position is taken
						NC = currentNC; //NC is now set to currentNC as this is the current best matching normalised correlation value 
						if (NC == 1) //if it is equal to 1 it is a perfect match and leaves the loop
							break;
					}
					if (NC == 1) //if it is equal to 1 it is a perfect match and leaves the loop
						break;
				}
			shuffledBlock = greyScaleShuffled.getBlock(i, i + 31, j, j + 31); //after a shuffledBlock is compared with all noisyBlocks it sets shuffledBlock to the block is was comparing with the rest
			printMatrix.setBlock(bestX, bestY, shuffledBlock); //sets the greyscale shuffledBlock to the best match located by bestX and bestY to printMatrix
			NC = -1, bestX = 0, bestY = 0; //results NC, bestX and bestY to thier default values for the next loop cycle
		}
	WritePGM("..\\Output Files\\NormalisedCorrelationCleaned.pgm", printMatrix.get_Data(), 512, 512, 255); //writes the .PGM file from the printMatrix object data
	std::cout << "File written in ..\\Output Files\\NormalisedCorrelationCleaned.pgm" << std::endl << std::endl;
	//WritePGM("..\\Output Files\\RefA.pgm", shuffled.get_Data(), 512, 512, 1); //test method used for showing the shuffled image 
	//WritePGM("..\\Output Files\\RefB.pgm", noisy.get_Data(), 512, 512, 1); //test method used for showing the noisy image
}

void sumSquaredDifference(double* shuffledLogo, double* noisyLogo) //sum of the squared differences nearest neighbor search method using BinaryImages
{
	Matrix<double> shuffledBlock; //creates a empty matrix for holding the current block of the shuffled image
	Matrix<double> noisyBlock; //creates a empty matrix for holding the current block of the noisy image
	Matrix<double> greyScaleShuffled(512, 512, shuffledLogo); //creates a matrix from the shuffled image data passed of 512 x 512 pixels and is used for setting to the correct location in the printMatrix
	Matrix<double> printMatrix(512, 512, 1); //creates printMatrix, a matrix with all values of 1 used to be assigned the blocks from the greyScaleShuffled matrix
	BinaryImage<double> shuffled(512, 512, shuffledLogo, 85);  //creates a BinaryImage from the shuffled image data passed of 512 x 512 pixels
	BinaryImage<double> noisy(512, 512, noisyLogo, 85); //creates a BinaryImage from the noisy image data passed of 512 x 512 pixels
	double SSD = 10000000000000000, currentSSD = 0;
	int bestX = 0, bestY = 0; //integers for holding the best start row and column for the 32x32 block in the search for the best match

	for (int i = 0; i < 512; i += 32) //loops through all of the shuffled image blocks by 32 as the image is shuffled in 32x32 blocks
		for (int j = 0; j < 512; j += 32)
		{
			shuffledBlock = shuffled.getBlock(i, i + 31, j, j + 31); //gets a block of the current shuffled elements
			for (int x = 0; x < 512; x += 32) //loops through all of the noisy image blocks by 32 as the image is shuffled in 32x32 blocks
			{
				for (int y = 0; y < 512; y += 32)
				{
					noisyBlock = noisy.getBlock(x, x + 31, y, y + 31); //gets the block of the current noisyBlock
					currentSSD = shuffledBlock.SSDScore(noisyBlock); //calls the SSDScore method to calculate the sum of the squared differences between the two matrices
					if (currentSSD < SSD) //the closer to 0 the better the match is so if the compared block is lower than any of the previously compared block the following is executed
					{
						SSD = currentSSD; //the SSD is set to currentSSD as this is now the lowest value for this comparison of the shuffledBlock to all of the noisyBlocks
						bestX = x; //the row start position is taken
						bestY = y; //the column start position is taken
					}
				}
			}
			shuffledBlock = greyScaleShuffled.getBlock(i, i + 31, j, j + 31); //after a shuffledBlock is compared with all noisyBlocks it sets shuffledBlock to the block is was comparing with the rest
			printMatrix.setBlock(bestX, bestY, shuffledBlock); //sets the greyscale shuffledBlock to the best match located by bestX and bestY to printMatrix
			SSD = 10000000000000000, bestX = 0, bestY = 0; //results SSD, bestX and bestY to thier default values for the next loop cycle
		}
	WritePGM("..\\Output Files\\SumOfTheSquaredDifferenceCleaned.pgm", printMatrix.get_Data(), 512, 512, 255); //writes the .PGM file from the printMatrix object data
	std::cout << "File written in ..\\Output Files\\SumOfTheSquaredDifferenceCleaned.pgm" << std::endl << std::endl;
	//WritePGM("..\\Output Files\\RefA.pgm", shuffled.get_Data(), 512, 512, 1); //test method used for showing the shuffled image 
	//WritePGM("..\\Output Files\\RefB.pgm", noisy.get_Data(), 512, 512, 1);  //test method used for showing the noisy image
}