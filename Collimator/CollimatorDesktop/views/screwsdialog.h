#ifndef SCREWSDIALOG_H
#define SCREWSDIALOG_H

#include <QDialog>
#include "model/screw.h"

namespace Ui {
class ScrewsDialog;
}

class ScrewsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScrewsDialog(QWidget *parent = nullptr);
    ~ScrewsDialog();


    /**
     * @brief setScrews Sets the Screws list from where to show the data
     * @param screws Screws list
     */
    void setScrews(QList<Screw> screws);


    /**
     * @brief setRadius Sets the radius of the screws
     * @param radius Radius of the screws
     */
    void setRadius(int radius);


    /**
     * @brief radius Returns the current radius with values from UI input
     * @return Radius from UI input
     */
    int radius();


    /**
     * @brief screws Returns the current list of screws from UI input
     * @return Screw List from UI input
     */
    QList<Screw> screws();
private:
    Ui::ScrewsDialog *ui;
    QList<Screw> m_screws;

    void loadItems();

private slots:
    void addScrew();
    void removeScrew();
};

#endif // SCREWSDIALOG_H
