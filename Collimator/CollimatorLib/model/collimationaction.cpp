#include "collimationaction.h"

CollimationAction::CollimationAction(Action action, Screw screw)
{
    m_action = action;
    m_screw = screw;
}

CollimationAction::Action CollimationAction::action()
{
    return m_action;
}

Screw CollimationAction::screw()
{
    return m_screw;
}
