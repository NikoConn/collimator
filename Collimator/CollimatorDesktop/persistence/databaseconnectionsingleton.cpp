#include "databaseconnectionsingleton.h"

DatabaseConnectionSingleton::DatabaseConnectionSingleton()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/collimator.db";
    db.setDatabaseName(dbPath);

    if(!QFile::exists(dbPath))
        createTables();
    else
    {
        db.open();
        db.exec("PRAGMA foreign_keys = ON");        //sqlite default = OFF
    }
}


DatabaseConnectionSingleton& DatabaseConnectionSingleton::instance()
{
    static DatabaseConnectionSingleton * _instance = 0;
    if(_instance == 0)
        _instance = new DatabaseConnectionSingleton();
    return *_instance;
}


void DatabaseConnectionSingleton::createTables()
{
    db.open();

    QSqlQuery query(db);

    query.exec("PRAGMA foreign_keys = ON");

    query.exec("CREATE TABLE telescope "
                "(id INTEGER PRIMARY KEY, "
                "name TEXT)");

    query.exec("CREATE TABLE processing_properties "
                "(id INTEGER PRIMARY KEY, "
                "scale FLOAT NOT NULL, "
                "refinate_radius FLOAT, "
                "stretching_max FLOAT, "
                "stretching_min FLOAT, "
                "threshold_size INT, "
                "threshold_constant DOUBLE, "
                "FOREIGN KEY (id) REFERENCES telescope(id) ON DELETE CASCADE)");

    query.exec("CREATE TABLE screw "
                "(id INTEGER PRIMARY KEY, "
                "telescope INT NOT NULL, "
                "degrees FLOAT NOT NULL, "
                "FOREIGN KEY (telescope) REFERENCES telescope(id) ON DELETE CASCADE)");

    query.exec("CREATE TABLE painting_properties "
                "(id INTEGER PRIMARY KEY, "
                "zoom FLOAT, "
                "automatic BOOL, "
                "view_bounding_box BOOL, "
                "view_screws BOOL, "
                "view_calculated_center BOOL, "
                "scope_center_x INT, "
                "scope_center_y INT, "
                "scope_radius INT, "
                "image_mirroring_vertically BOOL, "
                "image_mirroring_horizontally BOOL, "
                "screws_mirroring_vertically BOOL, "
                "screws_mirroring_horizontally BOOL, "
                "tight_mirroring BOOL, "
                "screws_radius INT, "
                "FOREIGN KEY (id) REFERENCES telescope(id) ON DELETE CASCADE)");

    query.exec("CREATE TABLE remote_telescope "
                "(id INTEGER PRIMARY KEY, "
                "name TEXT NOT NULL, "
                "port INT NOT NULL, "
                "host TEXT NOT NULL, "
                "exposure DOUBLE NOT NULL, "
                "FOREIGN KEY (id) REFERENCES telescope(id) ON DELETE CASCADE)");

    query.finish();
}

QSqlDatabase DatabaseConnectionSingleton::getDatabase()
{
    return db;
}

