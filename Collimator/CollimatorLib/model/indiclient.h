#ifndef INDICLIENT_H
#define INDICLIENT_H

#include "CollimatorLib_global.h"
#include "remotetelescope.h"
#include <qurl.h>
#include <libindi/baseclient.h>
#include <qdebug.h>
#include <QtGui/qpixmap.h>
#include <QDir>
#include <fstream>
#include <iostream>

class COLLIMATORLIB_EXPORT INDIClient : public QObject, INDI::BaseClient
{
    Q_OBJECT
public:
    INDIClient() {};
    ~INDIClient() { disconnect(); };

    bool connectToServer(QString host, int port);
    bool isConnectedToServer();
    void disconnect();
    void getImageFromTelescope(RemoteTelescope telescope);

signals:
    void deviceDetected(RemoteTelescope);
    void imageReceived(QString);

private:
//metodos necesarios por heredar de BaseClient, por si quisieran ser usados :)
    void newDevice(INDI::BaseDevice* /*dp*/) override {};
    void removeDevice(INDI::BaseDevice* /**dp*/) override {};
    void newProperty(INDI::Property* property) override  {
        if(QString::fromStdString(property->getName()) == "CCD_EXPOSURE") {
            INDI::BaseDevice* device = property->getBaseDevice();
            devices.insert(device->getDeviceName(), device);
            emit deviceDetected(RemoteTelescope(device, this));
        }
    };
    void removeProperty(INDI::Property* /**property*/) override {};
    void newBLOB(IBLOB *bp) override {
        QString file(QDir::temp().path() + "/collimator.fits");
        std::ofstream myfile("");
        myfile.open(file.toStdString(), std::ios::out | std::ios::binary);
        myfile.write(static_cast<char *>(bp->blob), bp->bloblen);
        myfile.close();
        emit imageReceived(file);
    };
    void newSwitch(ISwitchVectorProperty* /**svp*/) override {};
    void newNumber(INumberVectorProperty* /**nvp*/) override {};
    void newText(ITextVectorProperty* /**tvp*/) override {};
    void newLight(ILightVectorProperty* /**lvp*/) override {};
    void newMessage(INDI::BaseDevice* /**dp*/, int /*messageID*/) override {};
    void serverConnected() override { qDebug() << "Connected!"; };
    void serverDisconnected(int /*exit_code*/) override {};

protected:
    void newUniversalMessage(std::string /*message*/) override {};

QHash<QString, INDI::BaseDevice*> devices;
};

#endif // INDICLIENT_H
