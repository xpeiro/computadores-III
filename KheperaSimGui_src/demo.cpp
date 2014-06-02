//Clase Ejemplo de Código Usuario. Ver Documentación de Remote Api para entender funciones.
#include <demo.h>
#include <sstream>
#include <math.h>
extern "C" {
#include "extApi.h"
}

using namespace std;

int Demo::control(int clientID) {
    cout << "Demo: Inicio de Control\n";
    int leftMotorHandle;
    int rightMotorHandle;
    int leftMotorHandle0;
    int rightMotorHandle0;
    int error[4]={0,0,0,0};
    int i=0;

    //genera un handle para los motores de khepera y khepera#0 y de los objetos robot en sí.
    do {
	    error[0]= simxGetObjectHandle(clientID,"K3_rightWheelMotor#",&rightMotorHandle,simx_opmode_oneshot_wait);
	    error[1]= simxGetObjectHandle(clientID,"K3_leftWheelMotor#",&leftMotorHandle,simx_opmode_oneshot_wait);
	    error[2]= simxGetObjectHandle(clientID,"K3_rightWheelMotor#0",&rightMotorHandle0,simx_opmode_oneshot_wait);
	    error[3]= simxGetObjectHandle(clientID,"K3_leftWheelMotor#0",&leftMotorHandle0,simx_opmode_oneshot_wait);
        i++;
        cout << "Código de error: " << error[0] << error[1] << error[2] << error[3] << "\n";
    } while ((error[0]!=0 || error[1]!=0 || error[2]!=0 || error[3]!=0) && i < 5);
    


    while (simxGetConnectionId(clientID)!=-1)
    {
        //mover khepera
        simxSetJointTargetVelocity(clientID,leftMotorHandle,2*pi,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorHandle,2*pi,simx_opmode_oneshot);

        //mover khepera#0
        simxSetJointTargetVelocity(clientID,leftMotorHandle0,pi,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorHandle0,pi,simx_opmode_oneshot);

    }
    cout << "Demo: Control abortado\n";
    return 0;

}
//adelante
int Demo::interrupt_1(int clientID){
    int leftMotorHandle;
    int rightMotorHandle;
    int error[2] = {0,0};
    int i =0;
    float velocidad = this->velocidad;

    cout << "Demo: Adelante v: " << velocidad << "\n";
    do {
        error[0] = simxGetObjectHandle(clientID,rightmotor.c_str(),&rightMotorHandle,simx_opmode_oneshot_wait);
        error[1] = simxGetObjectHandle(clientID,leftmotor.c_str(),&leftMotorHandle,simx_opmode_oneshot_wait);
        i++;
    } while ((error[0]!=0 || error[1]!=0) && i < 5);

    while (simxGetConnectionId(clientID)!=-1) {
        simxSetJointTargetVelocity(clientID,leftMotorHandle,velocidad,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorHandle,velocidad,simx_opmode_oneshot);
    }
    return 0;
}
//marcha atrás
int Demo::interrupt_2(int clientID){
    int leftMotorHandle;
    int rightMotorHandle;
    int error[2] = {0,0};
    int i =0;
    float velocidad = this->velocidad;

    cout << "Demo: Marcha atrás v: " << velocidad << "\n";
    do {
        error[0] = simxGetObjectHandle(clientID,rightmotor.c_str(),&rightMotorHandle,simx_opmode_oneshot_wait);
        error[1] = simxGetObjectHandle(clientID,leftmotor.c_str(),&leftMotorHandle,simx_opmode_oneshot_wait);
        i++;
    } while ((error[0]!=0 || error[1]!=0) && i < 5);

    while (simxGetConnectionId(clientID)!=-1) {
        simxSetJointTargetVelocity(clientID,leftMotorHandle,-velocidad,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorHandle,-velocidad,simx_opmode_oneshot);
    }
    return 0;
}
//giro izquierda
int Demo::interrupt_3(int clientID){
    int i=0;
    int leftMotorHandle;
    int rightMotorHandle;
    float angulos[3]={0,0,0};
    float alfa =0;
    float anterior=0;
    float angulo=0;
    float velocidad = this->velocidad;
    int error[2] = {0,0};

    cout << "Demo: Giro Izq. v: " << velocidad << "\n";
    do {
        error[0] = simxGetObjectHandle(clientID,rightmotor.c_str(),&rightMotorHandle,simx_opmode_oneshot_wait);
        error[1] = simxGetObjectHandle(clientID,leftmotor.c_str(),&leftMotorHandle,simx_opmode_oneshot_wait);
        i++;
    } while ((error[0]!=0 || error[1]!=0) && i < 5);
    i=0;
    //para girar 90º, obtiene la orientación (angulos de euler con respecto a los ejes absolutos).
    simxGetObjectOrientation(clientID,leftMotorHandle,-1,angulos,simx_opmode_oneshot_wait);
    //alfa es el angulo beta inicial.
    alfa=angulos[1];
    //anterior es necesario para calcular el incremento de angulo (debido al sistema angular de vrep: 0->90->0->-90->0)
    anterior=alfa;
    //angulo es el angulo actual en coordenadas radiales.
    angulo=alfa;
    while (simxGetConnectionId(clientID)!=-1) {
        //calculo angulo nuevo con la diferencia del dado por vrep y el anterior dado por vrep (incremento).
        simxGetObjectOrientation(clientID,leftMotorHandle,-1,angulos,simx_opmode_oneshot);
        angulo= angulo+fabs(angulos[1]-anterior);
        //si el angulo actual está mas allá de 90º del inicial, giro completo. Si no, sigue girando.
        if ( angulo<alfa+pi/2 ) {
            simxSetJointTargetVelocity(clientID,leftMotorHandle,0,simx_opmode_oneshot);
            simxSetJointTargetVelocity(clientID,rightMotorHandle,velocidad,simx_opmode_oneshot);

        } else {
            simxSetJointTargetVelocity(clientID,leftMotorHandle,velocidad,simx_opmode_oneshot);
            simxSetJointTargetVelocity(clientID,rightMotorHandle,velocidad,simx_opmode_oneshot);
        }
        anterior=angulos[1];
    }
    return 0;
}
//giro derecha (ídem giro izquierda).
int Demo::interrupt_4(int clientID){
    int i=0;
    int leftMotorHandle;
    int rightMotorHandle;
    int error[2] = {0,0};
    float angulos[3]={0,0,0};
    float alfa =0;
    float anterior=0;
    float angulo=0;
    float velocidad = this->velocidad;

    cout << "Demo: Giro Der. v: " << velocidad << "\n";
    do {
        error[0] = simxGetObjectHandle(clientID,rightmotor.c_str(),&rightMotorHandle,simx_opmode_oneshot_wait);
        error[1] = simxGetObjectHandle(clientID,leftmotor.c_str(),&leftMotorHandle,simx_opmode_oneshot_wait);
        i++;
    } while ((error[0]!=0 || error[1]!=0) && i < 5);
    //para girar a la
    simxGetObjectOrientation(clientID,rightMotorHandle,-1,angulos,simx_opmode_oneshot_wait);
    alfa=angulos[1];
    anterior=alfa;
    angulo=alfa;
    while (simxGetConnectionId(clientID)!=-1) {
        simxGetObjectOrientation(clientID,leftMotorHandle,-1,angulos,simx_opmode_oneshot);
        angulo= angulo-fabs(angulos[1]-anterior);
        if (angulo>alfa-pi/2) {
            simxSetJointTargetVelocity(clientID,leftMotorHandle,velocidad,simx_opmode_oneshot);
            simxSetJointTargetVelocity(clientID,rightMotorHandle,0,simx_opmode_oneshot);

        } else {
            simxSetJointTargetVelocity(clientID,leftMotorHandle,velocidad,simx_opmode_oneshot);
            simxSetJointTargetVelocity(clientID,rightMotorHandle,velocidad,simx_opmode_oneshot);
        }
        anterior=angulos[1];
    }
    return 0;
}
//adjunta el índice del robot al label de vrep, para identificar que robot "interrumpir".
//permitir robots con nombre personalizado es posible, pero resulta mas trabajoso de lo que reporta al usuario, por tanto no se incluye en la demo.
void Demo::setRobot(int indice_robot) {

    if (indice_robot>0) {
        std::ostringstream sstr;
        sstr << "K3_leftWheelMotor#" << indice_robot -1;
        leftmotor = sstr.str();
        sstr.str("");
        sstr.clear();
        sstr << "K3_rightWheelMotor#" << indice_robot -1;
        rightmotor = sstr.str();
    } else {
        leftmotor = "K3_leftWheelMotor#";
        rightmotor = "K3_rightWheelMotor#";
    }

}
//establece la velocidad (multiplo de pi) con el argumento dado.
void Demo::setVelocidad(int multiplicador) {
    velocidad=multiplicador*pi;
}






