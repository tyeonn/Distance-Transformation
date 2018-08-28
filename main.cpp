#include <iostream>
#include <fstream>

class ImageProcessing
{
public:
    ImageProcessing(int, int, int, int);
    void loadImage(std::ifstream& is);
    void loadNeighbor(int, int);
    void loadNeighbor2(int, int);
    void firstPassDistance();
    void secondPassDistance();
    void prettyPrint(std::ofstream&, int);
    void print(std::ofstream&);
    int min();
    int min2();
    void zeroFramed();

private:
    int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
    int** zeroFramedAry;
    int neighborAry[5];
};

ImageProcessing::ImageProcessing(int row, int col, int min, int max)
{
    numRows = row;
    numCols = col;
    minVal = min;
    maxVal = max;
    newMinVal = newMaxVal = (minVal + maxVal) / 2;
    zeroFramedAry = new int* [numRows + 2];
    for (int i = 0; i < numRows + 2; i++)
    {
        zeroFramedAry[i] = new int [numCols + 2];
    }

}
void ImageProcessing::zeroFramed()
{
    for (int i = 0; i < numRows + 2; i++)
    {
        for (int j = 0; j < numCols + 2; j++)
        {
            zeroFramedAry[i][j] = 0;
        }
    }
}
void ImageProcessing::loadImage(std::ifstream& is)
{
    for (int i = 1; i < numRows + 1; i++)
    {
        for (int j = 1; j < numCols + 1; j++)
        {
            is >> zeroFramedAry[i][j];
        }
    }

}

void ImageProcessing::loadNeighbor(int row, int col)
{
    neighborAry[0] = zeroFramedAry[row - 1][col - 1];
    neighborAry[1] = zeroFramedAry[row - 1][col];
    neighborAry[2] = zeroFramedAry[row - 1][col + 1];
    neighborAry[3] = zeroFramedAry[row][col - 1];
    neighborAry[4] = zeroFramedAry[row][col];
}

void ImageProcessing::loadNeighbor2(int row, int col)
{
    neighborAry[0] = zeroFramedAry[row][col + 1];
    neighborAry[1] = zeroFramedAry[row + 1][col - 1];
    neighborAry[2] = zeroFramedAry[row + 1][col];
    neighborAry[3] = zeroFramedAry[row + 1][col + 1];
    neighborAry[4] = zeroFramedAry[row][col];
}

int ImageProcessing::min()
{
    int min = 0;
    for (int i = 0; i < 4; i++)
    {
        if (neighborAry[i] < neighborAry[min])
        {
            min = i;
        }
    }
    return neighborAry[min];
}

int ImageProcessing::min2()
{
    int min = 0;
    for (int i = 0; i < 4; i++)
    {
        if (neighborAry[i] < neighborAry[min])
        {
            min = i;
        }
    }
    if ((neighborAry[min] + 1) < neighborAry[4])
        return neighborAry[min] + 1;
    else
        return neighborAry[4];

}

void ImageProcessing::firstPassDistance()
{
    for (int i = 1; i < numRows + 1; i++)
    {
        for (int j = 1; j < numCols + 1; j++)
        {
            if (zeroFramedAry[i][j] > 0)
            {
                loadNeighbor(i, j);
                zeroFramedAry[i][j] = min() + 1;
            }

        }
    }
}

void ImageProcessing::secondPassDistance()
{
    for (int i = numRows; i > 0; i--)
    {
        for (int j = numCols; j > 0; j--)
        {
            if (zeroFramedAry[i][j] > 0)
            {
                loadNeighbor2(i, j);
                zeroFramedAry[i][j] = min2();

                if (zeroFramedAry[i][j] < newMinVal)
                {
                    newMinVal = zeroFramedAry[i][j];
                }
                if (zeroFramedAry[i][j] > newMaxVal)
                {
                    newMaxVal = zeroFramedAry[i][j];
                }
            }
        }
    }

}

void ImageProcessing::prettyPrint(std::ofstream& os, int pass)
{
    os << "Result of Pass " << pass << ": " << std::endl;
    for (int i = 1; i < numRows + 1; i++)
    {
        for (int j = 1; j < numCols + 1; j++)
        {
            if (zeroFramedAry[i][j] == 0)
                os << "  ";
            else if (zeroFramedAry[i][j] < 10)
                os << " " << zeroFramedAry[i][j];
            else
                os << zeroFramedAry[i][j];
        }
        os << std::endl;
    }
}

void ImageProcessing::print(std::ofstream& os)
{
    os << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << std::endl;
    for (int i = 1; i < numRows + 1; i++)
    {
        for (int j = 1; j < numCols + 1; j++)
        {
            os << zeroFramedAry[i][j] << " ";
        }
        os << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::ifstream inFile;
    std::ofstream outFile1;
    std::ofstream outFile2;
    int numRows, numCols, minVal, maxVal;
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3], std::ios::app);
    inFile >> numRows >> numCols >> minVal >> maxVal;

    ImageProcessing ip(numRows, numCols, minVal, maxVal);
    ip.zeroFramed();
    ip.loadImage(inFile);
    ip.firstPassDistance();
    ip.prettyPrint(outFile2, 1);
    ip.secondPassDistance();
    ip.prettyPrint(outFile2, 2);
    ip.print(outFile1);

    inFile.close();
    outFile1.close();
    outFile2.close();

    return 0;
}
