#include "connectionsettingsdialog.h"
#include "screwsdialog.h"
#include "telescopeview.h"
#include "ui_telescopeview.h"
#include <QFileDialog>
#include <collimator.h>
#include <qpainter.h>
#include <views/processingpropertiessettings.h>
#include <views/screwsdialog.h>
#include <QAction>
#include <QErrorMessage>
#include <QMessageBox>
#include <QScrollBar>
#include <qinputdialog.h>
#include <persistence/remotetelescopedao.h>
#include <persistence/telescopedao.h>

TelescopeView::TelescopeView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelescopeView)
{
    ui->setupUi(this);

    QPushButton* zoomInBtn = ui->zoomIn;
    zoomInBtn->setIcon(QIcon(":/icons/zoom-in.png"));
    QPushButton* zoomOutBtn = ui->zoomOut;
    zoomOutBtn->setIcon(QIcon(":/icons/zoom-out.png"));

    image = new CollimatorImage(this);
    image->setScrollArea(ui->imageScrollArea);
    ui->imageScrollArea->setWidget(image);
    ui->imageScrollArea->setAlignment(Qt::AlignCenter);

    connect(zoomInBtn, &QPushButton::clicked, this, [this] { image->zoom(0.5); });
    connect(zoomOutBtn, &QPushButton::clicked, this, [this] { image->zoom(-0.5); });

    connect(ui->loadImageButton, SIGNAL(clicked()), this, SLOT(openImageFromFile()));
    connect(ui->actionImage_processing_settings, SIGNAL(triggered()), this, SLOT(openImageProcessing()));
    connect(ui->actionSettingsScrews, SIGNAL(triggered()), this, SLOT(screwsSettings()));
    connect(ui->actionSave_screenshot, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
    connect(ui->actionClose_2, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionConnection, &QAction::triggered, this, &TelescopeView::connectionSettings);

    connect(ui->actionAutomatic, &QAction::triggered, this, [this] { this->setAutomatic(true); });
    connect(ui->actionManual, &QAction::triggered, this, [this] { this->setAutomatic(false); });
    connect(ui->actionViewBounding_Box, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionViewScrews, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionCalculated_Center, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionDebug_Image, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionMirrorImageHorizontally, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionMirrorImageVertically, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionMirrorScrewsHorizontally, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionMirrorScrewsVertically, &QAction::triggered, this, &TelescopeView::changeProperties);
    connect(ui->actionMirrorTightLose, &QAction::triggered, this, &TelescopeView::changeProperties);

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectTelescope()));
    connect(image, &CollimatorImage::connectedChanged, this, &TelescopeView::connectedChanged);
    connect(this, &TelescopeView::propertiesChanged, image, &CollimatorImage::propertiesChanged);
}


TelescopeView::~TelescopeView()
{
    delete ui;
}


void TelescopeView::setTelescope(TelescopeUI* telescope)
{
    this->telescope = telescope;
    this->setWindowTitle(telescope->name());

    PaintingProperties* paintingProperties = telescope->paintingProperties();

    image->setTelescope(this->telescope);

    setAutomatic(paintingProperties->automatic());
    ui->actionViewBounding_Box->setChecked(paintingProperties->viewBoundingBox());
    ui->actionCalculated_Center->setChecked(paintingProperties->viewCalculatedCenter());
    ui->actionViewScrews->setChecked(paintingProperties->viewScrews());
    ui->connectButton->setEnabled(telescope->remoteTelescope().isValid());
    ui->actionMirrorImageHorizontally->setChecked(paintingProperties->imageMirroring().horizontally);
    ui->actionMirrorImageVertically->setChecked(paintingProperties->imageMirroring().vertically);
    ui->actionMirrorScrewsHorizontally->setChecked(paintingProperties->screwsMirroring().horizontally);
    ui->actionMirrorScrewsVertically->setChecked(paintingProperties->screwsMirroring().vertically);
    ui->actionMirrorTightLose->setChecked(paintingProperties->tightMirroring());

    connect(this, SIGNAL(propertiesChanged()), this, SLOT(saveTelescope()));
}


void TelescopeView::openImageFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.fits *.png *.jpg)"));
    if(filePath.size() > 0)
        image->loadImage(filePath);
}


void TelescopeView::openImageProcessing()
{
    ProcessingProperties processingProperties = telescope->processingProperties();
    ProcessingPropertiesSettings settingsDialog(this);
    settingsDialog.setProcessingProperties(processingProperties);
    int dialogCode = settingsDialog.exec();
    if(dialogCode == QDialog::Accepted)
    {
        try
        {
            ProcessingProperties pp = settingsDialog.processingProperties();
            this->telescope->setProcessingProperties(pp);
            ProcessingPropertiesDAO dao;
            dao.update(pp);
            emit propertiesChanged();
        }
        catch (std::invalid_argument exception)
        {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", exception.what());
            messageBox.setFixedSize(500,200);
            if(messageBox.Accepted)
                openImageProcessing();
        }
    }
}


void TelescopeView::screwsSettings()
{
    ScrewsDialog screwsDialog(this);
    screwsDialog.setScrews(telescope->screws());
    screwsDialog.setRadius(telescope->paintingProperties()->screwsRadius());
    int dialogCode = screwsDialog.exec();

    if(dialogCode == QDialog::Accepted)
    {
        try
        {
            TelescopeDAO tDAO;
            telescope->setScrews(screwsDialog.screws());
            telescope->paintingProperties()->setScrewsRadius(screwsDialog.radius());
            emit propertiesChanged();
        }
        catch (std::invalid_argument exception)
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Error", exception.what());
            messageBox.setFixedSize(500,200);
        }
    }
}


void TelescopeView::setAutomatic(bool automatic)
{
    telescope->paintingProperties()->setAutomatic(automatic);
    ui->actionAutomatic->setChecked(automatic);
    ui->actionManual->setChecked(!automatic);

    ui->actionViewBounding_Box->setEnabled(automatic);
    ui->actionCalculated_Center->setEnabled(automatic);
    ui->actionManual->setEnabled(automatic);
    ui->actionAutomatic->setEnabled(!automatic);

    emit propertiesChanged();
}


void TelescopeView::saveTelescope()
{
    TelescopeDAO dao;
    dao.update(telescope);
}

void TelescopeView::saveScreenshot()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save image", "", ".png");
    if(filePath.length() > 0)
        image->pixmap()->save(filePath, "PNG");
}

void TelescopeView::connectionSettings()
{
    try
    {
        ConnectionSettingsDialog dialog(this);
        dialog.setRemoteTelescope(telescope->remoteTelescope());
        int dialogCode = dialog.exec();

        if(dialogCode == QDialog::Accepted)
        {
            telescope->setRemoteTelescope(dialog.remoteTelescope());
            ui->connectButton->setEnabled(telescope->remoteTelescope().isValid());

            RemoteTelescopeDAO remoteTelescopeDAO;
            remoteTelescopeDAO.update(telescope->remoteTelescope());
        }
    }
    catch (Exception exception)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", exception.what());
        messageBox.setFixedSize(500,200);
    }
}

void TelescopeView::connectTelescope()
{
    image->connectTelescope(telescope->remoteTelescope());
}

void TelescopeView::disconnectTelescope()
{
    image->setConnected(false);
}

void TelescopeView::connectedChanged(bool connected)
{
    if(connected)
    {
        ui->connectButton->setText("Disconnect");
        ui->loadImageButton->setEnabled(false);
        disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectTelescope()));
        connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectTelescope()));
    }
    else
    {
        ui->connectButton->setText("Connect");
        ui->loadImageButton->setEnabled(true);
        disconnect(ui->connectButton, SIGNAL(clicked()), this, SLOT(disconnectTelescope()));
        connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectTelescope()));
    }
}


void TelescopeView::changeProperties()
{
    telescope->paintingProperties()->setViewScrews(ui->actionViewScrews->isChecked());
    telescope->paintingProperties()->setViewCalculatedCenter(ui->actionCalculated_Center->isChecked());
    telescope->paintingProperties()->setViewBoundingBox(ui->actionViewBounding_Box->isChecked());
    image->setDebug(ui->actionDebug_Image->isChecked());

    PaintingProperties::Mirroring imageMirroring;
    imageMirroring.horizontally = ui->actionMirrorImageHorizontally->isChecked();
    imageMirroring.vertically = ui->actionMirrorImageVertically->isChecked();
    PaintingProperties::Mirroring screwsMirroring;
    screwsMirroring.horizontally = ui->actionMirrorScrewsHorizontally->isChecked();
    screwsMirroring.vertically = ui->actionMirrorScrewsVertically->isChecked();

    telescope->paintingProperties()->setImageMirroring(imageMirroring);
    telescope->paintingProperties()->setScrewsMirroring(screwsMirroring);
    telescope->paintingProperties()->setTigthMirroring(ui->actionMirrorTightLose->isChecked());

    emit propertiesChanged();
}
