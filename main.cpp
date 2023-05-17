// Program made to call "filter_image.hpp"

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter_image.hpp"

using namespace cv;
using namespace std;

// Number of images to display, 0-2         // Default Value:
#define BLOSSOM_DEBUG_NUM_DIPSLAY_IMAGES 2	// 2

// Function declarations
void print_num_blossoms(int numBlossoms);
void blob_fill_test(string path);


int main(void)
{
    // Path variables need to be changed to be actual paths
    string originalImagePath  = "path to original image";
    string processedImagePath = "path to where the processed image should go";
    Mat image;

    // Make sure the original image exists
    image = imread(originalImagePath);
    if ( !image.data )
    {
        // If the image does not exist, close program
        printf("No original image data\n");
        return -1;
    }

    // Dispaly original image only if num display is greater than one
    if (BLOSSOM_DEBUG_NUM_DIPSLAY_IMAGES > 1)
    {
        resize(image, image, Size(), 0.5, 0.5);
        namedWindow("Original Image");
        imshow("Original Image", image);
    }



    // Process image and print number of blossoms
    int numBlossoms = filter_image_pre(processedImagePath, originalImagePath);
    print_num_blossoms(numBlossoms);



    // Dispaly processed image only if num display is greater not 0
    if (BLOSSOM_DEBUG_NUM_DIPSLAY_IMAGES)
    {
        // Make sure the processed image exists before printing it
        image = imread(processedImagePath);
        if ( !image.data )
        {
            printf("No processed image data\n");
            return -2;
        }
        resize(image, image, Size(), 0.5, 0.5);
        namedWindow("Filtered Image");
        imshow("Filtered Image", image);

        // Don't immediately exit program so the user can see the images
        waitKey(0);
    }

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
