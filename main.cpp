#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter_image.hpp"
// #include "filter_image_v0.hpp"// Original filter

using namespace cv;
using namespace std;

#define DIPSLAY_IMAGES 1

void print_num_blossoms(int numBlossoms);
void blob_fill_test(string path);


int main(int argc, char** argv )
{
    string originalImagePath  = "C:/Users/10jos/OneDrive - Northwest Nazarene University/Research/BlossomCam/Original_Images/eg7.jpg";
    string processedImagePath = "C:/Users/10jos/OneDrive - Northwest Nazarene University/Research/BlossomCam/Processed_Images/prc0.jpg";
    Mat image;

    if (DIPSLAY_IMAGES > 1)
    {
        // Make sure the original image exists
        image = imread(originalImagePath);
        if ( !image.data )
        {
            printf("No image data\n");
            return -1;
        }
        resize(image, image, Size(), 0.5, 0.5);
        namedWindow("Display Image");
        imshow("Display Image", image);
        waitKey(0);
    }

    // ::::::::::::::::::::::
    // Process image       ::
    // ::::::::::::::::::::::
    int numBlossoms = filter_image_pre(processedImagePath, originalImagePath);
    print_num_blossoms(numBlossoms);


    if (DIPSLAY_IMAGES)
    {
        image = imread(processedImagePath);
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        // resize(image, image, Size(), 0.5, 0.5);
        namedWindow("Display Image");
        imshow("Display Image", image);
    }

    // Don't immediately exit program
    waitKey(0);

    // blob_fill_test(processedImagePath);//temp

    return 0;
}


// Print number of blossoms
void print_num_blossoms(int numBlossoms)
{
    printf("\n\033[0;31m:: :: :: :: :: :: :: :: :: :: :: ::\n");
    printf("::\t\t\t\t ::\n");

    if (numBlossoms < 100)
        printf("::\t \033[0mBlossom Count: \033[0;32m%i\t \033[0;31m::\n", numBlossoms);
    else
        printf("::\t\033[0mBlossom Count: \033[0;32m%i\t \033[0;31m::\n", numBlossoms);

    printf("::\t\t\t\t ::\n");
    printf(":: :: :: :: :: :: :: :: :: :: :: ::\033[0m\n\n");
}
