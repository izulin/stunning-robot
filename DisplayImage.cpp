#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;

int dist(Vec3b& X, Vec3b& Y)
{
    return (X[0]-Y[0])*(X[0]-Y[0]) + (X[1]-Y[1])*(X[1]-Y[1]) + (X[2]-Y[2])*(X[2]-Y[2]);
}

int main(int argc, char** argv )
{

    if(argc < 4)
    {
        printf("Usage: DisplayImage output.png gray.png file1 file2 ... filen\n");
        return -1;
    }
    int numOfImages = argc-3;
    char* name_output;
    char* name_gray;
    name_output = argv[1];
    name_gray = argv[2];

    int nRows, nCols;
    std::vector<Mat> input(numOfImages);
    for(int i=0;i<numOfImages;i++)
    {
        input[i] = imread( argv[i+3], IMREAD_COLOR);
        if( !input[i].data )
        {
            printf("%s : No image data \n", argv[i+1]);
            return -1;
        }
        if(i==0)
        {
            nRows = input[i].rows;
            nCols = input[i].cols;
        }
        else
        {
            assert(nRows == input[i].rows);
            assert(nCols == input[i].cols);
        }
    }

    Mat out, gray;

    out.create(nRows, nCols, CV_8UC3);
    gray.create(nRows, nCols, CV_8UC1);

    for(int x=0;x<nRows;x++)
        for(int y=0;y<nCols;y++)
        {
            int bestscore = -1000000000; //-infty with this distance function
            int bestind = -1; //image that minimizes the score
            for(int iter_i = 0; iter_i < numOfImages; iter_i++)
            {
                int score = 0;
                for(int iter_j = 0; iter_j < numOfImages; iter_j++)
                {
                    Vec3b& point_i = input[iter_i].at<Vec3b>(x,y);
                    Vec3b& point_j = input[iter_j].at<Vec3b>(x,y);
                    score += dist(point_i, point_j);
                }
                if(score > bestscore)
                {
                    bestscore = score;
                    bestind = iter_i;
                }
            }
            assert(bestind != -1);
            out.at<Vec3b>(x,y) = input[bestind].at<Vec3b>(x,y);
            gray.at<uchar>(x,y) = ((numOfImages-bestind-1)*256)/numOfImages;
        }

    imwrite( name_output, out);
    imwrite( name_gray, gray);

    return 0;
}
