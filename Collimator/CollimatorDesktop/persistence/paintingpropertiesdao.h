#ifndef PAINTINGPROPERTIESDAO_H
#define PAINTINGPROPERTIESDAO_H

#include "databaseconnectionsingleton.h"
#include <logic/paintingproperties.h>


class PaintingPropertiesDAO
{
public:
    /**
     * @brief save Method for saving an instance of PaintingProperties
     * @param paintingProperties PaintingProperties to be saved
     * @return id of the saved PaintingProperties
     */
    int save(PaintingProperties* paintingProperties);


    /**
     * @brief update Method for updating an instance of PaintingProperties
     * @param paintingProperties to be opdated
     * @return id of the painting properties
     */
    int update(PaintingProperties paintingProperties);


    /**
     * @brief get Mehod for obtaining a PaintingProperties by its id
     * @param id Id of the telescope to be obtained
     * @return
     */
    PaintingProperties get(int id);


    /**
     * @brief getAll Mehod for obtaining all PaintingProperties in the database
     * @return QList<PaintingProperites> List of all PaintingProperties
     */
    QList<PaintingProperties> getAll();


    /**
     * @brief remove Method for removing a PaintingProperties by its id
     * @param id Id of the PaintingProperties to be removed
     * @return bool indicating if removing was successful
     */
    bool remove(int id);


    /**
     * @brief createObject Method for getting PaintingProperties object given a query
     * @param query Query containing values
     * @return PaintingProperties given the data from the current row of the query
     */
    static PaintingProperties createObject(QSqlQuery query);

private:
    DatabaseConnectionSingleton db = DatabaseConnectionSingleton::instance();


    /**
     * @brief bindValues Private method for binding values fo the query with PaintingProperties to reduce code
     * @param query Query where to replace data
     * @param paintingProperties PaintingProperties from where to get data
     */
    void bindValues(QSqlQuery* query, PaintingProperties paintingProperties);
};

#endif // PAINTINGPROPERTIESDAO_H
