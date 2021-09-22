#include "paintingproperties.h"
#include <model/telescope.h>
#include <qpainter.h>
#include <QtMath>
#include <QDebug>

PaintingProperties::PaintingProperties()
{
    m_zoom = 1.0;
    m_viewBoundingBox = false;
    m_viewCalculatedCenter = false;
    m_viewScrews = false;
    m_automatic = false;
    m_scope.radius = -1;
    m_screwsRadius = 30;

    Mirroring mirroring;
    mirroring.horizontally = false;
    mirroring.vertically = false;

    m_imageMirroring = mirroring;
    m_screwsMirroring = mirroring;

    m_tightMirroring = false;
}


void PaintingProperties::setId(int id)
{
    m_id = id;
}


void PaintingProperties::setScrewsRadius(int screwsRadius)
{
    m_screwsRadius = screwsRadius;
}


void PaintingProperties::setZoom(float zoom)
{
    m_zoom = zoom;
}


void PaintingProperties::setAutomatic(bool automatic)
{
    m_automatic = automatic;
}


void PaintingProperties::setViewBoundingBox(bool viewBoundingBox)
{
    m_viewBoundingBox = viewBoundingBox;
}


void PaintingProperties::setViewScrews(bool viewScrews)
{
    m_viewScrews = viewScrews;
}


void PaintingProperties::setViewCalculatedCenter(bool viewCalculatedCenter)
{
    m_viewCalculatedCenter = viewCalculatedCenter;
}


void PaintingProperties::setImageResults(ImageResults* imageResults)
{
    m_imageResults = imageResults;
}


void PaintingProperties::setScope(Scope scope)
{
    m_scope = scope;
}


void PaintingProperties::setImageMirroring(Mirroring imageMirroring)
{
    m_imageMirroring = imageMirroring;
}


void PaintingProperties::setScrewsMirroring(Mirroring screwsMirroring)
{
    m_screwsMirroring = screwsMirroring;
}


void PaintingProperties::setTigthMirroring(bool tightMirroring)
{
    m_tightMirroring = tightMirroring;
}


int PaintingProperties::id()
{
    return m_id;
}

int PaintingProperties::screwsRadius()
{
    return m_screwsRadius;
}

float PaintingProperties::zoom()
{
    return m_zoom;
}


bool PaintingProperties::automatic()
{
    return m_automatic;
}


bool PaintingProperties::viewBoundingBox()
{
    return m_viewBoundingBox;
}


bool PaintingProperties::viewScrews()
{
    return m_viewScrews;
}


bool PaintingProperties::viewCalculatedCenter()
{
    return m_viewCalculatedCenter;
}


PaintingProperties::Scope PaintingProperties::scope()
{
    return m_scope;
}


PaintingProperties::Mirroring PaintingProperties::imageMirroring()
{
    return m_imageMirroring;
}


bool PaintingProperties::tightMirroring()
{
    return m_tightMirroring;
}


PaintingProperties::Mirroring PaintingProperties::screwsMirroring()
{
    return m_screwsMirroring;
}


void rotatePoint(QPoint* point, QPoint center, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    point->setX(point->x() - center.x());
    point->setY(point->y() - center.y());

    float x = point->x() * c - point->y() * s;
    float y = point->x() * s + point->y() * c;

    point->setX(x + center.x());
    point->setY(y + center.y());
}

void PaintingProperties::paintScrewsHelp(Telescope telescope, QPoint center, QPainter* painter)
{
    CollimationAction collimationAction = m_imageResults->getAction(telescope);
    float degrees = collimationAction.screw().degrees();
    CollimationAction::Action action = collimationAction.action();

    QPoint screwPoint = collimationAction.screw().getScrewPosition(center, this->m_screwsRadius);

    if(this->tightMirroring())
    {
        if(collimationAction.action() == CollimationAction::Tighten)
            action = CollimationAction::Loosen;
        else
            action = CollimationAction::Tighten;
    }

    QPoint ampliation(10, 10);
    screwPoint.setX(screwPoint.x());
    screwPoint.setY(screwPoint.y());
    QRect screwContainer(screwPoint + ampliation, screwPoint - ampliation);

    QPoint leftArrow(screwPoint.x() + 4, screwPoint.y());
    QPoint topArrow(screwPoint.x(), screwPoint.y() - 4);
    QPoint bottomArrow(screwPoint.x(), screwPoint.y() + 4);

    //ROTACION SOBRE UN EJE ES SENTIDO ANTIHORARIO
    float rotation;
    if(action == CollimationAction::Tighten)
        rotation = -degrees + 135;
    else
        rotation = -degrees - 135;
    float rotationInRadians = qDegreesToRadians(rotation);

    rotatePoint(&leftArrow, screwPoint, rotationInRadians);
    rotatePoint(&topArrow, screwPoint, rotationInRadians);
    rotatePoint(&bottomArrow, screwPoint, rotationInRadians);

    if(action == CollimationAction::Tighten)
        rotation = degrees - 45;
    else
        rotation = degrees + 45;
    rotationInRadians = qDegreesToRadians(rotation);

    QPoint shift(cos(rotationInRadians) * ampliation.x(), sin(rotationInRadians) * -ampliation.y());
    leftArrow += shift;
    topArrow += shift;
    bottomArrow += shift;

    QLine lineArrowLeft(leftArrow, topArrow);
    QLine lineArrowRight(leftArrow, bottomArrow);

    painter->setPen(QColor(111, 36, 240));
    painter->drawArc(screwContainer, (degrees + 45) * 16, 90 * -16);
    painter->drawLine(lineArrowLeft);
    painter->drawLine(lineArrowRight);
}


void PaintingProperties::paint(QImage* image, Telescope telescope)
{
    QPoint center(image->width()/2, image->height()/2);
    QPainter painter(image);

    if(this->m_automatic)
    {
        ImageResults::Rect boundingBox = m_imageResults->boundingBox();
        QRect rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.heigth);
        ImageResults::Point calculatedCenter = m_imageResults->center();
        center = rect.center();
        //center = QPoint(calculatedCenter.x, calculatedCenter.y);

        if(this->m_viewBoundingBox)
        {
            painter.setPen(Qt::red);
            painter.drawRect(rect);
            painter.drawEllipse(center, 3, 3);
        }
        if(this->m_viewCalculatedCenter)
        {
            painter.setPen(Qt::green);
            painter.drawEllipse(QPoint(calculatedCenter.x, calculatedCenter.y), 3, 3);
        }
    }
    else if(m_scope.radius != -1)
    {
        center = m_scope.center;

        painter.setPen(Qt::green);
        int radius = m_scope.radius;
        painter.drawEllipse(m_scope.center, radius, radius);
        painter.drawEllipse(m_scope.center, 3, 3);
    }

    if(this->m_viewScrews && telescope.screws().length() > 0)
    {
        painter.setPen(Qt::blue);
        QList<Screw> screws = telescope.screws();
        for(Screw screw : screws) {
            QPoint screwPosition = screw.getScrewPosition(center, this->m_screwsRadius);
            int screwY = screwPosition.y();
            int screwX = screwPosition.x();

            painter.drawEllipse(QPoint(screwX, screwY), 3, 3);
        }


        if(this->m_automatic)
            paintScrewsHelp(telescope, center, &painter);
    }
}
