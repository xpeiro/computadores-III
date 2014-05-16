//Definición de la clase de la Interfaz Gráfica.
#ifndef KHEPERASIMGUI_H
#define KHEPERASIMGUI_H
#include <QMainWindow>
#include <demo.h>
#include <control.h>
#include <i_control.h>
#include <stdio.h>


namespace Ui {
class KheperaSimGUI;
}

class KheperaSimGUI : public QMainWindow
{
    Q_OBJECT

public:

    explicit KheperaSimGUI(QWidget *parent = 0);
    ~KheperaSimGUI();
    typedef int (I_Control::* ptrfunc) (int);

private slots:
    //inicia la simulación remotamente (IP:parámetro. Puerto: 19997, predeterminado de V-REP)
    void iniciar_sim(std::string ip);
    //abre una conexión remota nueva en la ip y puerto dada
    int conectar(std::string ip,int puerto);
    //ejecuta la función hilo como un thread y lo desvincula del padre-> detach().
    void codigo_en_hilo(KheperaSimGUI::ptrfunc ptrfuncion);
    //muestra los datos de posicion y velocidad en la GUI.
    void refrescar_datos();
    //Slots/Métodos que se cargan en los eventos definidos por sus nombres.
    void on_ejecutar_clicked();

    void on_sim_clicked();

    void on_parar_clicked();

    void on_pausar_clicked();

    void on_interrupt1_clicked();

    void on_interrupt2_clicked();

    void on_interrupt3_clicked();

    void on_interrupt4_clicked();

    void on_controlset_clicked();

    void on_demoset_clicked();

    void on_comboBox_2_activated(QString robottexto);

    void on_comboBox_3_activated(const QString a);

    void on_comboBox_activated(int index);

    void on_actionSalir_triggered();

    void on_actionMostrar_Output_toggled(bool arg1);

    void closeEvent(QCloseEvent *bar);

    void on_actionAyuda_triggered();

private:

    Ui::KheperaSimGUI *ui;
    //Handle/identificador usado para conexiones de señales de manejo de simulación (Iniciar, Parar, Pausar/Reanudar)
    //Usa siempre el puerto 19997, inicializado por defecto en V-REP
    int clientIDsim=-1;
    //Handle/identificador usado para conexiones de señales de control/interrupcion.
    //Usa el puerto definido por el usuario (debe abrirse manualmente con un Script Lua. Ver modelo Khepera incluido).
    int clientIDexe=-1;
    //IP del Host Simulador. Localhost por defecto.
    std::string ip="127.0.0.1";
    //Puerto de escucha. Debe configurarse en un Script Lua. Ver modelo Khepera incluido.
    int puerto=20001;
    // buffer de coordenadas de posición.
    float position[3];
    // buffer de módulos de velocidad (lineal).
    float velocidad[3];
    //toggle de la advertencia de velocidad excesiva.
    bool nomostrarwarning =false;

    FILE * log;

    Demo demo;

    Control control;



};

#endif // KHEPERASIMGUI_H
