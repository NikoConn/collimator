#include "telescopeui.h"

TelescopeUI::TelescopeUI(TelescopeUI* telescope) : Telescope(telescope)
{
    m_paintingProperties = telescope->paintingProperties();
}

TelescopeUI::TelescopeUI() : Telescope()
{
    m_paintingProperties = new PaintingProperties();
}

void TelescopeUI::setPaintingProperties(PaintingProperties paintingProperties)
{
    *this->m_paintingProperties = paintingProperties;
}

PaintingProperties* TelescopeUI::paintingProperties()
{
    return m_paintingProperties;
}
