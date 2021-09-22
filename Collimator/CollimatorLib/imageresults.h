#ifndef COLLIMATION_H
#define COLLIMATION_H

#include "CollimatorLib_global.h"
#include <model/collimationaction.h>
#include <model/processingproperties.h>
#include <model/telescope.h>

class COLLIMATORLIB_EXPORT ImageResults
{
public:
    struct Point{
        float x, y;
    };
    struct Rect{
        float x, y;
        float width, heigth;
    };

    explicit ImageResults(Point center, Rect boundingBox, ProcessingProperties processingProperties);

    Point center();
    ProcessingProperties processingProperties();
    Rect boundingBox();
    CollimationAction getAction(Telescope telescope);
private:
    Point m_center;
    ProcessingProperties m_properties;
    Rect m_boundingBox;
};

#endif // COLLIMATION_H
