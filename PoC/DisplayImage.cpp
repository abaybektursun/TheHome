#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

CvMoments moments1,cenmoments1;
           double M00, M01, M10;

           cvMoments(largest_contour,&moments1);
           M00 = cvGetSpatialMoment(&moments1,0,0);
           M10 = cvGetSpatialMoment(&moments1,1,0);
           M01 = cvGetSpatialMoment(&moments1,0,1);
           posX_Yellow = (int)(M10/M00);
           posY_Yellow = (int)(M01/M00);

          double theta = 0.5 * atan(
                    (2 * cvGetCentralMoment(&moments1, 1, 1)) /
                    (cvGetCentralMoment(&moments1, 2, 0) -  cvGetCentralMoment(&moments1, 0, 2)));
                theta = (theta / PI) * 180;

                // fit an ellipse (and draw it)

                if (largest_contour->total >= 6) // can only do an ellipse fit
                                                 // if we have > 6 points
                {
                    CvBox2D box = cvFitEllipse2(largest_contour);
                    if ((box.size.width < imgYellowThresh->width) &&  (box.size.height < imgYellowThresh->height))
                    {

                        cvEllipseBox(imgYellowThresh, box, CV_RGB(255, 255 ,255), 3, 8, 0 );
                    }
                }

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}
