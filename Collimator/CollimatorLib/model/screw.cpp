#include "screw.h"
#include <QtMath>

Screw::Screw()
{
    m_id = -1;
}

Screw::Screw(float degrees)
{
    m_id = -1;
    setDegrees(degrees);
}

void Screw::setId(int id)
{
    m_id = id;
}

void Screw::setDegrees(float degrees)
{
    if(degrees > 360 || degrees < 0)
        throw new std::invalid_argument("Degrees must be between 0 and 360");

    m_degrees = degrees;
}

float Screw::degrees()
{
    return m_degrees;
}


int Screw::id()
{
    return m_id;
}

QPoint Screw::getScrewPosition(QPoint reference, int radius)
{
    float degrees = qDegreesToRadians(this->degrees());
    int x = reference.x() + (cos(degrees) * radius);
    int y = reference.y() - (sin(degrees) * radius);
    return QPoint(x, y);
}
