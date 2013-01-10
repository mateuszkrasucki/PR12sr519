#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "Const.h"
#include "stdio.h"

using namespace cv;
using namespace std;

class ImageGenerator {
private:
	int *tablica;
public:
	int generate(int *tab) {
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

		/*This function creates an image with name ‘image’*/
		cvNamedWindow("Edges", CV_WINDOW_AUTOSIZE); 
		//showing the image
		cvShowImage("Edges", img);
		/*saving the image parameters name with Extension*/
		cvSaveImage("../PR12sr519/TestImages/output.jpg", img);
		//wait for any key and Exit
		cvWaitKey(0);
		//release img
		cvReleaseImage(&img);
		//destroy the window
		cvDestroyWindow("Edges");
		return 0;
	}

};
