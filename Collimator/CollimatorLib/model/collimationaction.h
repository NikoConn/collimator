#ifndef COLLIMATIONACTION_H
#define COLLIMATIONACTION_H

#include "screw.h"
#include "CollimatorLib_global.h"

class COLLIMATORLIB_EXPORT CollimationAction
{
public:
    enum Action {
        Loosen,
        Tighten
    };

    CollimationAction(Action, Screw);

    Action action();
    Screw screw();
private:
    Action m_action;
    Screw m_screw;
};

#endif // COLLIMATIONACTION_H
