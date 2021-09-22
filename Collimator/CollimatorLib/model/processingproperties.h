#ifndef PROCESSINGPROPERTIES_H
#define PROCESSINGPROPERTIES_H

#include "CollimatorLib_global.h"

class COLLIMATORLIB_EXPORT ProcessingProperties
{
public:
    explicit ProcessingProperties(ProcessingProperties* proccessingProperties);
    explicit ProcessingProperties();

    typedef struct
    {
        float min;
        float max;
    } StretchingValues;
    typedef struct
    {
        int thresholdSize;
        double constant;
    } ThresholdValues;

    int id();
    float scale();
    float refinateRadius();
    StretchingValues stretchingValues();
    ThresholdValues thresholdValues();

    void setId(int id);
    void setScale(float scale);
    void setRefinateRadius(float refinateRadius);
    void setStretchingValues(StretchingValues values);
    void setThresholdValues(ThresholdValues values);

private:
    int m_id;
    float m_scale;
    float m_refinateRadius;
    StretchingValues m_stretchingValues;
    ThresholdValues m_thresholdValues;
};

#endif // PROCESSINGPROPERTIES_H
