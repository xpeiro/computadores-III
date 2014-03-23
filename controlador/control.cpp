//Clase Plantilla de Código de Control (puede ser cualquiera con tal de que implemente I_control)

#include "i_control.h"
#include <iostream>
extern "C" {
#include "extApi.h"
}

using namespace std;

//La clase implementa la "interfaz" (clase abstracta) i_control
class Control: public I_Control
{	public:
  	   int control(int clientID) {

  	   		//LLAMADA DESDE CONTROLADOR:

	  	   	return 0;	  	   
  		}
};