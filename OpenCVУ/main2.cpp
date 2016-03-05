#include <opencv/highgui.h>
#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>


CvHaarClassifierCascade *cascade, *cascade1;
CvMemStorage            *storage, *storage1;
int number=0;
void detect(IplImage *img);

int main(int argc, char** argv)
{
  CvCapture *capture;
  IplImage  *frame;
  int input_resize_percent = 100;
  
 

  cascade = (CvHaarClassifierCascade*) cvLoad("car.xml", 0, 0, 0);
  storage = cvCreateMemStorage(0);
  capture = cvCaptureFromAVI("vid.mp4");
  
  cascade1 = (CvHaarClassifierCascade*)cvLoad("person.xml", 0, 0, 0);
  storage1 = cvCreateMemStorage(0);

  assert(cascade && storage && capture);
  cvNamedWindow("video", 1);
  IplImage* frame1 = cvQueryFrame(capture);
  frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);
  const int KEY_SPACE  = 32;
  const int KEY_ESC    = 27;
  int key = 0;
  do
  { frame1 = cvQueryFrame(capture);

    if(!frame1)
      break;

    cvResize(frame1, frame);
	detect(frame);

    key = cvWaitKey(10);

    if(key == KEY_SPACE)
      key = cvWaitKey(0);

    if(key == KEY_ESC)
      break;

  }while(1);

  cvDestroyAllWindows();
  cvReleaseImage(&frame);
  cvReleaseCapture(&capture);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseHaarClassifierCascade(&cascade1);
  cvReleaseMemStorage(&storage);
  cvReleaseMemStorage(&storage1);

  return 0;
}

void detect(IplImage *img)
{
CvFont font;
int z = 0, zz = 0, array1[123][3], array[123][3];
  CvSize img_size = cvGetSize(img);
  CvSeq *object = cvHaarDetectObjects( img,    cascade,    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(20,20),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );

  //std::cout << "Total: " << object->total << " cars" << std::endl;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img,  cvPoint(r->x, r->y),   cvPoint(r->x + r->width, r->y + r->height),      CV_RGB(255, 0, 0),1);  
	array[z][0]= r->y;
	array[z][0] = r->y + r->height;
	z++;
  }
    //cvPutText(img, "person", ), &font, cvScalar(255, 255, 255));
  

   CvSeq *object1 = cvHaarDetectObjects(img, cascade1, storage1,
	  1.1, //1.1,//1.5, //-------------------SCALE FACTOR
	  1, //2        //------------------MIN NEIGHBOURS
	  0, //CV_HAAR_DO_CANNY_PRUNING
	  cvSize(20, 20),//cvSize( 30,30), // ------MINSIZE
	  img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
	  );

  //std::cout << "Total: " << object->total << " cars" << std::endl;
  for (int i = 0; i < (object1 ? object1->total : 0); i++)
  {
	  CvRect *r1 = (CvRect*)cvGetSeqElem(object1, i);
	  cvRectangle(img, cvPoint(r1->x, r1->y), cvPoint(r1->x + r1->width, r1->y + r1->height), CV_RGB(0, 0, 255), 1);
	  array1[zz][0] = r1->y;
	  array1[zz][1] = r1->x + r1->width;
	  zz++;
  }
  //cvPutText(img, "person", ), &font, cvScalar(255, 255, 255));



  if (z>0 && zz>0) {
	  for (int i = 0; i < zz; i++)
		  for (int j = 0; j < z; j++)
			  if (array1[i][1] >= array[j][0] && array1[i][1] <= array[j][1]) {

				  std::cout << "narushenie" << std::endl;

				  char filename[512];

				  sprintf(filename, "Image%d.jpg", number);
				  printf("[i] capture... %s\n", filename);
				  cvSaveImage(filename, img);
				  number++;


			  }

  }

  cvShowImage("video", img);
}

/*
#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

CvHaarClassifierCascade *cascade, *cascade1;
CvMemStorage            *storage;

void detect(IplImage *img);

int main(int argc, char** argv)
{
CvCapture *capture,*capture1;
IplImage  *frame;
int input_resize_percent = 100;



cascade = (CvHaarClassifierCascade*) cvLoad("car.xml", 0, 0, 0);
storage = cvCreateMemStorage(0);
capture = cvCaptureFromAVI("video2.avi");

cascade1 = (CvHaarClassifierCascade*)cvLoad("person.xml", 0, 0, 0);

assert(cascade && storage && capture);
cvNamedWindow("video", 1);
IplImage* frame1 = cvQueryFrame(capture);
frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);
const int KEY_SPACE  = 32;
const int KEY_ESC    = 27;
int key = 0;
do
{ frame1 = cvQueryFrame(capture);

if(!frame1)
break;

cvResize(frame1, frame);
detect(frame);

key = cvWaitKey(33);

if(key == KEY_SPACE)
key = cvWaitKey(0);

if(key == KEY_ESC)
break;

}while(1);

cvDestroyAllWindows();
cvReleaseImage(&frame);
cvReleaseCapture(&capture);
cvReleaseHaarClassifierCascade(&cascade);
cvReleaseMemStorage(&storage);

return 0;
}

void detect(IplImage *img)
{
CvFont font;

CvSize img_size = cvGetSize(img);
CvSeq *object = cvHaarDetectObjects( img,    cascade,    storage,
1.1, //1.1,//1.5, //-------------------SCALE FACTOR
1, //2        //------------------MIN NEIGHBOURS
0, //CV_HAAR_DO_CANNY_PRUNING
cvSize(20,20),//cvSize( 30,30), // ------MINSIZE
img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
);

std::cout << "Total: " << object->total << " cars" << std::endl;
for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
{
CvRect *r = (CvRect*)cvGetSeqElem(object, i);
cvRectangle(img,  cvPoint(r->x, r->y),   cvPoint(r->x + r->width, r->y + r->height),      CV_RGB(255, 0, 0),1);  }
//cvPutText(img, "person", cvPointx), &font, cvScalar(255, 255, 255));

cvShowImage("video", img);
}*/