#include "remotetelescopedao.h"

int RemoteTelescopeDAO::save(RemoteTelescope remoteTelescope)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("INSERT INTO remote_telescope "
                    "(id, name, port, host, exposure) "
                    "VALUES (:id, :name, :port, :host, :exposure)");

    bindValues(&query, remoteTelescope);

    if(!query.exec())
        return -1;
    return remoteTelescope.id();
}


int RemoteTelescopeDAO::update(RemoteTelescope remoteTelescope)
{
    QSqlQuery query(db.getDatabase());

    if(remoteTelescope.id() == -1)
        return -1;

    query.prepare("UPDATE remote_telescope "
                    "SET name = :name, "
                    "port = :port, host = :host, "
                    "exposure = :exposure "
                    "WHERE id = :id");

    bindValues(&query, remoteTelescope);

    if(!query.exec())
        return -1;

    return remoteTelescope.id();
}


RemoteTelescope RemoteTelescopeDAO::get(int id)
{
    QSqlQuery query(db.getDatabase());

    query.prepare("SELECT * FROM remote_telescope "
                    "WHERE id = :id");

    query.bindValue(":id", id);

    query.exec();
    return createObject(query);
}


QList<RemoteTelescope> RemoteTelescopeDAO::getAll()
{
    QSqlQuery query(db.getDatabase());

    query.prepare("SELECT * FROM remote_telescope");
    query.exec();

    QList<RemoteTelescope> remoteTelescopes;
    while(query.next())
        remoteTelescopes.append(createObject(query));

    return remoteTelescopes;
}


bool RemoteTelescopeDAO::remove(int id)
{
    QSqlQuery query(db.getDatabase());

    query.prepare("DELETE FROM remote_telescope "
                    "WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}


RemoteTelescope RemoteTelescopeDAO::createObject(QSqlQuery query)
{
    QString host = query.value("host").toString();
    QString name = query.value("name").toString();
    int port = query.value("name").toInt();
    double exposure = query.value("exposure").toDouble();

    RemoteTelescope remoteTelescope(host, port, name);
    remoteTelescope.setExposure(exposure);

    return remoteTelescope;
}


void RemoteTelescopeDAO::bindValues(QSqlQuery* query, RemoteTelescope remoteTelescope)
{
    query->bindValue(":id", remoteTelescope.id());
    query->bindValue(":name", remoteTelescope.name());
    query->bindValue(":port", remoteTelescope.port());
    query->bindValue(":host", remoteTelescope.host());
    query->bindValue(":exposure", remoteTelescope.exposure());
}
