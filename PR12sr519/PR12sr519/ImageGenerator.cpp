#include <opencv2/highgui/highgui.hpp>
#include "opencv2/stitching/stitcher.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <iostream>
#include "Const.h"
#include "stdio.h"
#include <stdio.h>
#include <stdarg.h>

using namespace cv;
using namespace std;

class ImageGenerator {
private:
	int *tablica;
public:
	int generate(int *tab, string id) {
		tablica = tab;

		/*creating image*/
		IplImage *img = cvCreateImage(cvSize(IMG_SIZE_j, IMG_SIZE_i), IPL_DEPTH_8U, 1);
		int i,j,k,l;
		uchar *data;
		data = (uchar *) img->imageData;

		for (i = 0; i < (IMG_SIZE_i*IMG_SIZE_j); i++) {
				if (tab[i]==1) {
					data[i] = 0;
				} else {
					data[i] = 255;
				}
		}
		
		/*nameing convention of output files*/
		string filename = "../PR12sr519/TestImages/Output/final_at_"+id+".jpg";
		/*This function creates an image with name ‘filename’*/
		//s	cvNamedWindow(filename.c_str(), CV_WINDOW_AUTOSIZE); 
		//showing the image
		//	cvShowImage(filename.c_str(), img);
		/*saving the image parameters name with Extension*/
		cvSaveImage(filename.c_str(), img);

		//wait for any key and Exit
		//	cvWaitKey(0);
		//release img
		//	cvReleaseImage(&img);
		//destroy the window
		//s	cvDestroyWindow(filename.c_str());
		return 0;
	}

	int generateBoth(int *tab, int *tab2, string id, string input) {
		tablica = tab;

		/*creating image*/
		IplImage *img = cvCreateImage(cvSize(IMG_SIZE_j, IMG_SIZE_i), IPL_DEPTH_8U, 1);
		int i,j,k,l;
		uchar *data;
		data = (uchar *) img->imageData;

		for (i = 0; i < (IMG_SIZE_i*IMG_SIZE_j); i++) {
				if (tab[i]==1) {
					data[i] = 170;
				} else {
					data[i] = 0;
				}
		}

		IplImage *img2 = cvCreateImage(cvSize(IMG_SIZE_j, IMG_SIZE_i), IPL_DEPTH_8U, 1);

		uchar *data2;
		data2 = (uchar *) img2->imageData;

		for (i = 0; i < (IMG_SIZE_i*IMG_SIZE_j); i++) {
				if (tab2[i]==1) {
					data2[i] = 0;
				} else {
					data2[i] = 255;
				}
		}
		
		/*nameing convention of output files*/
		string filename = "../PR12sr519/TestImages/output/visit_at_"+id+".jpg";
		string filename2 = "../PR12sr519/TestImages/output/found_at_"+id+".jpg";
		string filename_output = "Edges found after "+id;

		cvSaveImage(filename.c_str(), img);
		cvSaveImage(filename2.c_str(), img2);

		IplImage *img1a = cvLoadImage(filename.c_str());
		IplImage *img2b = cvLoadImage(filename2.c_str());
		IplImage *img2c = cvLoadImage(input.c_str());
		const char* file = filename_output.c_str();

		char *cstr = new char[filename_output.length() + 1];
		strcpy(cstr, filename_output.c_str());
		cvShowManyImages(cstr, 3, img1a, img2b, img2c);
		delete [] cstr;

		return 0;
	}

	void cvShowManyImages(char* title, int nArgs, ...) {

		IplImage *img;
		IplImage *DispImage;

		int size;
		int i;
		int m, n;
		int x, y;

		// w - Maximum number of images in a row 
		// h - Maximum number of images in a column 
		int w, h;

		// scale - how much we have to resize the image
		float scale;
		int max;

		// Determine the size of the image, 
		// and the number of rows/cols 
		// from number of arguments 
		if(nArgs <= 0) {
			printf("Number of arguments too small....\n");
			return;
		}
		else if(nArgs > 12) {
			printf("Number of arguments too large....\n");
			return;
		}
		else if (nArgs == 1) {
			w = h = 1;
			size = 300;
		}
		else if (nArgs == 2) {
			w = 2; h = 1;
			size = 300;
		}
		else if (nArgs == 3 || nArgs == 4) {
			w = 3; h = 1;
			size = 300;
		}
		else if (nArgs == 5 || nArgs == 6) {
			w = 3; h = 2;
			size = 200;
		}
		else if (nArgs == 7 || nArgs == 8) {
			w = 4; h = 2;
			size = 200;
		}
		else {
			w = 4; h = 3;
			size = 150;
		}

		// Create a new 3 channel image
		DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, 3 );

		va_list args;
		va_start(args, nArgs);

		for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
			// Get the Pointer to the IplImage
			img = va_arg(args, IplImage*);
			// If it is NULL, release the image, and return
			if(img == 0) {
				printf("Blad -");
				cvReleaseImage(&DispImage);
				return;
			}

			x = img->width;
			y = img->height;

			max = (x > y)? x: y;

			scale = (float) ( (float) max / size );

			if( i % w == 0 && m!= 20) {
				m = 20;
				n+= 20 + size;
			}

			// Set the image ROI to display the current image
			cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));

			// Resize the input image and copy the it to the Single Big Image
			cvResize(img, DispImage);

			// Reset the ROI in order to display the next image
			cvResetImageROI(DispImage);
		}

		// Create a new window, and show the Single Big Image
		cvNamedWindow(title, 1);
		cvShowImage(title, DispImage);
		cvWaitKey();
		cvDestroyWindow(title);
		va_end(args);

		// Release the Image Memory
		cvReleaseImage(&DispImage);
	}

};
