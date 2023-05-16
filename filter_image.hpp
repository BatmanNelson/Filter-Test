#pragma once

/*
    Here are the image filters
*/

// Debug variables
#define BLOSSOM_DEBUG_SHOW_BOXES true
#define BLOSSOM_DEBUG_SHOW_IMAGES true


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat bgr_to_bin(cv::Mat bgrImage)
{
    // Converting image to gray scale.
    cv::Mat grayImage;
    cv::cvtColor(bgrImage, grayImage, cv::COLOR_BGR2GRAY);

    // Converting image to binary
    cv::Mat ThresholdImage;
    cv::threshold(grayImage, ThresholdImage, 0, 255, cv::THRESH_BINARY);
    // ThresholdImage = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC3);
    // ThresholdImage = ThresholdImage > 1;

    // Invert image
    cv::Mat BinaryImage;
    cv::bitwise_not(ThresholdImage, BinaryImage);

    // Invert image again (back to original b/w)
    cv::Mat UnBinary;
    cv::bitwise_not(BinaryImage, UnBinary);

    return UnBinary;
}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat binary_filter(cv::Mat inputImage, std::string blobImagePath)
// cv::Mat filterImage0(cv::Mat inputImage)//, int* numBlossoms)
{
    cv::Mat filterImage = inputImage.clone();
    cv::Mat hsvImage;
    cv::cvtColor(filterImage, hsvImage, cv::COLOR_BGR2HSV);

    // Applying color filter to isolate blossoms.
    for ( int i = 0; i < hsvImage.rows; i++)
    {
        for (int j = 0; j < hsvImage.cols; j++)
        {
            // Filter based on saturation
            if (hsvImage.at<cv::Vec3b>(i,j)[1] > 30)
            {
                // Set value to 0 (black)
                hsvImage.at<cv::Vec3b>(i,j)[2] = 0;
            }
            // else
            // {
            //     hsvImage.at<cv::Vec3b>(i,j)[2] = 255;
            // }
            

            // Hue
            // hsvImage.at<cv::Vec3b>(i,j)[0] = 120;

            // Saturation
            // hsvImage.at<cv::Vec3b>(i,j)[1] = 255;

            // Value
            // hsvImage.at<cv::Vec3b>(i,j)[2] = 255;
        }
    }

    cv::Mat bgrImage;
    cv::cvtColor(hsvImage, bgrImage, cv::COLOR_HSV2BGR);

    // std::cout << filterImage.channels() << std::endl;// What is this line for?

    // // Applying color filter to isolate blossoms.
    // for ( int i = 0; i < filterImage.rows; i++)
    //     for (int j = 0; j < filterImage.cols; j++)
    //         if (((7 * (double)filterImage.at<cv::Vec3b>(i,j)[0] - 9 * (double)filterImage.at<cv::Vec3b>(i,j)[2] + 135) && (double)filterImage.at<cv::Vec3b>(i,j)[2] < 155)
    //             || !all_within((double)filterImage.at<cv::Vec3b>(i,j)[2], (double)filterImage.at<cv::Vec3b>(i,j)[1], (double)filterImage.at<cv::Vec3b>(i,j)[0], 50))
    //         {
    //             filterImage.at<cv::Vec3b>(i,j)[0] = 0;
    //             filterImage.at<cv::Vec3b>(i,j)[1] = 0;
    //             filterImage.at<cv::Vec3b>(i,j)[2] = 0;
    //         }


    // Converting image to binary
    cv::Mat BinaryImage = bgr_to_bin(bgrImage);

    // Write image to file for blob analysis
    cv::imwrite(blobImagePath, BinaryImage);

    return BinaryImage;
}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// From: https://docs.opencv.org/4.7.0/d6/d6e/group__imgproc__draw.html#ga746c0625f1781f1ffc9056259103edbc
cv::Mat blob_fill(std::string path)
{
    cv::Mat src = cv::imread(path, 0);
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    src = src > 1;
    // cv::namedWindow( "Source", 1 );
    // cv::imshow( "Source", src );
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( src, contours, hierarchy,
        cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        cv::Scalar color( 255, 255, 255 );// White
        // cv::Scalar color( rand()&255, rand()&255, rand()&255 );// Rand color
        cv::drawContours( dst, contours, idx, color, cv::FILLED );
    }
    // cv::namedWindow( "Components", 1 );
    // cv::imshow( "Components", dst );
    // // cv::waitKey(0);

    return dst;
}


// // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// // ::                                   ::                                   ::
// // ::                                   ::                                   ::
// // ::                                   ::                                   ::
// // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// cv::Mat blob_image(cv::Mat binaryImage, std::vector<std::vector<cv::Point>>* contours, std::string processedImagePath)//, int* numBlossoms)
// {
//     // Write to processed image file
//     cv::imwrite(processedImagePath, binaryImage);//copyImage);
//     cv::Mat binary2Image = blob_fill(contours, processedImagePath);
//     // cv::Mat binary2Image = binaryImage;//cv::imread(processedImagePath);

//     // ////////////////////////////////////////////////////////////////////COMMENT
    
//     // std::vector<cv::Vec4i> hierarchy;
//     // cv::findContours(binary2Image, *contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);


//     // ///////////////////////////////////////////////////////////////////////
//     // cv::Mat dst = cv::Mat::zeros(binary2Image.rows, binary2Image.cols, CV_8UC3);
//     // for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
//     // {
//     //     cv::Scalar color( 255, 255, 255 );
//     //     drawContours( dst, *contours, idx, color, cv::FILLED );
//     // }
//     // ///////////////////////////////////////////////////////////////////////

//     // cv::namedWindow( "dst", 1 );
//     // imshow( "dst", dst );

//     // cv::Mat drawing;
//     // cv::cvtColor(binary2Image, drawing, cv::COLOR_GRAY2BGR);

//     return binary2Image;
// }


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat remove_noise(cv::Mat src)
{
    cv::Mat dst;
    cv::medianBlur(src, dst, 5);

    return dst;
}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat count_blobs(cv::Mat inputImage, int* numBlossoms)
{
    int BlossomsDetected = 0;

    cv::Mat binaryImage = bgr_to_bin(inputImage);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binaryImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );

    // imshow("drawing0", drawing);

    // Converting image to bgr
    cv::Mat drawing;
    cv::cvtColor(binaryImage, drawing, cv::COLOR_GRAY2BGR);

    // Count blossoms and but box around them
    for (int pointer = 0; pointer < contours.size(); pointer++)
    {
        // If the contour is the correct size to be a blossom
        if (cv::contourArea(contours[pointer]) > 1000)// && cv::contourArea(contours[pointer]) < 3000)
        {
            BlossomsDetected = BlossomsDetected + 1;

            // // Fill contour
            // cv::Scalar color( 255, 255, 255 );
            // cv::drawContours(drawing, contours, -1, color, cv::FILLED);

            // Only show boxes if debug is true
            if (BLOSSOM_DEBUG_SHOW_BOXES)
            {
                cv::approxPolyDP( contours[pointer], contours_poly[pointer], 3, true );
                boundRect[pointer] = boundingRect( contours_poly[pointer] );

                cv::Scalar color = cv::Scalar(50 + rand() % 206, 50 + rand() % 206, 50 + rand() % 206);
                cv::rectangle( drawing, boundRect[pointer].tl(), boundRect[pointer].br(), color, 2 );
            }
        }
        // If not, make the contour black
        else
        {
            cv::drawContours(drawing, contours, pointer, cv::Scalar(0, 0, 0), cv::FILLED);
        }
    }


    // Save number of blossoms
    *numBlossoms = BlossomsDetected;

    // imshow("drawing", drawing);
    // return ThresholdImage;
    return drawing;
}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Filter image
int filter_image_pre(std::string processedImagePath, std::string originalImagePath)
{
    // The number of blossoms
    int numBlossoms = -404;

    // Filter the image using the algorithm
    cv::Mat originalImage = cv::imread(originalImagePath);

    cv::Mat binaryImage = binary_filter(originalImage, processedImagePath);
    cv::Mat blobImage   = blob_fill(processedImagePath);
    cv::Mat filterImage = remove_noise(blobImage);
    // cv::Mat binaryImage2 = binary_filter(blobImage, processedImagePath);
    // cv::Mat blobImage2   = blob_fill(processedImagePath);
    // cv::Mat filterImage2 = remove_noise(blobImage2);
    cv::Mat finalImage  = count_blobs(filterImage, &numBlossoms);

    // Remove this on final
    if (BLOSSOM_DEBUG_SHOW_IMAGES)
    {
        imshow("finalImage", finalImage);
        imshow("filterImage", filterImage);
        imshow("blobImage", blobImage);
        imshow("binaryImage", binaryImage);
        imshow("originalImage", originalImage);
    }

    // Write to processed image file
    cv::imwrite(processedImagePath, finalImage);//finalImage);

    return numBlossoms;
}
