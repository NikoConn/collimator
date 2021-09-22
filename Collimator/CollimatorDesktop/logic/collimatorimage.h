#ifndef COLLIMATORIMAGE_H
#define COLLIMATORIMAGE_H

#include "paintingproperties.h"
#include "telescopeui.h"
#include <QLabel>
#include <QMouseEvent>
#include <imageresults.h>
#include <qscrollarea.h>
#include <model/indiclient.h>
#include <model/remotetelescope.h>
#include <model/telescope.h>

class CollimatorImage : public QLabel
{
    Q_OBJECT
public:
    CollimatorImage(QWidget *parent = 0);
    ~CollimatorImage() {
        if(client.isConnectedToServer())
            client.disconnect();
    }


    /**
     * @brief setTelescope Sets the telescope which is going to be used for displaying properties and drawings
     * @param telescope
     */
    void setTelescope(TelescopeUI* telescope);


    /**
     * @brief setScrollArea Sets the scroll area container for better zoom
     * @param parentScrollArea Parent QScrollAreal
     */
    void setScrollArea(QScrollArea* parentScrollArea);


    /**
     * @brief setDebug If true shows window with debug image every refresh
     */
    void setDebug(bool debug);


    /**
     * @brief loadImage Loads image from path
     * @param path Path of the image to be loaded
     */
    void loadImage(QString path);


    /**
     * @brief connectTelescope Connects to the given RemoteTelescope and starts getting images
     * @param remoteTelescope
     */
    void connectTelescope(RemoteTelescope remoteTelescope);


    /**
     * @brief zoom Zoom on image
     * @param diff Float value of the zoom diff to do. If negative will unzoom
     */
    void zoom(float diff);


    /**
     * @brief refreshImage Reloads current image with its telescope properties
     */
    void refreshImage();

    /**
     * @brief setConnected Changes connection property. If false will stop receiving images
     * @param connected
     */
    void setConnected(bool connected);

private:
    struct Image {
        uchar* data;
        int width = -1;
        int height = -1;
        int step = -1;
        QImage::Format format;
    } image;
    bool dragging;

    QScrollArea* scrollArea;
    TelescopeUI* telescope;
    RemoteTelescope remoteTelescope;
    INDIClient client;
    bool showDebug;
    bool m_connected;

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

    void displayPixmap();

public slots:

    /**
     * @brief propertiesChanged Slot to be called when telescope properties are changed to refresh image
     */
    void propertiesChanged();
private slots:
    void telescopeDetected(RemoteTelescope);
    void imageReceived(QString);

signals:

    /**
     * @brief connectedChanged Signal emited when telescope is connected or disconnected
     */
    void connectedChanged(bool);
};

#endif // COLLIMATORIMAGE_H
