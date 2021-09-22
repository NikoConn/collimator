#include "indiclient.h"

#include <QDateTime>

bool INDIClient::connectToServer(QString host, int port)
{
    QUrl qhost = QUrl(host);
    if(!qhost.isValid())
        throw std::invalid_argument("Invalid host");

    setServer(host.toUtf8().constData(), port);
    return connectServer();
}

bool INDIClient::isConnectedToServer()
{
    return isServerConnected();
}

void INDIClient::disconnect()
{
    disconnectServer();
}

void INDIClient::getImageFromTelescope(RemoteTelescope telescope)
{   
    INDI::BaseDevice* device = devices.value(telescope.name());
    const char* telescopeName = device->getDeviceName();

    setBLOBMode(B_ALSO, telescopeName, nullptr);

    INumberVectorProperty *ccd_exposure = nullptr;
    ccd_exposure = device->getNumber("CCD_EXPOSURE");
    if (ccd_exposure == nullptr)
    {
        IDLog("Error: unable to find CCD Simulator CCD_EXPOSURE property...\n");
        return;
    }
    ccd_exposure->np[0].value = telescope.exposure();
    sendNewNumber(ccd_exposure);
}
