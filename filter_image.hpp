#pragma once

/*
    Here are the image filters
*/

// If debug is enabled
#define BLOSSOM_DEBUG true

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat binary_image(cv::Mat inputImage, std::string blobImagePath)
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
                // Set value to 0
                hsvImage.at<cv::Vec3b>(i,j)[2] = 0;
            }
            

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

    // Write image to file for blob analysis
    cv::imwrite(blobImagePath, UnBinary);

    return UnBinary;
}


// From: https://docs.opencv.org/4.7.0/d6/d6e/group__imgproc__draw.html#ga746c0625f1781f1ffc9056259103edbc
cv::Mat blob_fill(std::vector<std::vector<cv::Point>>* contours, std::string path)
{
    cv::Mat src = cv::imread(path, 0);
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    src = src > 1;
    // cv::namedWindow( "Source", 1 );
    // cv::imshow( "Source", src );
    // // std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( src, *contours, hierarchy,
        cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE );
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        cv::Scalar color( 255, 255, 255 );// White
        // cv::Scalar color( rand()&255, rand()&255, rand()&255 );// Rand color
        cv::drawContours( dst, *contours, idx, color, cv::FILLED );
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
// cv::Mat filter_image(cv::Mat drawing, std::vector<std::vector<cv::Point>> contours, int* numBlossoms)
// {
//     //
// }


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::                                   ::                                   ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cv::Mat count_blobs(cv::Mat drawing, std::vector<std::vector<cv::Point>> contours, int* numBlossoms)
{
    int BlossomsDetected = 0;

    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );

    // Count blossoms and but box around them
    for (int pointer = 0; pointer < contours.size(); pointer++)
    {
        if (cv::contourArea(contours[pointer]) > 1000)// && cv::contourArea(contours[pointer]) < 3000)
        {
            BlossomsDetected = BlossomsDetected + 1;

            // // Fill contures
            // cv::Scalar color( 255, 255, 255 );
            // cv::drawContours(drawing, contours, -1, color, cv::FILLED);

            // Only show boxes if debug is true
            if (BLOSSOM_DEBUG)
            {
                cv::approxPolyDP( contours[pointer], contours_poly[pointer], 3, true );
                boundRect[pointer] = boundingRect( contours_poly[pointer] );

                cv::Scalar color = cv::Scalar(50 + rand() % 206, 50 + rand() % 206, 50 + rand() % 206);
                cv::rectangle( drawing, boundRect[pointer].tl(), boundRect[pointer].br(), color, 2 );
            }
        }
    }


    // Save number of blossoms
    *numBlossoms = BlossomsDetected;

    // imshow("drawing", drawing);
    // return ThresholdImage;
    return drawing;
}


// Filter image
int filter_image_pre(std::string processedImagePath, std::string originalImagePath)
{
    // The number of blossoms
    int numBlossoms = -404;

    // Filter the image using the algorithm
    cv::Mat originalImage = cv::imread(originalImagePath);
    // cv::Mat copyImage = filterImage0(originalImage);//, &numBlossoms);

    // This is so blob_image, filter_image, and
    // count_blobs have access to the contours
    std::vector<std::vector<cv::Point>> contours;

    cv::Mat binaryImage = binary_image(originalImage, processedImagePath);
    cv::Mat blobImage   = blob_fill(&contours, processedImagePath);//, &numBlossoms);
    // cv::Mat filterImage = filter_image(blobImage, contours, &numBlossoms);
    cv::Mat finalImage  = count_blobs(blobImage, contours, &numBlossoms);

    // Write to processed image file
    cv::imwrite(processedImagePath, finalImage);//copyImage);

    return numBlossoms;
}
