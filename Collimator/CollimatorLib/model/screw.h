#ifndef SCREW_H
#define SCREW_H

#include "CollimatorLib_global.h"
#include <QPoint>
#include <QHash>

class COLLIMATORLIB_EXPORT Screw
{
public:
    Screw();
    Screw(float degrees);

    virtual int id();
    virtual float degrees();

    void setId(int id);
    void setDegrees(float degrees);
    QPoint getScrewPosition(QPoint reference, int radius);

private:
    int m_id;
    float m_degrees;
};

inline bool operator==(const Screw &sc1, const Screw &sc2)
{
    Screw screw1 = (Screw) sc1;
    Screw screw2 = (Screw) sc2;
    return screw1.id() == screw2.id();
}

inline uint qHash(const Screw &key, uint seed)
{
    Screw screw = (Screw) key;
    int degrees = static_cast<int>(screw.degrees());
    return qHash(screw.id(), seed) ^ degrees;
}

#endif // SCREW_H
