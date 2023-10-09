#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <regex>

using namespace cv;

using namespace std;

string extractFileName(string imgPath){
   regex pattern(R"(/([^/]+)\.png)");
   smatch matches;
   regex_search(imgPath, matches, pattern);
   string fileName = matches[1];
   return fileName;
}

void convertAndExportHSV(string imgPath){
   Mat img, imgHSV;
   // store img in Mat img with rgb color
   img = imread(imgPath, IMREAD_COLOR);
   // convert to HSV
   cvtColor(img, imgHSV, COLOR_BGR2HSV);
   //extract file name
   string outputName = extractFileName(imgPath);
   // export hsv img
   imwrite("./"+ outputName + "HSV.png", imgHSV);
   // set value property to max
   for(int i = 0; i < imgHSV.rows; i++){
      for(int j = 0; j < imgHSV.cols; j++){
         imgHSV.at<Vec3b>(i,j)[2] = 255;
      }
   }
   // export modified img
   imwrite("./"+ outputName + "HSV_modified.png", imgHSV);
}

void convertAndExportLAB(string imgPath){
   Mat img, imgLAB;
   // store img in Mat img with rgb color
   img = imread(imgPath, IMREAD_COLOR);
   // convert to LAB
   cvtColor(img, imgLAB, COLOR_BGR2Lab);
   //extract file name
   string outputName = extractFileName(imgPath);
   // export LAB img
   imwrite("./"+ outputName + "LAB.png", imgLAB);
   //set L property to 100.0
   vector<Mat> channels;
   split(imgLAB, channels);
   channels[0] = 100.0;
   merge(channels, imgLAB);
   //export the modified LAB image
   imwrite("./"+ outputName + "LAB_modified.png", imgLAB);
}

int main(){
   string mountainImgPath = "/home/starburst/Downloads/mountain.png" ;
   string faceImgPath = "/home/starburst/Downloads/face.png" ;
   convertAndExportHSV(mountainImgPath);
   convertAndExportHSV(faceImgPath);
   convertAndExportLAB(mountainImgPath);
   convertAndExportLAB(faceImgPath);
   return 0;
}
