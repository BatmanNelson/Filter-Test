#pragma once

// bool all_within(double r, double g, double b, int range);

/*
    Here are the image filters
*/


// Filter image
cv::Mat filterImage(cv::Mat inputImage, int* numBlossoms)
{
    // If debug is enabled
    const bool BLOSSOM_DEBUG = true;


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

    // ///////////////////
    // std::vector<std::vector<cv::Point>> contours;
    // std::vector<cv::Vec4i> hierarchy;
    // cv::findContours(BinaryImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // // cv::Mat BlobImage;
    // // cv::cvtColor(UnBinary, BlobImage, cv::COLOR_GRAY2BGR);

    // for(int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
    // {
    //     // cv::Scalar color( rand()&255, rand()&255, rand()&255 );
    //     cv::drawContours(UnBinary, contours, idx, 255, cv::FILLED, 8, hierarchy);
    // }
    

    // // std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    // // std::vector<cv::Rect> boundRect( contours.size() );

    // // cv::Mat drawing;
    // // cv::cvtColor(UnBinary, drawing, cv::COLOR_GRAY2BGR);

    // // des = cv2.bitwise_not(gray)
    // // contour,hier = cv2.findContours(des,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)

    // // for cnt in contour:
    // //     cv2.drawContours(des,[cnt],0,255,-1)

    // // gray = cv2.bitwise_not(des)

    // ///////////////////

    // // Invert image again (back to original b/w)
    // cv::Mat UnBinary;
    // cv::bitwise_not(BinaryImage, UnBinary);



    // // Get rid of noise
    // cv::Mat medianBlur;
    // cv::medianBlur(UnBinary, medianBlur, 1);

    // cv::Mat gaussianBlur;
    // cv::GaussianBlur(UnBinary, gaussianBlur, cv::Size(1,1), 9, 9);

    // cv::Mat bgrImageMedian;
    // cv::cvtColor(gaussianBlur, bgrImageMedian, cv::COLOR_GRAY2BGR);
    // cv::Mat bgrImageGauss;
    // cv::cvtColor(gaussianBlur, bgrImageGauss, cv::COLOR_GRAY2BGR);

    // imshow("bgrImageGauss", bgrImageGauss);
    // imshow("bgrImageMedian", bgrImageMedian);
    // imshow("medianBlur", medianBlur);
    // imshow("gaussianBlur", gaussianBlur);



    // // imshow("BlobImage", BlobImage);
    // imshow("UnBinary", UnBinary);
    // imshow("BinaryImage", BinaryImage);
    // imshow("ThresholdImage", ThresholdImage);
    // imshow("grayImage", grayImage);
    // imshow("bgrImage", bgrImage);
    // imshow("hsvImage", hsvImage);
    // imshow("filterImage", filterImage);


    ////////////////////////////////////////////////////////////////////COMMENT
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(UnBinary, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);


    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );

    ///////////////////////////////////////////////////////////////////////
    cv::Mat dst = cv::Mat::zeros(filterImage.rows, filterImage.cols, CV_8UC3);
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
    {
        cv::Scalar color( 255, 255, 255 );
        drawContours( dst, contours, idx, color, cv::FILLED );
    }
    ///////////////////////////////////////////////////////////////////////

    cv::namedWindow( "dst", 1 );
    imshow( "dst", dst );

    cv::Mat drawing;
    cv::cvtColor(UnBinary, drawing, cv::COLOR_GRAY2BGR);


    int BlossomsDetected = 0;

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

    // return drawing;
    imshow("drawing", drawing);
    return ThresholdImage;
}


// Filter image
int filterImagePre(std::string processedImagePath, std::string originalImagePath)
{
    // The number of blossoms
    int numBlossoms = -404;

    // Filter the image using the algorithm
    cv::Mat originalImage = cv::imread(originalImagePath);
    cv::Mat copyImage = filterImage(originalImage, &numBlossoms);

    // cv::Mat binaryImage;
    // cv::Mat blobImage;
    // cv::Mat filterImage;

    // numBlossoms = countBlobs;

    // Write to processed image file
    cv::imwrite(processedImagePath, copyImage);

    return numBlossoms;
}


// // Check if all numbers are within range
// bool all_within(double r, double g, double b, int range)
// {
//     int div = 3;

//     if (r > g + range / div || r < g - range)
//     {
//         return false;
//     }
//     if (r > b + range / div || r < b - range)
//     {
//         return false;
//     }
//     if (g > b + range / div || g < b - range)
//     {
//         return false;
//     }
    
//     return true;
// }
