#include "kheperasimgui.h"
#include "ui_kheperasimgui.h"
#include <thread>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <iostream>
#include <i_control.h>
#include "control.cpp"
#include "demo.cpp"
#include <QTextStream>
#include <QScrollBar>
#include <QUrl>
#include <QDesktopServices>
extern "C" {
#include "extApi.h"
}

using namespace std;
//Constructor del objeto GUI
KheperaSimGUI::KheperaSimGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KheperaSimGUI)
{
    ui->setupUi(this);
    log = freopen ("log","w",stdout);

    //Define un temporizador que ejecutar refrescar_datos() cada 150ms
    QTimer *timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(refrescar_datos()));
         //inicia el temporizador
         timer->start(150);

}
//Destructor del objeto GUI
KheperaSimGUI::~KheperaSimGUI()
{
    delete ui;
}


//Abre una conexión con V-REP (puerto por defecto 19997), Inicia la simulacion y cierra la conexión.
void KheperaSimGUI::iniciar_sim(string ip){
    //Comprueba que no haya una conexión ya abierta antes de conectar.
    if (simxGetConnectionId(clientIDsim)==-1){
        clientIDsim=simxStart((simxChar*) ip.c_str(),19997,true,true,2000,5);
    }

    //Comprueba que se haya realizado la conexión con éxito. Si no, muestra error.
    if (clientIDsim!=-1)
    {
        simxStartSimulation(clientIDsim,simx_opmode_oneshot_wait);
        simxFinish(clientIDsim);
        ui->pausar->setEnabled(true);
        cout << "Gui: Simulación iniciada\n";

    } else {
        cout << "Error: Imposible iniciar simulación remotamente. (Pulsar Play en V-REP y volver a intentar)\n";
        QMessageBox error;
        error.setText("Error: Imposible iniciar simulación remotamente. (Pulsar Play en V-REP y volver a intentar)");
        error.exec();
    }
}
//Abre una conexión con la escena V-REP, en la IP y puerto dado.
int KheperaSimGUI::conectar(string ip,int puerto){

    clientIDexe=simxStart((simxChar*) ip.c_str(),puerto,true,true,2000,5);
    //Comprueba la conexión, si ha fallado, muestra error.
    if (simxGetConnectionId(clientIDexe)==-1) {
       cout << "Error: Imposible conectar con el servidor. Asegúrese de que el puerto es correcto (" << ui->puertotexto->text().toInt() <<")\n";
       cout << "Debe ejecutar un proceso servidor en el Host Simulador, normalmente con una llamada a\n";
       cout << "simExtRemoteApiStart(puerto) en un Child Script de Lua.\n";
       cout << "Ver código de control demo y escena demo2khepera.ttt\n";
       QMessageBox error;
       error.setText("Error: Imposible conectar con el servidor");
       error.exec();
   }
    return clientIDexe;
}

//crea y lanza un thread (hilo) con el metodo pedido, para permitir funcionamiento concurrente.

void KheperaSimGUI::codigo_en_hilo(ptrfunc ptrfuncion)
{
    if (ui->demoset->isChecked()){
        std::thread hilo(ptrfuncion,&demo,clientIDexe);
        //el hilo continua ejecutándose hasta que retorna la función hilo (el hilo padre se "desprende" del hijo)
        hilo.detach();
    } else {
        std::thread hilo(ptrfuncion,&control,clientIDexe);
        hilo.detach();
    }
}

//refresca los datos de posición y velocidad de la gui. La invoca el timer creado en el constructor de la GUI.
void KheperaSimGUI::refrescar_datos()
{       //Comprueba que haya una conexión. Si la hay, obtiene los datos. (Ver Doc. Remote API para entender funciones)
        if (simxGetConnectionId(clientIDexe)!=-1) {
            int robotHandle;
            //indica de que robot se representarán los datos, segun el combobox de selección.
            string robot = ui->comboBox_2->currentText().toUtf8().constData();
            simxGetObjectHandle(clientIDexe,robot.c_str(),&robotHandle,simx_opmode_oneshot_wait);
            simxGetObjectPosition(clientIDexe,robotHandle,-1,position,simx_opmode_oneshot);
            simxGetObjectVelocity(clientIDexe,robotHandle,velocidad,NULL,simx_opmode_oneshot);
        } else {
            //Si no hay conexión, pone a 0 los valores.
            for (int i = 0; i<3; i++){
                position[i]=0;
                velocidad[i]=0;
            }
        }
        //actualiza los "LCD" de la GUI
        ui->xlcd->display(position[0]);
        ui->ylcd->display(position[1]);
        ui->zlcd->display(position[2]);
        ui->vxlcd->display(velocidad[0]);
        ui->vylcd->display(velocidad[1]);
        ui->vzlcd->display(velocidad[2]);
        //Muestra el contenido del archivo log en el textEdit. Solo si "mostrar output" está selecc.
        if (ui->actionMostrar_Output->isChecked()) {
            fclose(log);
            log = fopen("log","r");
            QTextStream stream(log);
            QString str = stream.readAll();
            ui->textEdit->setText(str);
            QScrollBar *sb = ui->textEdit->verticalScrollBar();
            sb->setValue(sb->maximum());
            fclose(log);
            //redirecciona stdout al archivo log. ver página man de freopen.
            log = freopen ("log","a",stdout);
        }


}

//código de evento: pulsar botón ejecutar.
void KheperaSimGUI::on_ejecutar_clicked()
{   //define la IP y puerto desde los textbox.
    ip = ui->iptexto->text().toUtf8().constData();
    puerto = ui->puertotexto->text().toInt();
    //si está marcada la opción de Inicio remoto, inicia la sim. remotamente.
    if(ui->inicioremotoset->isChecked()){
        iniciar_sim(ip);
    }
    //cierra cualquier conexión de control/interrupcion que haya.
    simxFinish(clientIDexe);
    //conecta y ejecuta el código de control (Demo ó Control) en un hilo independiente.
    if (conectar(ip,puerto) != -1) {
        ptrfunc ptrfuncion = &I_Control::control;
        codigo_en_hilo(ptrfuncion);
        cout << "Gui: Control ejecutado\n";
    }


}
//código de evento: pulsar botón iniciar simulación.
void KheperaSimGUI::on_sim_clicked()
{
    iniciar_sim(ui->iptexto->text().toUtf8().constData());

}
//código de evento: pulsar botón parar simulación.
void KheperaSimGUI::on_parar_clicked()
{
    //abre una conexión en el puerto por defecto, para la simulación y cierra la conexión.
    clientIDsim=simxStart((simxChar*) ip.c_str(),19997,true,true,2000,5);
    simxStopSimulation(clientIDsim,simx_opmode_oneshot_wait);
    simxFinish(clientIDsim);
    cout << "Gui: Simulación parada\n";
    ui->pausar->setText("Pausar Sim.");
    ui->pausar->setEnabled(false);

}
//código de evento: pulsar boton pausar/reanudar sim.
void KheperaSimGUI::on_pausar_clicked()
{   //obtiene el estado actual del botón: Pausar/Reanudar.
    string texto = ui->pausar->text().toUtf8().constData();
    //abre una conexión en el puerto por defecto.
    clientIDsim=simxStart((simxChar*) ip.c_str(),19997,true,true,2000,5);
    //si tiene que pausar, pausa, y se pone en estado reanudar. Ídem al contrario.
    if (!texto.compare("Pausar Sim.")) {
        simxPauseSimulation(clientIDsim,simx_opmode_oneshot_wait);
        cout << "Gui: Simulación pausada\n";
        ui->pausar->setText("Reanud Sim.");
    } else {
        simxStartSimulation(clientIDsim,simx_opmode_oneshot_wait);
        cout << "Gui: Simulación reanudada\n";
        ui->pausar->setText("Pausar Sim.");
    }
    //cierra la conexión en puerto por defecto.
    simxFinish(clientIDsim);


}
//código de evento: pulsar botón de interrupt X.
void KheperaSimGUI::on_interrupt1_clicked()
{   //cierra cualquier conexión de control/interrupcion que exista,conecta
    //y ejecuta el código de interrupción (Demo ó Control) en un hilo independiente.
    simxFinish(clientIDexe);
    if (conectar(ip,puerto) != -1) {
        //crea un puntero a metodo miembro con el metodo seleccionado.
        ptrfunc ptrfuncion = &I_Control::interrupt_1;
        //ejecuta el metodo seleccionado en un hilo.
        codigo_en_hilo(ptrfuncion);
    }
}

void KheperaSimGUI::on_interrupt2_clicked()
{
    simxFinish(clientIDexe);
    if (conectar(ip,puerto) != -1) {
        ptrfunc ptrfuncion = &I_Control::interrupt_2;
        codigo_en_hilo(ptrfuncion);
    }

}

void KheperaSimGUI::on_interrupt3_clicked()
{
    simxFinish(clientIDexe);
    if (conectar(ip,puerto) != -1) {
        ptrfunc ptrfuncion = &I_Control::interrupt_3;
        codigo_en_hilo(ptrfuncion);
    }
}
void KheperaSimGUI::on_interrupt4_clicked()
{
    simxFinish(clientIDexe);
    if (conectar(ip,puerto) != -1) {
        ptrfunc ptrfuncion = &I_Control::interrupt_4;
        codigo_en_hilo(ptrfuncion);
    }
}
//código de evento: selección de nombre de robot para mostrar sus datos.
void KheperaSimGUI::on_comboBox_2_activated(QString robottexto)
{   //Si se selecciona Personalizado, abre un dialogo para introducir un nuevo nombre.
    
    if (!robottexto.compare("Personalizado...")){
        QString nombrerobot = QInputDialog::getText(this,tr("Nombre Nuevo"),tr("Nombre robot:"));
        //Comprueba que no se canceló el dialogo, añade la nueva opción a la lista y la selecciona.
        if (nombrerobot!=NULL) {
          ui->comboBox_2->insertItem(0,nombrerobot);
          ui->comboBox_2->setCurrentIndex(0);
        }
    } else if (!robottexto.compare("Añadir")){
        std::ostringstream sstr;
        sstr << "K3_robot#" << ui->comboBox_2->count() -3;
        QString str = QString::fromStdString(sstr.str());
        ui->comboBox_2->insertItem(ui->comboBox_2->count()-2,str);
        ui->comboBox_2->setCurrentIndex(ui->comboBox_2->count()-3);
    }
}
//cambia los nombres de los botones de interrupcion al modo Control.
void KheperaSimGUI::on_controlset_clicked()
{
    ui->interrupt1->setText("Interrupt 1");
    ui->interrupt2->setText("Interrupt 2");
    ui->interrupt3->setText("Interrupt 3");
    ui->interrupt4->setText("Interrupt 4");
    ui->comboBox_3->setEnabled(false);
    ui->label_13->setEnabled(false);
    ui->comboBox->setEnabled(false);
}
//revierte los botones de interrupcion al modo demo.
void KheperaSimGUI::on_demoset_clicked()
{
    ui->interrupt1->setText("Adelante");
    ui->interrupt2->setText("Atrás");
    ui->interrupt3->setText("Izquierda");
    ui->interrupt4->setText("Derecha");
    ui->comboBox_3->setEnabled(true);
    ui->label_13->setEnabled(true);
    ui->comboBox->setEnabled(true);
}
//permite cambiar el robot al que se envian las interrupciones. Solo modo demo.(En modo control el usuario puede elegir con su código cualquier robot/acción)
void KheperaSimGUI::on_comboBox_3_activated(const QString a)
{   string texto = a.toUtf8().constData();
    if (!texto.compare("Añadir")){
        std::ostringstream sstr;
        sstr << "K3_robot#" << ui->comboBox_3->count() -2;
        QString str = QString::fromStdString(sstr.str());
        ui->comboBox_3->insertItem(ui->comboBox_3->count()-1,str);
        ui->comboBox_3->setCurrentIndex(ui->comboBox_3->count()-2);
    }
    demo.setRobot(ui->comboBox_3->currentIndex());
}

//permite cambiar la velocidad de los motores en las interrupciones. Solo modo Demo. Advierte que el robot se desestabiliza para altas velocidades.
void KheperaSimGUI::on_comboBox_activated(int index)
{   //comprueba velocidad > 2x y que no se ha especificado que no se muestre la advertencia.
    if (index>1 && !nomostrarwarning){
        QMessageBox warning;
        QCheckBox nomostrar;
        nomostrar.setText("No volver a mostrar");
        warning.setText("Para V > 2x, el control no asegura la estabilidad del robot");
        warning.setCheckBox(&nomostrar);
        warning.exec();
        if (warning.checkBox()->isChecked()){
            nomostrarwarning = true;
        }
        }
    demo.setVelocidad(index+1);
}
//codigo de cierre por menú
void KheperaSimGUI::on_actionSalir_triggered()
{
    fclose(log);
    exit(0);
}
//muestra el output (hace la ventana mas grande) o lo oculta (ventana mas pequeña)
void KheperaSimGUI::on_actionMostrar_Output_toggled(bool arg1)
{
    if (arg1) {
         setFixedSize(450,410);
    } else {
         setFixedSize(450,275);
    }
}
//código de cierre por pulsar X en ventana.
void KheperaSimGUI::closeEvent(QCloseEvent *bar) {

    fclose(log);

}

void KheperaSimGUI::on_actionAyuda_triggered()
{
    QDesktopServices::openUrl(QUrl("http://xpeiro.github.io/compu3/"));
}
