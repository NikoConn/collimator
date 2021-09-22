#ifndef REMOTETELESCOPE_H
#define REMOTETELESCOPE_H

#include "CollimatorLib_global.h"
#include <libindi/basedevice.h>
#include <QString>

class COLLIMATORLIB_EXPORT RemoteTelescope
{
public:
    RemoteTelescope();
    RemoteTelescope(INDI::BaseDevice* device, INDI::BaseClient* client);
    RemoteTelescope(QString host, int port, QString deviceName);

    bool isConnected();
    bool isValid();

    int id();
    QString name();
    QString host();
    int port();
    double exposure();

    void setExposure(float exposure);
    void setId(int id);
    void setPort(int port);
    void setName(QString name);
    void setHost(QString host);

private:
    int m_id;
    INDI::BaseDevice *m_device;
    QString m_host;
    int m_port;
    double m_exposure;
    QString m_deviceName;
};

#endif // REMOTETELESCOPE_H
