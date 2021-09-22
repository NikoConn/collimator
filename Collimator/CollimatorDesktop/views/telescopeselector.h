#ifndef TELESCOPESELECTOR_H
#define TELESCOPESELECTOR_H

#include <QListWidget>
#include <QMainWindow>

#include <model/telescope.h>
#include <persistence/telescopedao.h>

QT_BEGIN_NAMESPACE
namespace Ui { class TelescopeSelector; }
QT_END_NAMESPACE

class TelescopeSelector : public QMainWindow
{
    Q_OBJECT

public:
    TelescopeSelector(QWidget *parent = nullptr);
    ~TelescopeSelector();

private:
    Ui::TelescopeSelector *ui;
    QList<TelescopeUI> m_telescopes;
    bool creating = false;

    void paintTelescopeList(QList<TelescopeUI> telescopes);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void onClickTelescope();
    void createTelescope();
    void removeTelescope();
};
#endif // TELESCOPESELECTOR_H
