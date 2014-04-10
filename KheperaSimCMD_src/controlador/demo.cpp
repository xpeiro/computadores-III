//Clase Ejemplo de Código Usuario

#include "i_control.h"
#include <iostream>
extern "C" {
#include "extApi.h"
}

using namespace std;

//La clase implementa la "interfaz" (clase abstracta) i_control
class Demo: public I_Control
{	public:
  	   int control(int clientID) {
  	   	cout << "Demo: Inicio de Demo con dos Kheperas. La escena debe ser demo2khepera.ttt\n";
		int leftMotorHandle;
		int rightMotorHandle;	
		int leftMotorHandle0;
		int rightMotorHandle0;
		int robotHandle;
		int robotHandle0;
		int errorcode;		
		int i=0;
		float motorSpeed[2];
		float position[3];
		
		//valores para velocidades.
		motorSpeed[0]=3.1415;
		motorSpeed[1]=3.1415;

		//genera un handle para los motores de khepera y khepera#0 y de los objetos robot en sí.
		simxGetObjectHandle(clientID,"K3_rightWheelMotor",&rightMotorHandle,simx_opmode_oneshot_wait);
		simxGetObjectHandle(clientID,"K3_leftWheelMotor",&leftMotorHandle,simx_opmode_oneshot_wait);
		simxGetObjectHandle(clientID,"K3_rightWheelMotor#0",&rightMotorHandle0,simx_opmode_oneshot_wait);
		simxGetObjectHandle(clientID,"K3_leftWheelMotor#0",&leftMotorHandle0,simx_opmode_oneshot_wait);
		simxGetObjectHandle(clientID,"K3_robot",&robotHandle,simx_opmode_oneshot_wait);
		simxGetObjectHandle(clientID,"K3_robot#0",&robotHandle0,simx_opmode_oneshot_wait);
		
				
			while (simxGetConnectionId(clientID)!=-1)
			{	
				//mover khepera
				errorcode=simxSetJointTargetVelocity(clientID,leftMotorHandle,2*motorSpeed[0],simx_opmode_oneshot);
				errorcode=simxSetJointTargetVelocity(clientID,rightMotorHandle,2*motorSpeed[1],simx_opmode_oneshot);	

				//mover khepera#0			
				errorcode=simxSetJointTargetVelocity(clientID,leftMotorHandle0,motorSpeed[0],simx_opmode_oneshot);								
				errorcode=simxSetJointTargetVelocity(clientID,rightMotorHandle0,motorSpeed[1],simx_opmode_oneshot);	

				//Muestra en stdout la coordenada X de ambos robots. Se reduce el muestreo para mayor comodidad de visualizacion.
				
				if (i==100000)
				{
				cout << string(50,'\n');
				simxGetObjectPosition(clientID,robotHandle,-1,position,simx_opmode_oneshot);
				cout << "Posicion K3_robot:\n" << "\nX:" << position[0] << "\nY:" << position[1] <<"\nZ:" << position[2] << "\n";
				simxGetObjectPosition(clientID,robotHandle0,-1,position,simx_opmode_oneshot);				
				cout << "\nPosicion K3_robot#0:\n" << "\nX#0:" << position[0] << "\nY#0:" << position[1] <<"\nZ#0:" << position[2] << "\n";
							
				i=0;	
				}
				i++;


			}
			cout << "Demo: La conexión ha sido cerrada\n";
	  	   	return 0;
	  	   
  		}
};