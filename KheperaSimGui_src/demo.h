//Definicion Clase Ejemplo de Código Usuario.
#ifndef DEMO_H
#define DEMO_H
#include <i_control.h>

//La clase implementa la "interfaz" (clase abstracta) i_control
class Demo: public I_Control
{
    public:
       int control(int clientID);
       int interrupt_1(int clientID);
       int interrupt_2(int clientID);
       int interrupt_3(int clientID);
       int interrupt_4(int clientID);
       //establece el nombre del robot a controlar con las interrupciones demo.
       void setRobot (int indice_robot);
       //establece la velocidad de los motores en las interrupciones demo.
       void setVelocidad (int multiplicador);
    private:
       std::string leftmotor = "K3_leftWheelMotor#";
       std::string rightmotor = "K3_rightWheelMotor#";
       const float pi=3.141592;
       float velocidad = pi;
};

#endif // DEMO_H
