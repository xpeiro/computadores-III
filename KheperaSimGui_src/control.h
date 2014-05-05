//Definición de Clase Plantilla de Código de Control (puede ser cualquiera con tal de que implemente I_control)
#ifndef CONTROL_H
#define CONTROL_H
#include <i_control.h>

//La clase implementa la "interfaz" (clase abstracta) i_control
class Control: public I_Control
{
    public:
       int control(int clientID);
       int interrupt_1(int clientID);
       int interrupt_2(int clientID);
       int interrupt_3(int clientID);
       int interrupt_4(int clientID);
};

#endif // CONTROL_H
