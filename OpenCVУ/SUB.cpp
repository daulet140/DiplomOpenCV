
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

int g_slider_position = 0;
CvCapture* capture = NULL;
IplImage* image = 0;
IplImage* f = 0;
IplImage* ff = 0;
IplImage*	dst = 0;
IplImage* imagea = 0;
IplImage *img1 = 0, *img2 = 0, *diff = 0, *subs = 0;
IplImage* ch = 0;

IplImage* sub(IplImage* broad, IplImage* chs){
	img1 = cvCloneImage(broad);
	img2  = cvCloneImage(chs);
	diff = cvCloneImage(img1);
	subs = cvCloneImage(img1);
	/*cvZero(diff);
	for (int y = 0; y<diff->height; y++) {
		uchar* ptr1 = (uchar*)(img1->imageData + y * img1->widthStep);
		uchar* ptr2 = (uchar*)(img2->imageData + y * img2->widthStep);
		uchar* ptr = (uchar*)(diff->imageData + y * diff->widthStep);
		for (int x = 0; x<diff->width; x++) {
		    // B
			ptr[3 * x] = ptr1[3 * x] + ptr2[3 * x] - 2 * min(ptr1[3 * x], ptr2[3 * x]);
			// G
			ptr[3 * x + 1] = ptr1[3 * x + 1] + ptr2[3 * x + 1] - 2 * min(ptr1[3 * x + 1], ptr2[3 * x + 1]);
			// R
			ptr[3 * x + 2] = ptr1[3 * x + 2] + ptr2[3 * x + 2] - 2 * min(ptr1[3 * x + 2], ptr2[3 * x + 2]);
			//if (ptr[3*x]!=0|| ptr[3 * x+1] != 0 || ptr[3 * x+2] != 0  ) {			}
		}
	}*/
	// вычитаем 
	cvSub(img2, img1, subs);
	//cvThreshold(img2, img1, 50, 150, 1);
	//cvAnd(img2, img1, subs);
	//cvRectangle(subs, cvPoint(55,55), cvPoint(55,55), CV_RGB(255, 0, 0), 1, 8);
	//cvCvtColor(subs, subs, 1);
	//cvConvertImage(subs, subs, 1);
	//cvCanny(subs, subs, 10, 100, 3);
	return subs;
}
int main(int argc, char* argv[])
{
	//CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY);
	CvCapture* capture = cvCreateFileCapture("video2.avi");

	cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);

	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);

	double width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	double height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	CvSize size = CvSize(width, height);
	ff = cvCreateImage(size, IPL_DEPTH_8U, 3);
	f = cvCreateImage(size, IPL_DEPTH_8U, 1);
	printf("[i] %.0f x %.0f\n", width, height);
	
	int counter = 0;
	char filename[512];
	while (true)
	{
		image = cvQueryFrame(capture);
		//cvConvertImage(image, image, CV_RGB2GRAY);
	//	cvSmooth(image, image, CV_GAUSSIAN, 9, 0);
		//cvShowImage("capture", image);
	    //IplImage* ch = cvLoadImage("Image0.jpg");
		if (ch!=0) {
			ff=sub(image, ch);
			
		}
	//	cvSmooth(ff, ff, CV_GAUSSIAN, 9, 0);

		
		ch = cvCloneImage(image);
	//    cvShowImage("capture", image);
		cvShowImage("capture", ff);

		char x = cvWaitKey(33);
		if (x == 27)
			break;
		else if (x == 13) { // Enter
							// сохраняем кадр в файл
			
		if (ch)
		{
				printf("file exist");
		}else {
				sprintf(filename, "Image%d.jpg", 0);
				printf("[i] capture... %s\n", filename);
				cvSaveImage(filename, image);
				counter++;
			}
		ch = cvCloneImage(image);
		}

	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("capture");
	return 0;
}