#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

int main(int argc, char** argv)
{
	namedWindow("Before", CV_WINDOW_AUTOSIZE);

	// Load the source image
	Mat src = imread("02.jpg", 1);

	// Create a destination Mat object
	Mat dst;

	// display the source image
	imshow("Before", src);

	for (int i = 1; i<51; i = i + 2)
	{	medianBlur(src, dst, i);
	    imshow("Median filter", dst);
		char x = cvWaitKey(33);
		if (x == 27) {
			break;
		}
	}
	
	
}