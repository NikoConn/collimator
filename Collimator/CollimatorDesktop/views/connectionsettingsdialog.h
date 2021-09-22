#ifndef CONNECTIONSETTINGSDIALOG_H
#define CONNECTIONSETTINGSDIALOG_H

#include <QDialog>

#include <model/indiclient.h>

namespace Ui {
class ConnectionSettingsDialog;
}

class ConnectionSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionSettingsDialog(QWidget *parent = nullptr);
    ~ConnectionSettingsDialog();


    /**
     * @brief setRemoteTelescope Sets the remote telescope from where to show the data
     * @param remoteTelescope Remote telescope with data
     */
    void setRemoteTelescope(RemoteTelescope remoteTelescope);


    /**
     * @brief remoteTelescope Returns the current Remote Telescope with values from UI input
     * @return Remote Telescope with values from UI input
     */
    RemoteTelescope remoteTelescope();
private:
    Ui::ConnectionSettingsDialog *ui;
    INDIClient indiClient;
    RemoteTelescope m_remoteTelescope;

private slots:
    void connectToServer();
    void deviceDetected(RemoteTelescope);
    void ok();
};

#endif // CONNECTIONSETTINGSDIALOG_H
