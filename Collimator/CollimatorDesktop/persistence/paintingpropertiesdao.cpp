#include "paintingpropertiesdao.h"

int PaintingPropertiesDAO::save(PaintingProperties *paintingProperties)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("INSERT INTO painting_properties "
                    "(id, zoom, automatic, view_bounding_box, "
                    "view_calculated_center, scope_center_x, "
                    "scope_center_y, scope_radius, view_screws, "
                    "image_mirroring_vertically, image_mirroring_horizontally, "
                    "screws_mirroring_vertically, screws_mirroring_horizontally, "
                    "tight_mirroring, screws_radius) VALUES "
                    "(:id, :zoom, :automatic, :viewBoundingBox, "
                    ":viewCalculatedCenter, :scopeCenterX, "
                    ":scopeCenterY, :scopeRadius, :viewScrews, "
                    ":image_mirroring_vertically, :image_mirroring_horizontally, "
                    ":screws_mirroring_vertically, :screws_mirroring_horizontally, "
                    ":tight_mirroring, :screws_radius)");

    bindValues(&query, *paintingProperties);
    query.bindValue(":id", paintingProperties->id());

    query.exec();

    return query.next()? paintingProperties->id(): -1;
}

int PaintingPropertiesDAO::update(PaintingProperties paintingProperties)
{
    if(paintingProperties.id() == -1)
        return -1;

    QSqlQuery query(db.getDatabase());

    query.prepare("UPDATE painting_properties "
                "SET zoom = :zoom, automatic = :automatic, "
                "view_bounding_box = :viewBoundingBox, "
                "view_calculated_center = :viewCalculatedCenter, "
                "scope_center_x = :scopeCenterX, "
                "scope_center_y = :scopeCenterY, "
                "scope_radius = :scopeRadius, "
                "view_screws = :viewScrews, "
                "image_mirroring_vertically = :image_mirroring_vertically, "
                "image_mirroring_horizontally = :image_mirroring_horizontally, "
                "screws_mirroring_vertically = :screws_mirroring_vertically, "
                "screws_mirroring_horizontally = :screws_mirroring_horizontally, "
                "tight_mirroring = :tight_mirroring, screws_radius = :screws_radius "
                "WHERE id = :id");

    query.bindValue(":id", paintingProperties.id());
    bindValues(&query, paintingProperties);

    query.exec();
    return query.next() ? paintingProperties.id() : -1;
}


PaintingProperties PaintingPropertiesDAO::get(int id)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT * FROM painting_properties "
                    "WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.next())
        throw 20;

    return createObject(query);
}

QList<PaintingProperties> PaintingPropertiesDAO::getAll()
{
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT * FROM painting_properties");
    query.exec();

    QList<PaintingProperties> paintingProperties;
    while(query.next()) {
        paintingProperties.append(createObject(query));
    }
    return paintingProperties;
}


void PaintingPropertiesDAO::bindValues(QSqlQuery* query, PaintingProperties paintingProperties)
{
    query->bindValue(":zoom", paintingProperties.zoom());
    query->bindValue(":automatic", paintingProperties.automatic());
    query->bindValue(":viewBoundingBox", paintingProperties.viewBoundingBox());
    query->bindValue(":viewCalculatedCenter", paintingProperties.viewCalculatedCenter());
    query->bindValue(":scopeCenterX", paintingProperties.scope().center.x());
    query->bindValue(":scopeCenterY", paintingProperties.scope().center.y());
    query->bindValue(":scopeRadius", paintingProperties.scope().radius);
    query->bindValue(":viewScrews", paintingProperties.viewScrews());
    query->bindValue(":image_mirroring_vertically", paintingProperties.imageMirroring().vertically);
    query->bindValue(":image_mirroring_horizontally", paintingProperties.imageMirroring().horizontally);
    query->bindValue(":screws_mirroring_vertically", paintingProperties.screwsMirroring().vertically);
    query->bindValue(":screws_mirroring_horizontally", paintingProperties.screwsMirroring().horizontally);
    query->bindValue(":tight_mirroring", paintingProperties.tightMirroring());
    query->bindValue(":screws_radius", paintingProperties.screwsRadius());
}


PaintingProperties PaintingPropertiesDAO::createObject(QSqlQuery query)
{
    PaintingProperties paintingProperties;
    paintingProperties.setId(query.value("telescopeid").toInt());
    paintingProperties.setZoom(query.value("zoom").toFloat());
    paintingProperties.setAutomatic(query.value("automatic").toBool());
    paintingProperties.setViewBoundingBox(query.value("view_bounding_box").toBool());
    paintingProperties.setViewScrews(query.value("view_screws").toBool());
    paintingProperties.setViewCalculatedCenter(query.value("view_calculated_center").toBool());
    paintingProperties.setScrewsRadius(query.value("screws_radius").toBool());
    paintingProperties.setTigthMirroring(query.value("tight_mirroring").toBool());

    PaintingProperties::Scope scope;
    QPoint scopeCenter(query.value("scope_center_x").toInt(), query.value("scope_center_y").toInt());
    scope.center = scopeCenter;
    scope.radius = query.value("scope_radius").toInt();
    paintingProperties.setScope(scope);

    PaintingProperties::Mirroring imageMirroring;
    imageMirroring.horizontally = query.value("image_mirroring_horizontally").toBool();
    imageMirroring.vertically = query.value("image_mirroring_vertically").toBool();
    paintingProperties.setImageMirroring(imageMirroring);

    PaintingProperties::Mirroring screwsMirroring;
    screwsMirroring.vertically = query.value("screws_mirroring_vertically").toBool();
    screwsMirroring.horizontally = query.value("screws_mirroring_horizontally").toBool();
    paintingProperties.setScrewsMirroring(screwsMirroring);

    return paintingProperties;
}
