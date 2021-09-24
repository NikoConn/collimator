#include "telescopeselector.h"
#include "telescopeview.h"
#include "ui_telescopeselector.h"

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>


TelescopeSelector::TelescopeSelector(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TelescopeSelector)
{
    ui->setupUi(this);
    this->setWindowTitle("Collimator");

    TelescopeDAO telescopeDAO;
    paintTelescopeList(telescopeDAO.getAll());

    connect(ui->telescopesList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onClickTelescope()));
    connect(ui->addTelescopeButton, SIGNAL(clicked()), this, SLOT(createTelescope()));
    connect(ui->removeTelescopeButton, SIGNAL(clicked()), this, SLOT(removeTelescope()));

    this->installEventFilter(this);
}


TelescopeSelector::~TelescopeSelector()
{
    delete ui;
}


void TelescopeSelector::paintTelescopeList(QList<TelescopeUI> telescopes)
{
    m_telescopes.clear();
    ui->telescopesList->clear();
    for(TelescopeUI telescope : telescopes)
    {
        m_telescopes.append(telescope);
        ui->telescopesList->addItem(telescope.name());
    }
}


void TelescopeSelector::onClickTelescope()
{
    int index = ui->telescopesList->selectionModel()->selectedIndexes().at(0).row();
    TelescopeView *telescopeView = new TelescopeView(this);
    telescopeView->setTelescope(&m_telescopes[index]);
    telescopeView->move(frameGeometry().topLeft());
    telescopeView->raise();
    telescopeView->show();
}


void TelescopeSelector::createTelescope()
{
    bool ok;
    creating = true;
    QString telescopeName = QInputDialog::getText(this, "Telescope Name", "Telescope name:", QLineEdit::Normal, "", &ok);
    if(ok)
    {
       try {
           TelescopeDAO dao;
           TelescopeUI telescope;
           telescope.setName(telescopeName);
           dao.save(&telescope);

           m_telescopes.append(telescope);
           ui->telescopesList->addItem(telescope.name());
       }  catch (std::invalid_argument exception) {
           QMessageBox messageBox;
           messageBox.critical(0,"Error", exception.what());
           messageBox.setFixedSize(500,200);
       }
    }
    creating = false;
}


void TelescopeSelector::removeTelescope()
{
    int index = ui->telescopesList->selectionModel()->selectedIndexes().at(0).row();
    Telescope telescope = m_telescopes[index];

    TelescopeDAO dao;
    dao.remove(telescope.id());

    delete ui->telescopesList->selectedItems().at(0);
    ui->telescopesList->clearSelection();
    m_telescopes.removeAt(index);
}


bool TelescopeSelector::eventFilter(QObject *object, QEvent *event)
{
    if(!ui->telescopesList->selectionModel()->hasSelection() || event->type() != QEvent::KeyRelease || creating)
            return false;

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    switch(keyEvent->key()){
        case(Qt::Key_Delete):
            removeTelescope();
            return true;
        case(Qt::Key_Space):
        case(Qt::Key_Return):
        case(Qt::Key_Enter):
            onClickTelescope();
            return true;
    }
    return false;
}
