#include "kheperasimgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{   //Instancia la GUI
    QApplication a(argc, argv);
    KheperaSimGUI w;
    //muestra la GUI
    w.show();
    //Fija el tamaño de la ventana.
    w.setFixedSize(450,275);

    return a.exec();
}
