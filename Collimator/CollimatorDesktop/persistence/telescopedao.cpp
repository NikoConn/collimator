#include "paintingpropertiesdao.h"
#include "remotetelescopedao.h"
#include "telescopedao.h"


int TelescopeDAO::save(TelescopeUI* telescope)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("INSERT INTO telescope "
                "(name) "
                "VALUES (:name)");

    query.bindValue(":name", telescope->name());

    query.exec();

    query = QSqlQuery("SELECT last_insert_rowid()");
    query.exec();

    if(!query.next())
        return -1;

    int id = query.value(0).toInt();
    telescope->setId(id);

    ProcessingProperties processingProperties = telescope->processingProperties();
    processingProperties.setId(id);
    ProcessingPropertiesDAO processingPropertiesDAO;
    processingPropertiesDAO.save(&processingProperties);
    telescope->setProcessingProperties(processingProperties);

    PaintingProperties* paintingProperties = telescope->paintingProperties();
    paintingProperties->setId(id);
    PaintingPropertiesDAO paintingPropertiesDAO;
    paintingPropertiesDAO.save(paintingProperties);
    telescope->setPaintingProperties(*paintingProperties);

    RemoteTelescope remoteTelescope = telescope->remoteTelescope();
    remoteTelescope.setId(id);
    RemoteTelescopeDAO remoteTelescopeDAO;
    remoteTelescopeDAO.save(remoteTelescope);
    telescope->setRemoteTelescope(remoteTelescope);

    for(Screw screw : telescope->screws())
    {
        query.prepare("INSERT INTO screw (telescope, degrees) VALUES (:telescopeid, :degrees)");
        query.bindValue(":telescopeid", telescope->id());
        query.bindValue(":degrees", screw.degrees());
        query.exec();
    }

    return id;
}


int TelescopeDAO::update(TelescopeUI telescope)
{
    QSqlQuery query(db.getDatabase());

    if(telescope.id() == -1)
        return -1;

    ProcessingProperties processingProperties = telescope.processingProperties();
    ProcessingPropertiesDAO processingPropertiesDAO;
    processingPropertiesDAO.update(processingProperties);

    PaintingProperties* paintingProperties = telescope.paintingProperties();
    PaintingPropertiesDAO paintingPropertiesDAO;
    paintingPropertiesDAO.update(*paintingProperties);

    RemoteTelescope remoteTelescope = telescope.remoteTelescope();
    RemoteTelescopeDAO remoteTelescopeDAO;
    remoteTelescopeDAO.update(remoteTelescope);

    query.prepare("UPDATE telescope "
                "SET name = :name "
                "WHERE id = :telescopeid");

    query.bindValue(":telescopeid", telescope.id());
    query.bindValue(":name", telescope.name());

    query.exec();

    query.prepare("DELETE FROM screw "
                  "WHERE telescope = :telescope");
    query.bindValue(":telescope", telescope.id());
    query.exec();

    for(Screw screw : telescope.screws())
    {
        query.prepare("INSERT INTO screw (degrees, telescope)"
                    "VALUES(:degrees, :telescope)");
        query.bindValue(":degrees", screw.degrees());
        query.bindValue(":telescope", telescope.id());
        query.exec();
    }

    return telescope.id();
}


TelescopeUI TelescopeDAO::get(int id)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT *, t.id as telescopeid, pp.id as processingpropertiesid, sc.id as screwid "
                "FROM telescope t "
                "LEFT JOIN screw sc ON sc.telescope = pp.id "
                "WHERE t.id = :id");
    query.bindValue(":id", id);

    query.exec();
    if(!query.next())
        throw 20;

    TelescopeUI telescope;
    telescope.setId(query.value("telescopeid").toInt());
    telescope.setName(query.value("name").toString());

    QList<Screw> screws;
    while(query.next())
    {
        Screw screw;
        screw.setId(query.value("screwid").toInt());
        screw.setDegrees(query.value("degrees").toFloat());
        screws.append(screw);
    }
    telescope.setScrews(screws);

    ProcessingPropertiesDAO processingPropertiesDAO;
    ProcessingProperties processingProperties = processingPropertiesDAO.get(id);
    telescope.setProcessingProperties(processingProperties);

    PaintingPropertiesDAO paintingPropertiesDAO;
    PaintingProperties paintingProperties = paintingPropertiesDAO.get(id);
    telescope.setPaintingProperties(paintingProperties);

    RemoteTelescopeDAO remoteTelescopeDAO;
    RemoteTelescope remoteTelescope = remoteTelescopeDAO.get(id);
    telescope.setRemoteTelescope(remoteTelescope);

    return telescope;
}


QList<TelescopeUI> TelescopeDAO::getAll()
{
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT *, t.id as telescopeid, sc.id as screwid, t.name as telescopename, rt.name as remotetelescopename "
                "FROM telescope t "
                "LEFT JOIN processing_properties processingProperties ON processingProperties.id = t.id "
                "LEFT JOIN painting_properties paintingProperties ON paintingProperties.id = t.id "
                "LEFT JOIN screw sc ON sc.telescope = t.id "
                "LEFT JOIN remote_telescope rt ON rt.id = t.id "
                "ORDER BY telescopeid");

    query.exec();
    QHash<int, TelescopeUI> telescopes;
    QHash<int, QList<Screw>*> screws;
    while(query.next()) {
        int telescopeId = query.value("telescopeid").toInt();

        TelescopeUI telescope;
        if(telescopes.contains(telescopeId))
            telescope = telescopes.value(telescopeId);
        else
            telescope = createObject(query);

        QList<Screw>* t_screws = new QList<Screw>();
        if(screws.contains(telescopeId))
            t_screws = screws.value(telescopeId);

        if(!query.isNull("screwid")) {
            Screw screw;
            screw.setId(query.value("screwid").toInt());
            screw.setDegrees(query.value("degrees").toFloat());
            t_screws->append(screw);
        }

        telescopes.insert(telescopeId, telescope);
        screws.insert(telescopeId, t_screws);
    }

    for(TelescopeUI telescope : telescopes)
    {
        int telescopeId = telescope.id();
        telescope.setScrews(*screws.value(telescopeId));
        telescopes.insert(telescopeId, telescope);
    }

    return telescopes.values();
}


bool TelescopeDAO::remove(int id)
{
    QSqlQuery query(db.getDatabase());


    query.prepare("DELETE FROM telescope WHERE id = :id");

    query.bindValue(":id", id);

    if(!query.exec())
        return false;
    return true;
}


TelescopeUI TelescopeDAO::createObject(QSqlQuery query)
{
    TelescopeUI telescope;
    telescope.setName(query.value("telescopename").toString());
    telescope.setId(query.value("telescopeid").toInt());

    ProcessingProperties processingProperties;
    processingProperties.setId(query.value("telescopeid").toInt());
    processingProperties.setRefinateRadius(query.value("refinate_radius").toFloat());
    processingProperties.setScale(query.value("scale").toFloat());

    ProcessingProperties::StretchingValues stretchingValues;
    stretchingValues.max = query.value("stretching_max").toFloat();
    stretchingValues.min = query.value("stretching_min").toFloat();
    processingProperties.setStretchingValues(stretchingValues);

    ProcessingProperties::ThresholdValues thresholdValues;
    thresholdValues.constant = query.value("threshold_constant").toDouble();
    thresholdValues.thresholdSize = query.value("threshold_size").toInt();
    processingProperties.setThresholdValues(thresholdValues);

    PaintingProperties paintingProperties;
    paintingProperties.setId(query.value("telescopeid").toInt());
    paintingProperties.setZoom(query.value("zoom").toFloat());
    paintingProperties.setAutomatic(query.value("automatic").toBool());
    paintingProperties.setViewBoundingBox(query.value("view_bounding_box").toBool());
    paintingProperties.setViewScrews(query.value("view_screws").toBool());
    paintingProperties.setViewCalculatedCenter(query.value("view_calculated_center").toBool());
    paintingProperties.setScrewsRadius(query.value("screws_radius").toInt());
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

    RemoteTelescope remoteTelescope(query.value("host").toString(),
                                    query.value("port").toInt(),
                                    query.value("remotetelescopename").toString());
    remoteTelescope.setExposure(query.value("exposure").toDouble());
    remoteTelescope.setId(query.value("telescopeid").toInt());

    telescope.setProcessingProperties(processingProperties);
    telescope.setPaintingProperties(paintingProperties);
    telescope.setRemoteTelescope(remoteTelescope);
    return telescope;
}
