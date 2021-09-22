#include "screwsdialog.h"
#include "ui_screwsdialog.h"

#include <QInputDialog>

ScrewsDialog::ScrewsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScrewsDialog)
{
    ui->setupUi(this);
    loadItems();
    connect(ui->addScrewButton, SIGNAL(clicked()), this, SLOT(addScrew()));
    connect(ui->removeScrewButton, SIGNAL(clicked()), this, SLOT(removeScrew()));
    this->setWindowTitle("Screws");
}


ScrewsDialog::~ScrewsDialog()
{
    delete ui;
}


void ScrewsDialog::setScrews(QList<Screw> screws)
{
    m_screws = screws;
    loadItems();
}


QList<Screw> ScrewsDialog::screws()
{
    return m_screws;
}


void ScrewsDialog::setRadius(int radius)
{
    ui->centerDistanceSpinBox->setValue(radius);
}


int ScrewsDialog::radius()
{
    return ui->centerDistanceSpinBox->value();
}


void ScrewsDialog::loadItems()
{
    ui->screwListWidget->clear();
    for(Screw screw : m_screws)
        ui->screwListWidget->addItem(QString::number(screw.degrees()) + "°");
}


void ScrewsDialog::addScrew()
{
    bool ok;
    int degrees = QInputDialog::getInt(this, "Screw degrees: ", "Screw degrees (0-360):", 0, 0, 360, 1, &ok);
    if(ok && degrees >= 0 && degrees <= 360)
    {
        Screw screw;
        screw.setDegrees(degrees);
        m_screws.append(screw);
    }
    loadItems();
}


void ScrewsDialog::removeScrew()
{
    int index = ui->screwListWidget->selectionModel()->selectedIndexes().at(0).row();
    m_screws.removeAt(index);
    loadItems();
}
