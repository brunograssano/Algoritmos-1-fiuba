
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "funciones_azkaban.h"

const int PARAMETRO_MIN=1,ACCION=1,NOMBRE=2,FECHA=3,CONDUCTA=4;
#define FALLO -1
#define PERDONABLES "perdonables"
#define LIBERAR "liberar"
#define ACTUALIZAR "actualizar"
#define MOSTRAR_LIBERADOS "mostrar_liberados"

/*
 * Mostrara el estado de la operacion realizada, el estado recibido es un entero. En caso de que sea que hubo un error le mostrara ayudas.
 */
void mostrar_estado(int estado){
	if(estado==FALLO){
		printf("Algo salio mal, ha habido un error. Quizas escribio mal el comando. Aqui puede ver unas ayudas \n");
		mostrar_ayuda();
	}
	else{
		printf("Accion realizada correctamente \n");
	}
}

/*
 * Revisa que supere la cantidad inicial de parametros.
 */
bool hay_parametro(int cantidad_parametros){
	return cantidad_parametros>PARAMETRO_MIN;
}

/*
 * Verifica que el usuario haya escrito algun parametro, en caso de que no, mostrara ayuda.
 */
void verificar_hay_parametro(int cantidad_parametros){
	if(!hay_parametro(cantidad_parametros)){
		printf("No escribio ningun parametro.\n");
		mostrar_ayuda();
	}	
}

/*
 * Comprueba si los comandos ingresados son iguales.
 */
bool es_comando(char comando_ingresado[],char comando_posible[]){
	return (strcmp(comando_ingresado,comando_posible)==0);
}

/*
 * Ejecutara el comando deseado en caso de que haya escrito alguno, en caso de que se haya equivocado se le mostrara ayuda
 */
void realizar_comando(int cantidad_parametros,char* parametro[]){
	
	if(hay_parametro(cantidad_parametros)){
		if(es_comando(parametro[ACCION],PERDONABLES)){
			int estado = perdonables(parametro[NOMBRE]);
			mostrar_estado(estado);
		}
		else if(es_comando(parametro[ACCION],LIBERAR)){
			int estado = liberar(parametro[NOMBRE],parametro[FECHA],parametro[CONDUCTA]);
			mostrar_estado(estado);
		}
		else if(es_comando(parametro[ACCION],ACTUALIZAR)){
			int estado = actualizar(parametro[NOMBRE]);
			mostrar_estado(estado);		
		}
		else if(es_comando(parametro[ACCION],MOSTRAR_LIBERADOS)){
			mostrar_liberados(parametro[NOMBRE]);
		}
		else{
			mostrar_ayuda();
		}
	}

}

/*
 * Si el programa recibe correctamente los argumentos, realizara la accion correspondiente.
 * En caso de que algo salga mal, se le avisara al usuario y se le podra mostrar ayuda.
 */
int main(int argc,char *argv[]){

	verificar_hay_parametro(argc);

	realizar_comando(argc,argv);
	
	return 0;
}