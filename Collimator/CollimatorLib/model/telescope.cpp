#include "telescope.h"


Telescope::Telescope(Telescope* telescope)
{
    m_id = telescope->id();
    m_name = telescope->name();
    m_processingProperties = telescope->processingProperties();
    m_screws = telescope->screws();
}

Telescope::Telescope()
{
    m_id = -1;
    m_name = QString("");
    m_processingProperties = ProcessingProperties();
    m_screws = QList<Screw>();

    m_screws.append(Screw(45));
    m_screws.append(Screw(135));
    m_screws.append(Screw(270));
}

void Telescope::setId(int id)
{
    m_id = id;
}

void Telescope::setName(QString name)
{
    if(name.length() < 1)
        throw std::invalid_argument("Empty name");
    m_name = name;
}

void Telescope::setProcessingProperties(ProcessingProperties processingProperties)
{
    m_processingProperties = processingProperties;
}

void Telescope::setScrews(QList<Screw> screws)
{
    if(screws.length() < 2)
        throw std::invalid_argument("Telescope must have two or more screws");

    /*QHash<int, bool> quadrants;
    for(Screw screw: screws)
        quadrants.insert(screw.degrees()/90, true);

    if(quadrants.size() < 2)
        throw  std::invalid_argument("Screws must be placed in more than one quadrant");*/

    m_screws = screws;
}

int Telescope::id()
{
    return m_id;
}

QString Telescope::name()
{
    return m_name;
}


ProcessingProperties Telescope::processingProperties()
{
    return m_processingProperties;
}

QList<Screw> Telescope::screws()
{
    return m_screws;
}

RemoteTelescope Telescope::remoteTelescope()
{
    return m_remoteTelescope;
}

void Telescope::setRemoteTelescope(RemoteTelescope remoteTelescope)
{
    m_remoteTelescope = remoteTelescope;
}
