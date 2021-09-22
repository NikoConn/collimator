#include "imageresults.h"

#include <QHash>
#include <QLine>
#include <QRect>
#include <QDebug>

ImageResults::ImageResults(Point center, Rect boundingBox, ProcessingProperties processingProperties)
{
    m_center = center;
    m_properties = processingProperties;
    m_boundingBox = boundingBox;
}

ProcessingProperties ImageResults::processingProperties()
{
    return m_properties;
}

ImageResults::Rect ImageResults::boundingBox()
{
    return m_boundingBox;
}

ImageResults::Point ImageResults::center()
{
    return m_center;
}

CollimationAction ImageResults::getAction(Telescope telescope)
{
    QRect rect(m_boundingBox.x, m_boundingBox.y, m_boundingBox.width, m_boundingBox.heigth);
    QPoint boundingBoxCenter = rect.center();
    QPoint calculatedCenter(m_center.x, m_center.y);

    QPair<Screw, int> minDistance;
    QHash<Screw, QPoint> screwPositions;
    minDistance.second = -1;

    for(Screw screw : telescope.screws()) {
        //Creamos linea imaginaria con tornillo y centro
        QPoint virtualScrewPosition = screw.getScrewPosition(boundingBoxCenter, 30);
        QLineF line(virtualScrewPosition, boundingBoxCenter);
        screwPositions.value(screw, virtualScrewPosition);

        //Calculamos distancia minima entre centro calculado y linea
        QPointF result;
        QLineF perpendicLine(virtualScrewPosition, QPointF(virtualScrewPosition.x(), 0.0));
        perpendicLine.setAngle(90.0 + line.angle());
        line.intersect(perpendicLine, &result);
        int distance = abs(QLineF(result, calculatedCenter).length());

        //Si la distancia es menor que la almacenada, la guardamos;
        if(distance < minDistance.second || minDistance.second == -1) {
            minDistance.second = distance;
            minDistance.first = screw;
        }
    }

    QPoint screwVirtualPosition = screwPositions.value(minDistance.first);
    QLineF centerToScrew(boundingBoxCenter, screwVirtualPosition);
    QLineF calculatedToScrew(calculatedCenter, screwVirtualPosition);

    if(centerToScrew.length() < calculatedToScrew.length())
        return CollimationAction(CollimationAction::Tighten, minDistance.first);
    else
        return CollimationAction(CollimationAction::Loosen, minDistance.first);
}
