#include "indiclient.h"
#include "remotetelescope.h"

RemoteTelescope::RemoteTelescope()
{
    m_id = -1;
    m_host = "";
    m_port = 0;
    m_deviceName = "";
    m_exposure = 1;
}


RemoteTelescope::RemoteTelescope(INDI::BaseDevice* device, INDI::BaseClient* client)
{
    m_id = -1;
    m_device = device;
    m_deviceName = device->getDeviceName();
    m_host = client->getHost();
    m_port = client->getPort();
    m_exposure = 1;
}


RemoteTelescope::RemoteTelescope(QString host, int port, QString deviceName)
{
    if(port < 0)
        throw std::invalid_argument("Port can't be negative");

    m_host = host;
    m_port = port;
    m_deviceName = deviceName;
    m_exposure = 1;
}


QString RemoteTelescope::name()
{
    return m_deviceName;
}


bool RemoteTelescope::isConnected()
{
    return m_device->isConnected();
}


bool RemoteTelescope::isValid() {
    QUrl urlHost(m_host);
    if(m_port < 0)
        return false;
    if(m_host.trimmed() == "" || !urlHost.isValid())
        return false;
    if(m_deviceName.trimmed() == "")
        return false;
    if(m_exposure < 0.1 || m_exposure > 60)
        return false;
    return true;
}


int RemoteTelescope::port()
{
    return m_port;
}


QString RemoteTelescope::host()
{
    return m_host;
}


double RemoteTelescope::exposure()
{
    return m_exposure;
}


void RemoteTelescope::setExposure(float exposure)
{
    if(exposure < 0.1 || exposure > 60)
        throw std::invalid_argument("Exposure must be < 0.1 and > 60");
    m_exposure = exposure;
}


void RemoteTelescope::setId(int id)
{
    m_id = id;
}


int RemoteTelescope::id()
{
    return m_id;
}


void RemoteTelescope::setPort(int port)
{
    if(port <= 0)
        throw std::invalid_argument("Invalid port");
    m_port = port;
}


void RemoteTelescope::setName(QString name)
{
    if(name.trimmed() == "")
        throw std::invalid_argument("Device name can't be empty");
    m_deviceName = name;
}


void RemoteTelescope::setHost(QString host)
{
    if(host.trimmed() == "")
        throw std::invalid_argument("Host can't be empty");
    m_host = host;
}
