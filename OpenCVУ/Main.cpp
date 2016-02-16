#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\opencv.hpp>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <math.h>
using namespace cv;
using namespace std;
//  страница 55- 60
int g_slider_position = 0;
CvCapture* capture = NULL;
IplImage* image=0;
IplImage* f = 0;
IplImage* ff = 0;
IplImage*	dst = 0;
IplImage* imagea = 0;
IplImage *img1 = 0, *img2 = 0, *diff = 0, *subs = 0;
IplImage* ch = 0;


void EnclosingCircle(IplImage* _image)
{
	assert(_image != 0);

	IplImage* bin = cvCreateImage(cvGetSize(_image), IPL_DEPTH_8U, 1);

	// конвертируем в градации серого
	cvConvertImage(_image, bin, CV_BGR2GRAY);
	// находим границы
	cvCanny(bin, bin, 50, 200);

	cvNamedWindow("bin", 1);
	cvShowImage("bin", bin);

	// хранилище памяти для контуров
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	// находим контуры
	int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	assert(contours != 0);

	// обходим все контуры
	for (CvSeq* current = contours; current != NULL; current = current->h_next) {
		CvPoint2D32f center;
		float radius = 0;
		// находим параметры окружности
		cvMinEnclosingCircle(current, &center, &radius);
		// рисуем
		cvCircle(_image, cvPointFrom32f(center), radius, CV_RGB(255, 0, 0), 1, 8);
	}

	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&bin);
}
IplImage* sub(IplImage* broad, IplImage* chs) {
	img1 = cvCloneImage(broad);
	img2 = cvCloneImage(chs);

	diff = cvCloneImage(img1);
	subs = cvCloneImage(img1);

	cvZero(diff);

	// пробегаемся по всем пикселям изображения
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
		}
	}
	// вычитаем 
	cvSub(img2, img1, subs);
	return subs;
}

int main(int argc, char* argv[])
{
	IplImage *src = 0, *dst = 0;
	CvCapture* capture = cvCreateFileCapture("video2.avi");

	while (true)
	{
		image = cvQueryFrame(capture);
		// имя картинки задаётся первым параметром
		char* filename = argc >= 2 ? argv[1] : "02.jpg";
		// получаем картинку
		src = cvCloneImage(image);

		printf("[i] image: %s\n", filename);
		assert(src != 0);

		
		dst = cvCloneImage(src);

		// показываем
		EnclosingCircle(dst);

		cvNamedWindow("circles", 1);
		cvShowImage("circles", dst);

		// ждём нажатия клавиши
		char x = cvWaitKey(33);
	}

	// освобождаем ресурсы
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	// удаляем окна
	cvDestroyAllWindows();
	return 0;
}