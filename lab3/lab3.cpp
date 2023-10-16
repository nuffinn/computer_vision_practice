#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <regex>

using namespace cv;

Mat inverseImage(Mat img){
   Mat img_inverse;
   img_inverse = 255 - img;
   return img_inverse;
}

int main() {
   std::string img1Path = "img1.png";
   std::string img2Path = "img2.png";

   Mat img1 = imread(img1Path, IMREAD_GRAYSCALE);
   Mat img2_unfix = imread(img2Path, IMREAD_GRAYSCALE);
   Mat img2;

   resize(img2_unfix, img2, Size(206, 167));

   Mat img1_inverse, img2_inverse;

   img1_inverse = inverseImage(img1);
   imwrite("./img1_inverse.png", img1_inverse);

   img2_inverse = inverseImage(img2);
   imwrite("./img2_inverse.png", img2_inverse);

   Mat onlyLeftBall;
   add(img1,img2_inverse, onlyLeftBall);
   imwrite("./onlyLeftBall.png", onlyLeftBall);

   Mat onlyRightBall;
   add(img2,img1_inverse, onlyRightBall);
   imwrite("./onlyRightBall.png", onlyRightBall);

   Mat bothBalls;
   bitwise_and(onlyLeftBall,onlyRightBall, bothBalls);
   imwrite("./bothBalls.png", bothBalls);
}
