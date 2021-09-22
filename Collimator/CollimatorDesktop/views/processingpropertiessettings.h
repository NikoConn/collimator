#ifndef PROCESSINGPROPERTIESSETTINGS_H
#define PROCESSINGPROPERTIESSETTINGS_H

#include <QDialog>
#include <model/processingproperties.h>

namespace Ui {
class ProcessingPropertiesSettings;
}

class ProcessingPropertiesSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessingPropertiesSettings(QWidget *parent = nullptr);
    ~ProcessingPropertiesSettings();


    /**
     * @brief setProcessingProperties Sets the processing properties from where to show the data
     * @param processingProperties Processing Properties with data
     */
    void setProcessingProperties(ProcessingProperties processingProperties);


    /**
     * @brief processingProperties Returns the current ProcessingProperties with values from UI
     * @return ProcessingProperties with values from UI
     */
    ProcessingProperties processingProperties();
private:
    Ui::ProcessingPropertiesSettings *ui;
    ProcessingProperties m_processingProperties;

    void setValuesOnScreen(ProcessingProperties processingProperties);
};

#endif // PROCESSINGPROPERTIESSETTINGS_H
