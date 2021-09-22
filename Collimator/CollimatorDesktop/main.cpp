#include <QApplication>
#include <model/indiclient.h>
#include <views/telescopeselector.h>

#include <opencv2/core/mat.hpp>
using namespace cv;
#include <collimator.h>
#include <omp.h>


int main(int argc, char *argv[])
{
    qRegisterMetaType<RemoteTelescope>("RemoteTelescope");

    QApplication a(argc, argv);
    TelescopeSelector w;
    w.show();
    return a.exec();
}
