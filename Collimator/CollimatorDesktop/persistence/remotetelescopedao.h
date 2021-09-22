#ifndef REMOTETELESCOPEDAO_H
#define REMOTETELESCOPEDAO_H

#include "databaseconnectionsingleton.h"

#include <model/remotetelescope.h>



class RemoteTelescopeDAO
{
public:
    /**
     * @brief save Method for saving an instance of RemoteTelescope
     * @param remoteTelescope Remote Telescope to be saved
     * @return int id of the saved Remote telescope. -1 if error
     */
    int save(RemoteTelescope remoteTelescope);


    /**
     * @brief update Method for updating a Remote Telescope
     * @param remoteTelescope Remote Telescope to be updated
     * @return int id of the Remote Telescope. -1 if error
     */
    int update(RemoteTelescope remoteTelescope);


    /**
     * @brief get Method for obtaining Remote Telescope by id
     * @param id Id of the Remote Telescope to be obtained
     * @return RemoteTelescope Remote telescope matching id
     */
    RemoteTelescope get(int id);


    /**
     * @brief getAll Method for obtaining all Remote Telescopes in the database
     * @return QList<RemoteTelescope> List of all Remote Telescopes in the database
     */
    QList<RemoteTelescope> getAll();


    /**
     * @brief remove Method for removing Remote telescope by id
     * @param id Id of the remote telescope to be removed
     * @return bool indicating if removing was successful
     */
    bool remove(int id);


    /**
     * @brief createObject Method for getting RemoteTelescope
     * @param query Query with values
     * @return RemoteTelescope given the data from the current row of the query
     */
    static RemoteTelescope createObject(QSqlQuery query);

private:
    /**
     * @brief bindValues Private method for binding values of the query with RemoteTelescope to reduce code
     * @param query Query where to replace data
     * @param remoteTelescope Remote Telescope from where to get data
     */
    void bindValues(QSqlQuery* query, RemoteTelescope remoteTelescope);
    DatabaseConnectionSingleton db = DatabaseConnectionSingleton::instance();
};

#endif // REMOTETELESCOPEDAO_H
