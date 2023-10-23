#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat background_subtraction_processing(Mat background, Mat obj) {
   Mat mask;
   subtract(background, obj, mask); 
   return mask;
}

Mat lockOpening(Mat img){
   Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
   Mat eroded_image, lock_opening_result;
   erode(img, eroded_image, kernel, Point(-1, -1), 1);
   dilate(eroded_image, lock_opening_result, kernel, Point(-1, -1), 1);
   return lock_opening_result;
}

Mat BGR_Processing(Mat background, Mat obj) {
   Mat mask, result;
   
   mask = background_subtraction_processing(background, obj); 

   cvtColor(mask, mask, COLOR_BGR2GRAY);
   
   threshold(mask, mask, 0, 255, THRESH_BINARY);

   mask = lockOpening(mask);

   cvtColor(mask, mask, COLOR_GRAY2BGR);

   try{
      result = mask & obj;
   } catch(const cv::Exception& e) {
      std::cout << "here 2 " << e.what() << std::endl;
   }

   return result;
}

Mat HSV_Processing(Mat background, Mat obj) {
   Mat mask, result;

   cvtColor(background, background, COLOR_BGR2HSV);
   cvtColor(obj, obj, COLOR_BGR2HSV);
   
   mask = background_subtraction_processing(background, obj); 

   std::vector<Mat> hsvChannels;
   split(mask, hsvChannels);  
   Mat valueChannel = hsvChannels[2];

   threshold(valueChannel ,valueChannel  , 0, 255, THRESH_BINARY);

   hsvChannels[0] = 255;
   hsvChannels[1] = 255;

   hsvChannels[2] = valueChannel;
   merge(hsvChannels, mask);
   mask = lockOpening(mask);

   try{
      result = mask & obj;
      cvtColor(result, result, COLOR_HSV2BGR);
   } catch(const cv::Exception& e) {
      std::cout << "here 2 " << e.what() << std::endl;
   }

   return result;
}

int main() {
   std::string background_path = "./wall.jpg";
   std::string obj_path = "./hand_and_bottle.jpg";
   
   Mat backgroundImg = imread(background_path, IMREAD_COLOR);
   Mat objImg = imread(obj_path, IMREAD_COLOR);
   
   Mat resultBGR = BGR_Processing(backgroundImg, objImg);
   imwrite("./result_BGR.png", resultBGR);

   Mat resultHSV = HSV_Processing(backgroundImg, objImg);
   imwrite("./result_HSV.png", resultHSV);
}

