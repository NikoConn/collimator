#ifndef TELESCOPEUI_H
#define TELESCOPEUI_H

#include "paintingproperties.h"
#include <model/telescope.h>


/**
 * @brief The TelescopeUI class Child of Telescope for implementing PaintingProperties into telescope, necessary for drawing in UI
 */
class TelescopeUI : public Telescope
{
public:
    TelescopeUI(TelescopeUI*);
    TelescopeUI();

    /**
     * @return Pointer to the processing properties of the telescope
     */
    PaintingProperties* paintingProperties();


    /**
     * @brief setPaintingProperties Sets the given PaintingProperties to the telescope
     * @param paintingProperties
     */
    void setPaintingProperties(PaintingProperties paintingProperties);
private:
    PaintingProperties* m_paintingProperties;
};

#endif // TELESCOPEUI_H
