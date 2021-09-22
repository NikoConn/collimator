#include "collimator.h"

#include <QStringList>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>
#include <omp.h>

#include <fitsio.h>
#include <qdebug.h>

using namespace cv;
using namespace std;

Collimator::Collimator(ProcessingProperties *properties)
{
    m_propiertes = properties;
}

Mat Collimator::loadFitsImage(char *filename)
{
    fitsfile *fitsFile;
    long fpixel[2]={1,1};
    int status = 0;
    int dim;
    long sizedim[3];
    Mat img;
    int pixelsize;

    fits_open_file(&fitsFile, filename, READONLY, &status);
    fits_get_img_param(fitsFile, 3, &pixelsize, &dim, sizedim, &status);
    if(status)
        qDebug() << "error";
    if (dim == 2)
    {
        int cv_pixelsize, fits_datatype;
        switch (pixelsize) {
        case 8:
            cv_pixelsize = CV_8U;
            fits_datatype = TBYTE;
            break;
        case 16:
            cv_pixelsize = CV_16U;
            fits_datatype = TUSHORT;
            break;
        default:
            qDebug() << "No suitable pixelsize";
            return Mat();
        }

        Size size(sizedim[0],sizedim[1]);
        Mat imgAux = Mat(size, cv_pixelsize);

        fits_read_pix(fitsFile, fits_datatype, fpixel, sizedim[0]*sizedim[1], NULL, imgAux.data, NULL, &status);
        fits_close_file(fitsFile, &status);

        if(cv_pixelsize != CV_8U)
            imgAux.convertTo(img, CV_8U, 1/257.0);
        else
            imgAux.copyTo(img);
    }
    return img;
}

/**
 * @brief strench Contrast stretching
 * @param img imagen
 * @param vmin
 * @param vmax
 */
void Collimator::strench(Mat* img, int vmin, int vmax)
{
    int i;
    #pragma omp parallel for private(i)
    for(i = 0; i < img->cols*img->rows; i++)
    {
        uchar *v = &(img->ptr<uchar>(i%img->rows))[i/img->rows];

        if(*v < vmin)
            *v = 0;
        else if(*v > vmax)
            *v = 255;
        else
            *v = 255*(*v - vmin)/(vmax-vmin)+0.5;

    }
}

/**
 * @brief massCenter Calcula el centro de masa a partir de la escala de grises; cuando es blanco, suma el valor para mas tarde dividirlo
 * @param img
 * @param cx
 * @param cy
 */
void Collimator::massCenter(Mat *img, float *cx, float *cy){
    Size size = img->size();
    int width, heigth, startx, starty;
    float ncx = 0;
    float ncy = 0;
    int count = 0;
    width = size.width;
    heigth = size.height;
    startx = 0;
    starty = 0;

    int i;
    #pragma omp parallel for reduction(+:ncx,ncy,count) private(i)
    for(i = starty; i < heigth*width; i++)
    {
        uchar* gray = &(img->ptr<uchar>(i/width))[i%width];
        if(*gray == 255)
        {
            ncx += i%width;
            ncy += i/width;
            count++;
        }

    }

    if(count > 0)
    {
        *cx = ncx / count;
        *cy = ncy / count;
    }
}


Rect Collimator::boundingBox(Mat *img, int cx, int cy, int radius)
{
    Size size = img->size();
    int x1 = 0;
    int x0 = size.width;
    int y1 = 0;
    int y0 = size.height;

    int heigth = cy-radius > 0 ? cy-radius : 0;
    int width = cx-radius > 0 ? cx-radius : 0;
    int startingWidth = width;

    int finalHeight = cy+radius < size.height ? cy+radius : size.height;
    int finalWidth = cx+radius < size.width ? cx+radius: size.width;

    while(heigth < finalHeight)
    {
        while(width < finalWidth)
        {
            int gray = img->ptr(heigth)[width];
            if(gray == 255)
            {
                if(width < x0)
                    x0 = width;
                if(width > x1)
                    x1 = width;
                if(heigth < y0)
                    y0 = heigth;
                if(heigth > y1)
                    y1 = heigth;
            }
            width++;
        }
        width = startingWidth;
        heigth++;
    }
    return Rect(x0, y0, x1-x0, y1-y0);
}

ImageResults* Collimator::proccessImage(QImage image, bool showDebug)
{
    float scale = m_propiertes->scale();
    ProcessingProperties::StretchingValues strechingValues = m_propiertes->stretchingValues();
    ProcessingProperties::ThresholdValues thresholdValues = m_propiertes->thresholdValues();
    float refinateRadius = m_propiertes->refinateRadius();
    float cx,cy;

    original = Mat(image.height(), image.width(), CV_8U, image.bits());

    Size originalSize = original.size();

    //escalado
    Size scaledSize(originalSize.width*scale, originalSize.height*scale);
    scaled = Mat(scaledSize, CV_8U);
    resize(original, scaled, scaledSize, INTER_LANCZOS4, 1);

    //binario
    strench(&scaled, strechingValues.min, strechingValues.max);
    binary = Mat(scaled.size(), CV_8U, 1);
    adaptiveThreshold(scaled, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,
                        thresholdValues.thresholdSize, thresholdValues.constant);

    //se obtiene el centro del circulo
    massCenter(&binary, &cx, &cy);
    Rect box = boundingBox(&binary, cx, cy, refinateRadius);
    binary = binary(box);
    cx -= box.x;
    cy -= box.y;
    massCenter(&binary, &cx, &cy);
    cx += box.x;
    cy += box.y;

    if(showDebug)
        imshow("debug", binary);

    ImageResults::Point calculatedCenter;
    calculatedCenter.x = cx;
    calculatedCenter.y = cy;

    ImageResults::Rect boundingBox;
    boundingBox.x = box.x;
    boundingBox.y = box.y;
    boundingBox.heigth = box.height;
    boundingBox.width = box.width;

    ImageResults* result = new ImageResults(calculatedCenter, boundingBox, *m_propiertes);
    return result;
}
