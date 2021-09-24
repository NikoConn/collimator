#include "collimatorimage.h"

#include <collimator.h>
#include <imageresults.h>
#include <qpainter.h>
#include <qscrollbar.h>
#include <QtMath>
#include <QDebug>
#include <qpixmap.h>
#include <QTimer>

#include <model/indiclient.h>

CollimatorImage::CollimatorImage(QWidget *parent) : QLabel(parent)
{
    telescope = new TelescopeUI();
    this->setScaledContents(true);
    this->dragging = false;
    this->setText(QString("No Image"));
    this->showDebug = false;
    this->m_connected = false;
}

void CollimatorImage::setTelescope(TelescopeUI* telescope)
{
    this->telescope = telescope;
}


void CollimatorImage::setScrollArea(QScrollArea* scrollArea)
{
    this->scrollArea = scrollArea;
}


void CollimatorImage::loadImage(QString path)
{
    if(path.split(".").last() == "fits")
    {
        Mat imageMat = Collimator::loadFitsImage(path.toLocal8Bit().data());

        image.data = new uchar[imageMat.total()];
        std::memcpy(image.data, imageMat.data, imageMat.total());
        image.width = imageMat.cols;
        image.height = imageMat.rows;
        image.step = imageMat.step;
        image.format = QImage::Format_Indexed8;
    }
    else
    {
        QImage qImage = QImage(path);
        image.data = new uchar[qImage.sizeInBytes()];
        std::memcpy(image.data, qImage.bits(), qImage.sizeInBytes());
        image.width = qImage.width();
        image.height = qImage.height();
        image.step = qImage.bytesPerLine();
        image.format = qImage.format();
    }
    refreshImage();
}

void CollimatorImage::connectTelescope(RemoteTelescope telescope)
{
    remoteTelescope = telescope;
    connect(&client, &INDIClient::deviceDetected, this, &CollimatorImage::telescopeDetected);
    client.connectToServer(telescope.host(), telescope.port());
}

void CollimatorImage::zoom(float diff)
{
    if(telescope->paintingProperties()->zoom() + diff <= 0)
        return;
    if(this->image.data == nullptr)
        return;

    QScrollBar* horizontal = scrollArea->horizontalScrollBar();
    QScrollBar* vertical = scrollArea->verticalScrollBar();
    float zoom = telescope->paintingProperties()->zoom() + diff;
    telescope->paintingProperties()->setZoom(zoom);

    horizontal->setValue(horizontal->value()/(zoom-diff)*zoom + horizontal->singleStep());
    vertical->setValue(vertical->value()/(zoom-diff)*zoom + vertical->singleStep());

    refreshImage();
}

float calculateScrewsDegrees(Screw screw, PaintingProperties paintingProperties)
{
    float degrees = screw.degrees();
    if(paintingProperties.screwsMirroring().horizontally)
    {
        int cuadrant = degrees / 90;
        if((int) degrees % 90 == 0 && (cuadrant == 0 || cuadrant == 2))
            cuadrant = (cuadrant + 4 - 1) % 4;

        switch(cuadrant)
        {
            case 0:
                degrees += (90 - degrees) * 2;
                break;
            case 1:
                degrees -= (degrees - 90) * 2;
                break;
            case 2:
                degrees += (degrees - 270 * 90) * 2;
                break;
            case 3:
                degrees -= (270 - degrees) * 2;
                break;
        }
    }

    if(paintingProperties.screwsMirroring().vertically)
    {
        int cuadrant = degrees / 90;

        switch(cuadrant)
        {
            case 0:
                degrees = (int (degrees - degrees * 2) + 360) % 360;
                break;
            case 1:
                degrees += (180 - degrees) * 2;
                break;
            case 2:
                degrees -= (degrees - 180) * 2;
                break;
            case 3:
                degrees = (int (degrees + (360 - degrees) * 2)) % 360;
        }
    }
    return degrees;
}

void CollimatorImage::refreshImage()
{
    QImage qImage = QImage(image.data, image.width, image.height, image.step, image.format);
    qImage.toPixelFormat(QImage::Format_Indexed8);
    qImage = qImage.mirrored(telescope->paintingProperties()->imageMirroring().horizontally,
                             telescope->paintingProperties()->imageMirroring().vertically);

    TelescopeUI telescope(this->telescope);
    QList<Screw> screws;
    for(Screw screw : telescope.screws())
        screws.append(Screw(calculateScrewsDegrees(screw.degrees(), *telescope.paintingProperties())));
    telescope.setScrews(screws);

    if(telescope.paintingProperties()->automatic())
    {
        ProcessingProperties pp = telescope.processingProperties();
        Collimator collimator(&pp);
        ImageResults* imageResult = collimator.proccessImage(qImage, showDebug);
        telescope.paintingProperties()->setImageResults(imageResult);
    }

    QImage rgbImage = qImage.convertToFormat(QImage::Format_RGB16);
    telescope.paintingProperties()->paint(&rgbImage, telescope);

    QSize size(rgbImage.size() * telescope.paintingProperties()->zoom());
    QPixmap pixmap = QPixmap::fromImage(rgbImage);
    setPixmap(pixmap);
    resize(size);
}


void CollimatorImage::propertiesChanged()
{
    if(this->image.width > 0)
        refreshImage();
}

void CollimatorImage::mousePressEvent(QMouseEvent* event){
    QPoint pressedPoint = event->pos() / telescope->paintingProperties()->zoom();
    if(!telescope->paintingProperties()->automatic())
    {
        PaintingProperties::Scope scope = telescope->paintingProperties()->scope();
        scope.center = pressedPoint;
        telescope->paintingProperties()->setScope(scope);
        dragging = true;
    }
}


void CollimatorImage::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint releasePoint = event->pos() / telescope->paintingProperties()->zoom();
    if(!telescope->paintingProperties()->automatic() && dragging)
    {
        PaintingProperties::Scope scope = telescope->paintingProperties()->scope();
        QPoint center = scope.center;
        QLineF lineBetween(center, releasePoint);
        int distance = lineBetween.length();

        scope.radius = distance;
        telescope->paintingProperties()->setScope(scope);
        dragging = false;
        refreshImage();
    }
}

void CollimatorImage::setDebug(bool debug)
{
    this->showDebug = debug;
}


void CollimatorImage::telescopeDetected(RemoteTelescope detectedTelescope)
{
    if(detectedTelescope.name() == remoteTelescope.name()) {
        connect(&client, &INDIClient::imageReceived, this, &CollimatorImage::imageReceived);
        client.getImageFromTelescope(remoteTelescope);
        m_connected = true;
        emit connectedChanged(true);
    }
}


void CollimatorImage::imageReceived(QString filePath)
{
    loadImage(filePath);
    if(m_connected)
        client.getImageFromTelescope(remoteTelescope);
}


void CollimatorImage::setConnected(bool connected)
{
    m_connected = connected;
    if(!connected)
        client.disconnect();
    emit connectedChanged(m_connected);
}
