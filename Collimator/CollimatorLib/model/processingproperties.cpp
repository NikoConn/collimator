#include "processingproperties.h"
#include "telescope.h"

ProcessingProperties::ProcessingProperties(ProcessingProperties* proccessingProperties)
{
    m_id = -1;
    m_scale = proccessingProperties->scale();
    m_refinateRadius = proccessingProperties->refinateRadius();
    m_stretchingValues = proccessingProperties->stretchingValues();
    m_thresholdValues = proccessingProperties->thresholdValues();
}

ProcessingProperties::ProcessingProperties()
{
    m_id = -1;
    m_stretchingValues.max = 100;
    m_stretchingValues.min = 50;
    m_thresholdValues.constant = -30;
    m_thresholdValues.thresholdSize = 25;

    m_scale = 1;
    m_refinateRadius = 30;
}

void ProcessingProperties::setId(int id)
{
    m_id = id;
}

void ProcessingProperties::setScale(float scale)
{
    if(scale < 0)
        throw std::invalid_argument("Scale must be > 0");

    m_scale = scale;
}

void ProcessingProperties::setRefinateRadius(float refinateRadius)
{
    m_refinateRadius = refinateRadius;
}

void ProcessingProperties::setStretchingValues(StretchingValues values)
{
    if(values.min > values.max)
        throw std::invalid_argument("Stretching min must be > max");

    m_stretchingValues = values;
}

/**
 * @brief ProcessingProperties::setThresholdValues
 * @param values
 * size: 3,5,7,9...
 * constant cualquiera, normalmente positivo
 */
void ProcessingProperties::setThresholdValues(ThresholdValues values)
{
    if(values.thresholdSize % 2 != 1)
        throw std::invalid_argument("Threshold Size must be odd");
    if(values.thresholdSize < 3)
        throw std::invalid_argument("Threshold Size must be > 1");

    m_thresholdValues = values;
}

int ProcessingProperties::id()
{
    return m_id;
}

float ProcessingProperties::scale()
{
    return m_scale;
}

float ProcessingProperties::refinateRadius()
{
    return m_refinateRadius;
}

ProcessingProperties::StretchingValues ProcessingProperties::stretchingValues()
{
    return m_stretchingValues;
}

ProcessingProperties::ThresholdValues ProcessingProperties::thresholdValues()
{
    return m_thresholdValues;
}
