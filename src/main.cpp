#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;
const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;
int thresh = 100;
int max_thresh = 255;
int areas[100];
float sumaX[100],sumaY[100];
bool esta[1000][1000];
void mouseCoordinatesExampleCallback(int event, int x, int y, int flags, void* param);
void mouseCallbackYIQ(int event, int x, int y, int flags, void* param);
void RGBtoYIQ(const Mat &sourceImage, Mat &destinationImage);
void restore(const Mat &sourceImage,const Mat &binImage, Mat &destinationImage);
void binarizeChannel(const Mat &sourceImage, int blowValue, int bhighValue,int glowValue, int ghighValue,int rlowValue, int rhighValue, Mat &destinationImage);
Mat display2(const Mat &mat_1, const Mat &mat_2);
Mat display2v(const Mat &mat_1, const Mat &mat_2);
int centrox, centroy = 0;

vector<Point> points;//Se guardan los puntos donde se hace click
int RGB[3];//Se guardan los valores BGR del punto donde se da click
int YIQ[3];//Se guardan los valores YIQ del punto donde se da click
const int dsv = 25;

struct Nodo {
   int x1;
   int y1;
   Nodo *next;
};

class list
{
		private:
		Nodo *head, *tail;
		public:
		list()
		{
			head=NULL;
			tail=NULL;
		}
		Nodo* gethead()
		{
			return head;
		}
		void insert(int value,int value2)
		{
			Nodo *temp = new Nodo;
			temp->x1=value;
			temp-> y1=value2;
			temp->next=NULL;
			if(head==NULL)
			{
				head=temp;
				tail=temp;
			}
			else
			{
				tail->next=temp;
				tail=temp;
			}
		}
		void cleanLinkedList()
		{
      while(head != NULL){
        Nodo *temp = head;
        head = head -> next;
        delete temp;
      }

		}
};

//////////////////###############END LINKED LIST#####################
list obj;
void detectobject(Mat &sourceImage,Mat &destinationImage)
{
	int i,j;
	int color[3]={30,30,30};
	int aux4=0;
	//DATOS PARA LA ACCESAR A LAS IMAGENES
	uint8_t* pixelPtr = (uint8_t*)sourceImage.data;
	int cn = sourceImage.channels();
	Scalar_<uint8_t> bgrPixel;
	uint8_t* pixelPtr2 = (uint8_t*)destinationImage.data;
	int cn2 = destinationImage.channels();
	Scalar_<uint8_t> bgrPixel2;
  struct Nodo* ptr;
	//RECORRES LA IMAGEN
		for (int x = 1; x<sourceImage.rows;x=x+100)
		{
			for(int y = 1; y<sourceImage.cols;y=y+100)
			{
				//Input image PARA QUE ME DE EL VALOR DEL PIXEL
				bgrPixel.val[0] = pixelPtr[x*sourceImage.cols*cn + y*cn + 0];
				bgrPixel.val[1] = pixelPtr[x*sourceImage.cols*cn + y*cn + 1];
				bgrPixel.val[2] = pixelPtr[x*sourceImage.cols*cn + y*cn + 2];
				//OutputImage
				bgrPixel2.val[0] = pixelPtr2[x*destinationImage.cols*cn2 + y*cn2 + 0];
				bgrPixel2.val[1] = pixelPtr2[x*destinationImage.cols*cn2 + y*cn2 + 1];
				bgrPixel2.val[2] = pixelPtr2[x*destinationImage.cols*cn2 + y*cn2 + 2];

				if (bgrPixel[0] == 255 && bgrPixel[1] == 255  && bgrPixel[2] == 255  && bgrPixel2.val[0] == 0 && bgrPixel2.val[1] == 0 && bgrPixel2.val[2] == 0)
				{
					//PONES EN FALSO PARA QUE NO ENTRE SI YA ESTA PINTADO EL INDEX
					esta[x][y]=false;
					obj.insert(x,y);
   				ptr = obj.gethead();
					//HASTA QUE LA LISTA SE VACIE
  				while (ptr != NULL)
					{
						i = ptr-> x1;
						j = ptr-> y1;
						destinationImage.at<Vec3b>(Point(j, i))[0] = color[0];
						destinationImage.at<Vec3b>(Point(j, i))[1] = color[1];
						destinationImage.at<Vec3b>(Point(j, i))[2] = color[2];



						if(i>0 && j>0 && i<(sourceImage.rows-1)&& j<(sourceImage.cols-1)){
						//Input image i-1
						bgrPixel.val[0] = pixelPtr[(i-1)*sourceImage.cols*cn + j*cn + 0];
						bgrPixel.val[1] = pixelPtr[(i-1)*sourceImage.cols*cn + j*cn + 1];
						bgrPixel.val[2] = pixelPtr[(i-1)*sourceImage.cols*cn + j*cn + 2];
						//OutputImage
						bgrPixel2.val[0] = pixelPtr2[(i-1)*destinationImage.cols*cn2 + j*cn2 + 0];
						bgrPixel2.val[1] = pixelPtr2[(i-1)*destinationImage.cols*cn2 + j*cn2 + 1];
						bgrPixel2.val[2] = pixelPtr2[(i-1)*destinationImage.cols*cn2 + j*cn2 + 2];
						if (bgrPixel[0] == 255 && bgrPixel[1] == 255  && bgrPixel[2] == 255  && bgrPixel2.val[0] == 0 && bgrPixel2.val[1] == 0 && bgrPixel2.val[2] == 0 && esta[i-1][j])
						{
							obj.insert(i-1,j);
							esta[i-1][j] = false;
						}
						//Input image i-1
						bgrPixel.val[0] = pixelPtr[i*sourceImage.cols*cn + (j+1)*cn + 0];
						bgrPixel.val[1] = pixelPtr[i*sourceImage.cols*cn + (j+1)*cn + 1];
						bgrPixel.val[2] = pixelPtr[i*sourceImage.cols*cn + (j+1)*cn + 2];
						//OutputImage
						bgrPixel2.val[0] = pixelPtr2[i*destinationImage.cols*cn2 + (j+1)*cn2 + 0];
						bgrPixel2.val[1] = pixelPtr2[i*destinationImage.cols*cn2 + (j+1)*cn2 + 1];
						bgrPixel2.val[2] = pixelPtr2[i*destinationImage.cols*cn2 + (j+1)*cn2 + 2];
						if (bgrPixel[0] == 255 && bgrPixel[1] == 255  && bgrPixel[2] == 255  && bgrPixel2.val[0] == 0 && bgrPixel2.val[1] == 0 && bgrPixel2.val[2] == 0 && esta[i][j+1])
						{
							obj.insert(i,j+1);
							esta[i][j+1] = false;
						}
						//Input image i-1
						bgrPixel.val[0] = pixelPtr[(i+1)*sourceImage.cols*cn + j*cn + 0];
						bgrPixel.val[1] = pixelPtr[(i+1)*sourceImage.cols*cn + j*cn + 1];
						bgrPixel.val[2] = pixelPtr[(i+1)*sourceImage.cols*cn + j*cn + 2];
						//OutputImage
						bgrPixel2.val[0] = pixelPtr2[(i+1)*destinationImage.cols*cn2 + j*cn2 + 0];
						bgrPixel2.val[1] = pixelPtr2[(i+1)*destinationImage.cols*cn2 + j*cn2 + 1];
						bgrPixel2.val[2] = pixelPtr2[(i+1)*destinationImage.cols*cn2 + j*cn2 + 2];
						if (bgrPixel[0] == 255 && bgrPixel[1] == 255  && bgrPixel[2] == 255  && bgrPixel2.val[0] == 0 && bgrPixel2.val[1] == 0 && bgrPixel2.val[2] == 0 && esta[i+1][j])
						{
							obj.insert(i+1,j);
							esta[i+1][j] = false;
						}
						bgrPixel.val[0] = pixelPtr[i*sourceImage.cols*cn + (j-1)*cn + 0];
						bgrPixel.val[1] = pixelPtr[i*sourceImage.cols*cn + (j-1)*cn + 1];
						bgrPixel.val[2] = pixelPtr[i*sourceImage.cols*cn + (j-1)*cn + 2];
						//OutputImage
						bgrPixel2.val[0] = pixelPtr2[i*destinationImage.cols*cn2 + (j-1)*cn2 + 0];
						bgrPixel2.val[1] = pixelPtr2[i*destinationImage.cols*cn2 + (j-1)*cn2 + 1];
						bgrPixel2.val[2] = pixelPtr2[i*destinationImage.cols*cn2 + (j-1)*cn2 + 2];
                       if (bgrPixel[0] == 255 && bgrPixel[1] == 255  && bgrPixel[2] == 255  && bgrPixel2.val[0] == 0 && bgrPixel2.val[1] == 0 && bgrPixel2.val[2] == 0 && esta[i][j-1])
						{
							obj.insert(i,j-1);
							esta[i][j-1] = false;
						}
						}
						ptr = ptr->next;
						areas[aux4]++;
						sumaX[aux4] = sumaX[aux4] + j;
            sumaY[aux4] = sumaY[aux4] +i;
					}
					obj.cleanLinkedList();
					aux4++;
					color[0] = color[0] + 10;
					color[1] = color[1] + 40;
					color[2] = color[2] + 60;
				}
			}
		}
		cout<<"numero de objetos = "<<aux4<<endl;
		for(int i=0; i<aux4;i++)
		{

			cout<<"objeto "<<i+1<<" = "<<areas[i]<<endl;
      cout<<"x testada "<<i+1<<" = "<<sumaX[i]/areas[i]<<endl;
      cout<<"y testada "<<i+1<<" = "<<sumaY[i]/areas[i]<<endl;
			centrox = sumaX[i]/areas[i];
			centroy = sumaY[i]/areas[i];
			Point p(centrox,centroy);
			circle(destinationImage, p ,5, Scalar(128,0,0),-1);

      areas[i] = 0;
      sumaX[i] = 0;
      sumaY[i] = 0;
		}
    obj.cleanLinkedList();
}

bool fre = false;
int main(int argc, char **argv)
{
	srand(time(NULL)); //seed for random colors
  /* First, open camera device */
  VideoCapture camera = VideoCapture(0);
  bool isCameraAvailable = camera.isOpened();
  /* Create images where captured and transformed frames are going to be stored*/
  Mat currentImage;
  Mat YIQimage;
  Mat aux,aux1;

  //Mat local;
  //Mat HSV_convertido;
  //vector<Mat> hsv_planes;
  Mat sepYIQ, sepImage;

  Mat YIQrest;
  /* Clean the terminal */
  cout << "\033[2J\033[1;1H";
  cout << "Basic Show Image \t|\tUse 'x' or 'Esc' to terminate execution\nUse space to freeze\n";
  /* Create main OpenCV window to attach callbacks */
  namedWindow("Original");
  namedWindow("YIQ");

  setMouseCallback("Original", mouseCoordinatesExampleCallback, &currentImage);
  setMouseCallback("YIQ",mouseCallbackYIQ, &YIQimage);

  bool update = true; //enable camera update
  while (true) {
    /* Obtain a new frame from camera */
    if (isCameraAvailable && update) {
      camera.read(currentImage);
    } /*else {
      currentImage = imread("PlaceholderImage.jpg", CV_LOAD_IMAGE_COLOR);
    }*/
    if (currentImage.size().width <= 0 && currentImage.size().height <= 0) {
      cout << "ERROR: Camera returned blank image, check connection\n";
      break;
    }
    //flipImageBasic(currentImage, flippedImage);/* Call custom flipping routine. From OpenCV, you could call flip(currentImage, flippedImage, 1) */
    RGBtoYIQ(currentImage,YIQimage);//Transforma la imagén a YIQ
    binarizeChannel(YIQimage,YIQ[0]-dsv,YIQ[0]+dsv,YIQ[1]-dsv,YIQ[1]+dsv,YIQ[2]-dsv,YIQ[2]+dsv,sepYIQ);//Binarisa sobre los valores en los que se dio click YIQ

    //restore(currentImage,sepYIQ,YIQrest); //Crea una imagen con los colores originales y la imagen binarizada

    imshow("Original", currentImage);
    //aux = display2v(YIQimage,sepYIQ);
    //aux1 = display2v(YIQrest,Mat (YIQrest.rows,YIQrest.cols,YIQrest.type(),Scalar(0,0,0)));
    //imshow("YIQ",display2(aux,aux1));
    imshow("YIQ",YIQimage);

		for(int i=0; i<1000;i++)
	{
		for(int j=0; j<1000;j++)
		{
			esta[i][j]= true;
		}
	}

	Mat binaryImage;
	Mat auximage;
	cvtColor( sepYIQ, auximage, CV_BGR2GRAY );
	for ( int i = 1; i < 13; i = i + 4 ){ medianBlur ( auximage, auximage, i );}
	threshold( auximage, binaryImage, 120, 255,THRESH_BINARY );
	Mat detectionimage = Mat(binaryImage.rows,binaryImage.cols,currentImage.type());
	detectionimage.setTo(Scalar(0,0,0));
	detectobject(binaryImage,detectionimage);
	imshow("detection",detectionimage);


    /* If 'x' is pressed, exit program */
    char key = waitKey(1);
    if(key == 'x' || key == 27 ){ // 27 = ESC
      break;
    } else if(key == 32){ // if 32=Space is pressed, freeze image
      update = !update;
    }
  }
	// im_in = imread("/home/diego96/Pictures/Webcam/prueba21.jpg", CV_LOAD_IMAGE_COLOR);

}

void RGBtoYIQ(const Mat &sourceImage, Mat &destinationImage)
//Convierte de RGB a YIQ
{
  int R,G,B;
  float Y,I,Q;

  if (destinationImage.empty())
		destinationImage = Mat(sourceImage.rows, sourceImage.cols, sourceImage.type());

	for (int y = 0; y < sourceImage.rows; ++y)
		for (int x = 0; x < sourceImage.cols; ++x){
      //Extrae los componentes de cada color
      B = sourceImage.at<Vec3b>(y, x)[0];
      G = sourceImage.at<Vec3b>(y, x)[1];
      R = sourceImage.at<Vec3b>(y, x)[2];
      //Conversion a YIQ
      Y = (0.299*R + 0.587*G + 0.114*B);
      I = (0.596*R - 0.275*G - 0.321*B)+128;
      Q = (0.212*R - 0.523*G + 0.311*B)+128;
      //Guardar los valores en la nueva matriz
      destinationImage.at<Vec3b>(y, x)[0] = Y;
      destinationImage.at<Vec3b>(y, x)[1] = I;
      destinationImage.at<Vec3b>(y, x)[2] = Q;
    }
}

void binarizeChannel(const Mat &sourceImage, int blowValue, int bhighValue,int glowValue, int ghighValue,int rlowValue, int rhighValue, Mat &destinationImage)
//Binariza una imagen en 3 canales, recibiendo los valores altos y bajos para binarizar
{
  if (destinationImage.empty())
    destinationImage = Mat(sourceImage.rows, sourceImage.cols, sourceImage.type());
  for(int y = 0; y < sourceImage.rows; ++y){
    for(int x = 0; x < sourceImage.cols; ++x){
      if (sourceImage.at<Vec3b>(y,x)[0]>blowValue && sourceImage.at<Vec3b>(y,x)[0]<bhighValue && sourceImage.at<Vec3b>(y,x)[1]>glowValue && sourceImage.at<Vec3b>(y,x)[1]<ghighValue && sourceImage.at<Vec3b>(y,x)[2]>rlowValue && sourceImage.at<Vec3b>(y,x)[2]<rhighValue)
        destinationImage.at<Vec3b>(y,x) = sourceImage.at<Vec3b>(y,x);
      else{
        destinationImage.at<Vec3b>(y,x)[0] = 0;
        destinationImage.at<Vec3b>(y,x)[1] = 0;
        destinationImage.at<Vec3b>(y,x)[2] = 0;
      }
    }
  }
}


void restore(const Mat &sourceImage, const Mat &binImage, Mat &destinationImage)
//Crea una imagen con los colores originales RGB de la sección binarizada
{
  if (destinationImage.empty())
    destinationImage = Mat(sourceImage.rows, sourceImage.cols, sourceImage.type());
  for(int y = 0; y < sourceImage.rows; ++y){
    for(int x = 0; x < sourceImage.cols; ++x){
      if(binImage.at<Vec3b>(y,x)[0])
        destinationImage.at<Vec3b>(y,x) = sourceImage.at<Vec3b>(y,x);
      else{
        destinationImage.at<Vec3b>(y,x)[0] = 0;
        destinationImage.at<Vec3b>(y,x)[1] = 0;
        destinationImage.at<Vec3b>(y,x)[2] = 0;
      }
    }
  }
}

void mouseCoordinatesExampleCallback(int event, int x, int y, int flags, void* param)
//Regresa los valores al arreglo de RGB y los muestra en consola
{
    Mat &currentImage = *(Mat*)param;
    int R = currentImage.at<Vec3b>(y,x)[2];
    int G = currentImage.at<Vec3b>(y,x)[1];
    int B = currentImage.at<Vec3b>(y,x)[0];
    switch (event)
    {
        case CV_EVENT_LBUTTONDOWN:
            cout << "  Mouse X, Y: " << x << ", " << y << "   ";
            cout << "R: " << R<< " G: " << G<< " B: " << B<< endl;
            /*  Draw a point */
            points.push_back(Point(x, y));
            RGB[2] = R;
            RGB[1] = G;
            RGB[0] = B;
            break;
        case CV_EVENT_RBUTTONDOWN:
            points.clear();
            break;
    }
}
void mouseCallbackYIQ(int event, int x, int y, int flags, void* param)
//Regresa los valores al arreglo de YIQ y los muestra en consola
{
  Mat &currentImage = *(Mat*)param;
  int Q = currentImage.at<Vec3b>(y,x)[2];
  int I = currentImage.at<Vec3b>(y,x)[1];
  int Y = currentImage.at<Vec3b>(y,x)[0];
  switch (event)
  {
      case CV_EVENT_LBUTTONDOWN:
          cout << "  Mouse X, Y: " << x << ", " << y << "   ";
          cout << "Y: " << Y<< " I: " << I<< " Q: " << Q<< endl;
          YIQ[0] = Y;
          YIQ[1] = I;
          YIQ[2] = Q;
          break;
  }
}

Mat display2(const Mat &mat_1, const Mat &mat_2)
//Une dos imagenes del mismo tamaño horizontalmente
{
  Mat win_mat(mat_1.rows,mat_1.cols * 2,mat_1.type());

  mat_1.copyTo(win_mat(Rect(  0, 0, mat_1.cols, mat_1.rows)));
  mat_2.copyTo(win_mat(Rect(mat_1.cols, 0, mat_1.cols, mat_1.rows)));

  return win_mat;
}

Mat display2v(const Mat &mat_1, const Mat &mat_2)
//Une dos imagenes del mismo tamaño verticalmente
{
  Mat win_mat(mat_1.rows * 2,mat_1.cols,mat_1.type());

  mat_1.copyTo(win_mat(Rect(  0, 0, mat_1.cols, mat_1.rows)));
  mat_2.copyTo(win_mat(Rect(0, mat_1.rows, mat_1.cols, mat_1.rows)));

  return win_mat;
}
