#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main() {
  Mat img = imread("./chessboard.jpg", IMREAD_COLOR);
  Mat grayImg;
  cvtColor(img, grayImg, COLOR_BGR2GRAY);
  Mat edges;
  Canny(grayImg, edges, 50, 200);

  std::vector<Vec4i> lines; // This time, we use Vec4i to represent lines
  HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);

  Mat resultImage = img.clone();
  for (size_t i = 0; i < lines.size(); i++) {
        line( resultImage, Point(lines[i][0], lines[i][1]),
        Point( lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
  }

  imwrite("./result.png", resultImage);

  return 0;
}

