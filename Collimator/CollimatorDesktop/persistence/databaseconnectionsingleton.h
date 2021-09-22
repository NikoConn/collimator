#ifndef DATABASECONNECTIONSINGLETON_H
#define DATABASECONNECTIONSINGLETON_H

#include <QtSql/QtSql>

/**
 * @brief DatabaseConnectionSingleton Class for
 * using only one instance of the database. SQLQLITE only accepts one connection
 * at time.
 */
class DatabaseConnectionSingleton
{
public:
    /**
     * @brief instance Method that creates an instance if
     * it is not created or returns the created instance
     * @return instance of the databse connection
     */
    static DatabaseConnectionSingleton & instance();


    /**
     * @brief getDatabase
     * @return Returns single instance of the database
     */
    QSqlDatabase getDatabase();
private:
    DatabaseConnectionSingleton();
    QSqlDatabase db;

    /**
     * @brief createTables Method that creates all
     * tables needed
     */
    void createTables();
};

#endif // DATABASECONNECTIONSINGLETON_H
