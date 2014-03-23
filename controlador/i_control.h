//"Interfaz" (clase abstracta) que debe implementar la clase de código cliente
#ifndef I_CONTROL_H
#define I_CONTROL_H


using namespace std;

class I_Control
{
    public:
       virtual int control(int clientID) = 0; //Método virtual puro, debe sobrecargarse con el contenido de Control.
};

#endif