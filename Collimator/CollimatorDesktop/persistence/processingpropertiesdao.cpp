#include <persistence/processingpropertiesdao.h>

int ProcessingPropertiesDAO::save(ProcessingProperties* processingProperties)
{
    QSqlQuery query(db.getDatabase());
    query.prepare("INSERT INTO processing_properties "
                "(id, scale, refinate_radius, stretching_max, "
                "stretching_min, threshold_size, threshold_constant) "
                "VALUES ( :id, :scale, :refinateradius, :stretchingmax, "
                ":stretchingmin, :thresholdsize, :thresholdconstant)");

    bindValues(&query, *processingProperties);
    query.bindValue(":id", processingProperties->id());

    query.exec();
    if(!query.next())
    {
        return -1;
    }
   return processingProperties->id();
}


int ProcessingPropertiesDAO::update(ProcessingProperties processingProperties)
{
    QSqlQuery query(db.getDatabase());

    if(processingProperties.id() == -1)
        return -1;

    query.prepare("UPDATE processing_properties "
                "SET scale = :scale, refinate_radius = :refinateradius, stretching_max = :stretchingmax, "
                "stretching_min = :stretchingmin, threshold_size = :thresholdsize, "
                "threshold_constant = :thresholdconstant WHERE id = :id");

    bindValues(&query, processingProperties);
    query.bindValue(":id", processingProperties.id());

    query.exec();
    return query.next() ? processingProperties.id() : -1;
}


ProcessingProperties ProcessingPropertiesDAO::get(int id)
{
    QSqlQuery query(db.getDatabase());

    query.prepare("SELECT * FROM processing_properties WHERE id = :id");
    query.bindValue(":id", id);

    query.exec();
    return createObject(query);
}


QList<ProcessingProperties> ProcessingPropertiesDAO::getAll()
{
    QSqlQuery query(db.getDatabase());

    query.prepare("SELECT * FROM processing_properties");
    query.exec();

    QList<ProcessingProperties> processingPropertiesList;
    while(query.next())
        processingPropertiesList.append(createObject(query));

    return processingPropertiesList;
}


bool ProcessingPropertiesDAO::remove(int id)
{
    QSqlQuery query(db.getDatabase());

    query.prepare("DELETE FROM processing_properties "
                "WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();

}


void ProcessingPropertiesDAO::bindValues(QSqlQuery* query, ProcessingProperties processingProperties)
{
    query->bindValue(":scale", processingProperties.scale());
    query->bindValue(":refinateradius", processingProperties.refinateRadius());
    query->bindValue(":stretchingmin", processingProperties.stretchingValues().min);
    query->bindValue(":stretchingmax", processingProperties.stretchingValues().max);
    query->bindValue(":thresholdsize", processingProperties.thresholdValues().thresholdSize);
    query->bindValue(":thresholdconstant", processingProperties.thresholdValues().constant);
}


ProcessingProperties ProcessingPropertiesDAO::createObject(QSqlQuery query)
{
    ProcessingProperties processingProperties;

    processingProperties.setId(query.value("id").toInt());
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

    return processingProperties;
}
