#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <math.h>
//opencv lib
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>


CvCapture* capture = NULL;
IplImage* image = 0;
int number = 0;
int main(int argc, char* argv[])
{	//CvCapture* capture =  cvCreateCameraCapture(CV_CAP_ANY);
	//cv::VideoCapture capture(0);
	CvCapture* capture = cvCreateFileCapture("video2.avi");
	cvNamedWindow("capture", 0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, 8);
	
	while (true)
	{
		image = cvQueryFrame(capture);
		cv::Mat src = cv::cvarrToMat(image);
		cv::Mat gray;
		cvtColor(src, gray, cv::COLOR_BGR2GRAY);
		cv::CascadeClassifier cascadeSymbol;
		cv::CascadeClassifier cascadeSymbol1;
	    
		//car
		bool cascadeSymbolLoad = cascadeSymbol.load("cars.xml");
		std::vector<cv::Rect> symbols;
		cascadeSymbol.detectMultiScale(gray, symbols);
		int array1[1234][2], zz = 0;
		
		for (auto& c : symbols)
		{
			cv::Point symbolBegin = cv::Point(c.x, c.y);
			cv::Point symbolEnd = cv::Point(c.x + c.width, c.y + c.height);
			rectangle(src, symbolBegin, symbolEnd, cv::Scalar(0, 0, 255), 1);
			cvPutText(image, "car", symbolBegin, &font, cvScalar(255, 255, 255));
			array1[zz][0] = c.y;
			array1[zz][1] = c.y + c.height;
			zz++;
		}
	//person
		int array[1233],z=0;
		bool cascadeSymbol1Load = cascadeSymbol1.load("person.xml");
		std::vector<cv::Rect> symbols1;
		cascadeSymbol1.detectMultiScale(gray, symbols1);
		for (auto& p : symbols1)
		{
			cv::Point symbolBegin = cv::Point(p.x-5, p.y-5);
			cv::Point symbolEnd = cv::Point(p.x + p.width, p.y + p.height);
			rectangle(src, symbolBegin, symbolEnd, cv::Scalar(0, 255, 0), 1);
			cvPutText(image, "person", symbolBegin, &font, cvScalar(255, 255, 255));
			array[z++] = p.y + p.height;
		}
		if (z>0 && zz>0) {
			for (int i = 0; i < z; i++)
				for (int j = 0; j < zz; j++)
					if (array[i] >= array1[j][0] && array[i] <= array1[j][1]) {
					
						std::cout << "narushenie" << std::endl;
						
						char filename[512];
						
						sprintf(filename, "Image%d.jpg", number);
						printf("[i] capture... %s\n", filename);
						cvSaveImage(filename, image);
						number++;
					
					
	 				}
		
		}
		IplImage* img = new IplImage(src);
		cvShowImage("capture", img);
		char x = cvWaitKey(33);
		
		if (x == 27) {
			break;
		}
		/*
		*/
	}


	cvReleaseCapture(&capture);
	cvDestroyWindow("capture");
	return 0;
}