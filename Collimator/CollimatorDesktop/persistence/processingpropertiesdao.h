#pragma once

#include "databaseconnectionsingleton.h"
#include <model/processingproperties.h>

class ProcessingPropertiesDAO
{
public:
    /**
     * @brief save Method for saving an instance of processingProperties
     * @param processingProperties Processing properties to be saved
     * @return int id of the saved Processing properties. -1 if error.
     */
    int save(ProcessingProperties* processingProperties);


    /**
     * @brief update Method for updating Processing Properties
     * @param processingProperties Processing properties to be updated
     * @return int id of the processing properites. -1 if error
     */
    int update(ProcessingProperties processingProperties);


    /**
     * @brief get Method for obtaining Processing properties by id
     * @param id Id of the processing properties to be obtained
     * @return ProcessingProperties Processing properties matching id
     */
    ProcessingProperties get(int id);


    /**
     * @brief getAll Method for obtaining all Processing Properties in the database
     * @return QList<ProcessingProperties> List of all Processing Properties in the database
     */
    QList<ProcessingProperties> getAll();


    /**
     * @brief remove Method for removing Processing properties by id
     * @param id Id of the processing properties to be removed
     * @return bool indicating if removing was successful
     */
    bool remove(int id);


    /**
     * @brief createObject Method for getting ProcessingProperties
     * object using a row of a query
     * @param query Query with values
     * @return ProcessingProperties given the data from the current row of the query
     */
    static ProcessingProperties createObject(QSqlQuery query);

private:
    DatabaseConnectionSingleton db = DatabaseConnectionSingleton::instance();


    /**
     * @brief bindValues Private method for binding values of
     * the query with Processing properties to reduce code
     * @param query Query where to replace data
     * @param processingProperties Processing properties from where to get data
     */
    static void bindValues(QSqlQuery* query, ProcessingProperties processingProperties);
};
