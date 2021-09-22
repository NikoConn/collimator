#ifndef PAINTINGPROPERTIES_H
#define PAINTINGPROPERTIES_H

#include <imageresults.h>
#include <qimage.h>
#include <model/telescope.h>

/**
 * @brief The PaintingProperties class defines the properties for painting automatic and manual drawings of a collimation result
 */
class PaintingProperties
{
public:
    struct Mirroring {
        bool vertically;
        bool horizontally;
    };
    struct Scope {
        QPoint center;
        int radius;
    };

    PaintingProperties();

    void setId(int id);
    void setScrewsRadius(int screwsRadius);
    void setZoom(float zoom);
    void setAutomatic(bool automatic);
    void setViewBoundingBox(bool boundingBox);
    void setViewScrews(bool viewScrews);
    void setViewCalculatedCenter(bool viewCalculatedCenter);
    void setImageResults(ImageResults* imageResults);
    void setScope(Scope scope);
    void setImageMirroring(Mirroring imageMirroring);
    void setScrewsMirroring(Mirroring screwsMirroring);
    void setTigthMirroring(bool tightMirroring);

    int id();
    int screwsRadius();
    float zoom();
    bool automatic();
    bool viewBoundingBox();
    bool viewScrews();
    bool viewCalculatedCenter();
    bool tightMirroring();
    Scope scope();
    Mirroring imageMirroring();
    Mirroring screwsMirroring();

    /**
     * @brief paint Paints into image own properties of the telescope
     * @param image Image where to draw
     * @param telescope
     */
    void paint(QImage* image, Telescope telescope);
private:
    Scope m_scope;
    Mirroring m_imageMirroring;
    Mirroring m_screwsMirroring;

    int m_id;
    int m_screwsRadius;
    float m_zoom;
    bool m_automatic;
    bool m_viewBoundingBox;
    bool m_viewCalculatedCenter;
    bool m_viewScrews;
    bool m_tightMirroring;
    ImageResults* m_imageResults;

    void paintScrewsHelp(Telescope telescope, QPoint center, QPainter* painter);

};

#endif // PAINTINGPROPERTIES_H
