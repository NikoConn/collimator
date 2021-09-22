#include "connectionsettingsdialog.h"
#include "ui_connectionsettingsdialog.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <qdebug.h>

ConnectionSettingsDialog::ConnectionSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionSettingsDialog)
{
    ui->setupUi(this);

    connect(&indiClient, &INDIClient::deviceDetected, this, &ConnectionSettingsDialog::deviceDetected);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(ui->connectServerButton, SIGNAL(clicked()), this, SLOT(connectToServer()));

    this->setWindowTitle("Connection settings");
}

ConnectionSettingsDialog::~ConnectionSettingsDialog()
{
    delete ui;
}


void ConnectionSettingsDialog::deviceDetected(RemoteTelescope remoteTelescope)
{
    QString name = remoteTelescope.name();
    ui->deviceComboBox->addItem(name);

    if(ui->deviceComboBox->count() > 0) {
        ui->deviceComboBox->setEnabled(true);
        ui->connectServerButton->setText("Connected!");
    }
}


void ConnectionSettingsDialog::connectToServer()
{
    QString host = ui->hostTextField->text();
    int port = ui->portSpinBox->text().toInt();

    ui->deviceComboBox->clear();
    ui->deviceComboBox->setEnabled(false);
    ui->connectServerButton->setEnabled(false);
    ui->hostTextField->setEnabled(false);
    ui->portSpinBox->setEnabled(false);

    indiClient.disconnect();
    bool connected = indiClient.connectToServer(host, port);
    if(!connected) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", "Couldn't connect to server");
        messageBox.setFixedSize(500,200);
        ui->connectServerButton->setEnabled(true);
        ui->hostTextField->setEnabled(true);
        ui->portSpinBox->setEnabled(true);
    }
    else
    {
        ui->deviceComboBox->clear();
    }
}

void ConnectionSettingsDialog::ok()
{
    QString host = ui->hostTextField->text();
    int port = ui->portSpinBox->text().toInt();
    QString deviceName = ui->deviceComboBox->itemText(ui->deviceComboBox->currentIndex());

    try{
        m_remoteTelescope.setHost(host);
        m_remoteTelescope.setPort(port);
        m_remoteTelescope.setName(deviceName);
        m_remoteTelescope.setExposure(ui->exposureSpinBox->value());
        accept();
    }
    catch(std::invalid_argument exception) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", exception.what());
        messageBox.setFixedSize(500,200);
    }
}

RemoteTelescope ConnectionSettingsDialog::remoteTelescope()
{
    return m_remoteTelescope;
}


void ConnectionSettingsDialog::setRemoteTelescope(RemoteTelescope remoteTelescope)
{
    m_remoteTelescope = remoteTelescope;

    ui->hostTextField->setText(remoteTelescope.host());
    ui->portSpinBox->setValue(remoteTelescope.port());
    ui->deviceComboBox->addItem(remoteTelescope.name());
    ui->exposureSpinBox->setValue(remoteTelescope.exposure());
}
