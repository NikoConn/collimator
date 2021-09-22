#ifndef TELESCOPEDAO_H
#define TELESCOPEDAO_H

#include "processingpropertiesdao.h"
#include <logic/telescopeui.h>

class TelescopeDAO
{
public:
    /**
     * @brief TelescopeDAO::save Method for saving an insance of a Telescope
     * @param telescope Telescope to be saved
     * @return id of the saved Telescope. -1 if error
     */
    int save(TelescopeUI*);


    /**
     * @brief update Method for updating an instance of a telescope.
     * Also updates or creates other objects that a telescope contains
     * @param telescope  to be updated
     * @return int Id of the Telescope. -1 if error
     */
    int update(TelescopeUI);



    /**
     * @brief get Method for obtaining a Telescope by id
     * @param id Id of the telescope to be obtained
     * @return Telescope mathcing id
     */
    TelescopeUI get(int id);


    /**
     * @brief getAll Method for obtaining all Telescopes in the database
     * @return QList<Telescope> List of all telescopes
     */
    QList<TelescopeUI> getAll();


    /**
     * @brief remove Method for removing a Telescope by id
     * @param id Id of the telescope to be removed
     * @return bool indicating if removing was successful
     */
    bool remove(int id);

private:
    DatabaseConnectionSingleton db = DatabaseConnectionSingleton::instance();


    /**
     * @brief createObject Private method for getting Telescope object
     * using a row of a query
     * @param query Query with values
     * @return Telescope given the data from the current row of the query
     */
    static TelescopeUI createObject(QSqlQuery query);
};

#endif // TELESCOPEDAO_H
