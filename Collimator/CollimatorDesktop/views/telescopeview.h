#ifndef TELESCOPEVIEW_H
#define TELESCOPEVIEW_H

#include <QMainWindow>
#include <imageresults.h>
#include <logic/collimatorimage.h>
#include <logic/paintingproperties.h>
#include <logic/telescopeui.h>
#include <model/processingproperties.h>
#include <model/remotetelescope.h>
#include <model/telescope.h>
#include <persistence/processingpropertiesdao.h>

namespace Ui {
class TelescopeView;
}

class TelescopeView : public QMainWindow
{
    Q_OBJECT

public:
    explicit TelescopeView(QWidget *parent = nullptr);
    ~TelescopeView();

    void setTelescope(TelescopeUI* telescope);
private:
    Ui::TelescopeView *ui;
    CollimatorImage* image;
    TelescopeUI* telescope;

    void setAutomatic(bool automatic);

private slots:
    void changeProperties();

    void openImageFromFile();
    void openImageProcessing();
    void screwsSettings();
    void saveTelescope();
    void saveScreenshot();
    void connectionSettings();
    void connectTelescope();
    void disconnectTelescope();
    void connectedChanged(bool connected);

signals:
    void propertiesChanged();
};

#endif // TELESCOPEVIEW_H
