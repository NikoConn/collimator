#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "CollimatorLib_global.h"
#include "screw.h"
#include "processingproperties.h"
#include "remotetelescope.h"
#include <QObject>

class COLLIMATORLIB_EXPORT Telescope
{
public:
    explicit Telescope(Telescope* telescope);
    explicit Telescope();

    int id();
    QString name();
    ProcessingProperties processingProperties();
    RemoteTelescope remoteTelescope();
    QList<Screw> screws();

    void setId(int id);
    void setName(QString name);
    void setProcessingProperties(ProcessingProperties processingproperties);
    void setScrews(QList<Screw> screws);
    void setRemoteTelescope(RemoteTelescope remoteTelescope);

private:
    int m_id;
    QString m_name;
    ProcessingProperties m_processingProperties;
    QList<Screw> m_screws;
    RemoteTelescope m_remoteTelescope;
};

#endif // TELESCOPE_H
