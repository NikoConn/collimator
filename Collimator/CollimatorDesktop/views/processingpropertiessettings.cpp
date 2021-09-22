#include "processingpropertiessettings.h"
#include "ui_processingpropertiessettings.h"
#include <QDebug>

ProcessingPropertiesSettings::ProcessingPropertiesSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessingPropertiesSettings)
{
    ui->setupUi(this);
    this->setWindowTitle("Processing properties");
}

ProcessingPropertiesSettings::~ProcessingPropertiesSettings()
{
    delete ui;
}

void ProcessingPropertiesSettings::setProcessingProperties(ProcessingProperties processingProperties)
{
    this->m_processingProperties = processingProperties;
    setValuesOnScreen(processingProperties);
}

ProcessingProperties ProcessingPropertiesSettings::processingProperties()
{
    this->m_processingProperties.setRefinateRadius(ui->refinateRadiusSpinBox->value());

    ProcessingProperties::StretchingValues stretchingValues;
    stretchingValues.max = ui->stretchingMaxSpinBox->value();
    stretchingValues.min = ui->stretchingMinSpinBox->value();
    this->m_processingProperties.setStretchingValues(stretchingValues);

    ProcessingProperties::ThresholdValues thresholdValues;
    thresholdValues.constant = ui->thresholdConstantSpinBox->value();
    thresholdValues.thresholdSize = ui->thresholdSizeSpinBox->value();
    this->m_processingProperties.setThresholdValues(thresholdValues);

    return this->m_processingProperties;
}

void ProcessingPropertiesSettings::setValuesOnScreen(ProcessingProperties processingProperties)
{
    ui->stretchingMinSpinBox->setValue(processingProperties.stretchingValues().min);
    ui->stretchingMaxSpinBox->setValue(processingProperties.stretchingValues().max);
    ui->refinateRadiusSpinBox->setValue(processingProperties.refinateRadius());
    ui->thresholdConstantSpinBox->setValue(processingProperties.thresholdValues().constant);
    ui->thresholdSizeSpinBox->setValue(processingProperties.thresholdValues().thresholdSize);
}

