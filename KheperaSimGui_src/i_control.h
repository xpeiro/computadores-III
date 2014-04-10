//"Interfaz" (clase abstracta) que debe implementar la clase de código cliente
//Ver clase ejemplo en Demo.
#ifndef I_CONTROL_H
#define I_CONTROL_H




class I_Control
{
    public:
       virtual int control(int clientID) = 0; //Método virtual puro, debe sobrecargarse con el contenido de Control.
       virtual int interrupt_1(int clientID) = 0; //Métodos virtuales puros, debe sobrecargarse con el contenido de interrupciones.
       virtual int interrupt_2(int clientID) = 0; //(normalmente adelante,atras,izq y der, respectivamente).
       virtual int interrupt_3(int clientID) = 0;
       virtual int interrupt_4(int clientID) = 0;
};

#endif
