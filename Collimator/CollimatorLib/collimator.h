#ifndef COLLIMATOR_H
#define COLLIMATOR_H

#include "CollimatorLib_global.h"
#include "model/processingproperties.h"
#include <imageresults.h>
#include <opencv2/core.hpp>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>

using namespace cv;

class COLLIMATORLIB_EXPORT Collimator
{
public:
    Collimator(ProcessingProperties* propiertes);

    Mat original;
    Mat scaled;
    Mat binary;

    static Mat loadFitsImage(char* filename);
    ImageResults* proccessImage(QImage image, bool showdebug = false);

    static void strench(Mat* img, int vmin, int vmax);
    static void massCenter(Mat* img, float *cx, float *cy);
    static Rect boundingBox(Mat* img, int cx, int cy, int radius);
private:
    ProcessingProperties* m_propiertes;
};

#endif // COLLIMATOR_H
