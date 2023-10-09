#include <stdio.h>
#include <regex>
#include <opencv2/opencv.hpp>
#include <string.h>

using namespace cv;
using namespace std;

void exportHistogramChart(const cv::Mat& img, string name) {
    // Split the BGR image into its color channels
    std::vector<cv::Mat> channels;
    cv::split(img, channels);

    // Create histograms for each color channel (B, G, R)
    int num_bins = 256;  // Number of bins for the histogram
    float range[] = {0, 256}; // Range for pixel values
    const float* hist_range[] = {range};
    bool uniform = true;
    bool accumulate = false;
    cv::Mat b_hist, g_hist, r_hist;

    cv::calcHist(&channels[0], 1, 0, cv::Mat(), b_hist, 1, &num_bins, hist_range, uniform, accumulate);
    cv::calcHist(&channels[1], 1, 0, cv::Mat(), g_hist, 1, &num_bins, hist_range, uniform, accumulate);
    cv::calcHist(&channels[2], 1, 0, cv::Mat(), r_hist, 1, &num_bins, hist_range, uniform, accumulate);

    // Create histograms plot
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / num_bins);
    cv::Mat hist_image(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));

    // Normalize the histograms
    cv::normalize(b_hist, b_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // Draw histograms
    for (int i = 1; i < num_bins; i++) {
        cv::line(hist_image,
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, 8, 0);
        cv::line(hist_image,
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(hist_image,
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, 8, 0);
    }

    // Save the histogram chart as an image
    cv::imwrite(name + "_histogram_chart.png", hist_image);
}


string extractFileName(string imgPath){
   regex pattern(R"(/([^/]+)\.png)");
   smatch matches;
   regex_search(imgPath, matches, pattern);
   string fileName = matches[1];
   return fileName;
}

void equalizingHistogramBySplit(string path){
   Mat img, img_modified;
   string fileName = extractFileName(path);
   
   img = imread(path, IMREAD_COLOR);

   exportHistogramChart(img, fileName);

   vector<cv::Mat> channels;

   split(img, channels);

   equalizeHist(channels[0],channels[0]);
   equalizeHist(channels[1],channels[1]);
   equalizeHist(channels[2],channels[2]);

   cv::merge(channels, img_modified);

   exportHistogramChart(img_modified, fileName + "_RGB_equalized");
   imwrite("./" + fileName + "_RGB_equalized.png", img_modified);
   return ;
}

void equalizingHSV(string path){
   Mat img, imgHSV, img_modified, imgHSV_modified;
   vector<cv::Mat> channels;
   
   img = imread(path, IMREAD_COLOR);

   cvtColor(img, imgHSV, COLOR_BGR2HSV);

   split(imgHSV, channels);

   equalizeHist(channels[2], channels[2]);

   cv::merge(channels, imgHSV_modified);
   
   cvtColor(imgHSV_modified, img_modified, COLOR_HSV2BGR);

   string fileName = extractFileName(path);
   exportHistogramChart(img_modified, fileName + "_HSV_equalized");
   imwrite("./" + fileName + "_HSV_equalized.png", img_modified);
   return;
}

int main() {
   string mountainImgPath = "/home/starburst/Downloads/mountain.png" ;
   string faceImgPath = "/home/starburst/Downloads/face.png" ;
  
   equalizingHistogramBySplit(mountainImgPath);
   equalizingHistogramBySplit(faceImgPath);

   equalizingHSV(mountainImgPath);
   equalizingHSV(faceImgPath);
   return 0;
}
