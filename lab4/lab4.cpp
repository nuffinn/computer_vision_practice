#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <regex>

using namespace cv;

int main(){
   std::string imgPath = "./hand.png";

   Mat img = imread(imgPath, IMREAD_COLOR);

   Mat imgHSV ;
   cvtColor(img, imgHSV, COLOR_BGR2HSV);
   Scalar lowerBound(0, 40, 70);  // Adjust these values based on your specific image
   Scalar upperBound(20, 255, 255); // Adjust these values based on your specific image

   Mat mask;
   inRange(imgHSV, lowerBound, upperBound, mask);

   Mat segmentedHand;
   img.copyTo(segmentedHand, mask);

   imwrite("./segmented_hand.png", segmentedHand);
   return 0;
}
