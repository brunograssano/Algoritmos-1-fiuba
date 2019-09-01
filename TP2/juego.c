#include "copa.h"
#include <stdio.h>
#include "laberinto.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#define EN_CURSO 0
#define GANADO 1
#define PERDIDO -1
#define ESFINGE 'F'
#define IMPEDIMENTA 'I'
#define RIDDIKULUS 'R'
#define VIDA_PARA_COPA 15

/*precondiciones: Recibe el juego con todos los datos ya cargados.
 *postcondiciones: Le pide una tecla al usuario hasta que ingrese una correcta, ahi la guarda y devuelve por referencia
 */
void leer_movimiento(juego_t* juego,char* movimiento){

	printf("Ingrese su movimiento (a-Izquierda d-Derecha s-Abajo w-Arriba) \n");
	scanf(" %c",movimiento);

	while(!es_movimiento_valido(juego,*movimiento)){
		printf("Ingreso un movimiento que no es valido, ingrese uno correcto. (a-Izquierda d-Derecha s-Abajo w-Arriba) \n");
		scanf(" %c",movimiento);
	}
}

/*precondiciones: Recibe el jugador ya con todos los datos cargados correctamente
 *postcondiciones: Muestra esos datos
 */
void mostrar_datos_jugador(jugador_t jugador){
	
	printf("Sus puntos de vida son %i \n", jugador.vida);

	if(jugador.vida<=VIDA_PARA_COPA){
		printf("Le queda poca vida, puede ver la copa. \n");
	}

	if(jugador.tope_ayudas>0){
		printf("Agarro una o mas ayudas. \n");
		for(int i=0;i<jugador.tope_ayudas;i++){
			if(jugador.ayudas[i].codigo==ESFINGE){
				printf("Tiene la Esfinge (F), puede ver la copa.\n");
			}
			else if(jugador.ayudas[i].codigo==IMPEDIMENTA){
				printf("Tiene Impedimenta, no le hace danio el Escreguto de cola explosiva. (E)\n");
			}
			else if(jugador.ayudas[i].codigo==RIDDIKULUS){
				printf("Tiene Riddikulus, no le hace danio el Boggart. (B)\n");
			}
		}
	}
	else{
		printf("No tiene ayudas actualmente.\n");
	}
}

/*precondiciones: El juego ya tiene que haber terminado
 *postcondiciones: Mostrara el resultado
 */
void mostrar_estado_juego(juego_t juego){
	if (estado_juego(juego)==GANADO){
		printf("Felicitaciones, ha ganado el juego! \n");
	}
	else if(estado_juego(juego)==PERDIDO){
		printf("Ha perdido el juego :( \n");
	}
}

/* 
 * Muestra informacion varia del juego con respecto a los identificadores y su tipo
 */
void mostrar_informacion_juego(){
	printf("Usted es el jugador, representado con la 'J', tiene que encontrar la copa 'C' dentro del laberinto antes de que el rival 'G' lo haga.\n");
	printf("Pero ten cuidado! Cada vez que te mueves pierdes 3 de vida, por lo que piensa bien tus movimientos!\n");
	printf("Ademas ten en cuenta que hay criaturas que te pueden hacer danio, estos son: el Escreguto 'E', la Acromantula 'A', y el Boggart 'B'. \n");
	printf("Puedes evitarlas con las siguientes ayudas, el Riddikulus'R' que te proteje del Boggart, y el Impedimenta 'I' que te proteje del Escreguto.\n");
	printf("Otra cosa a tener en cuenta, es que no puedes ver donde esta la copa a menos que agarres la Esfinge 'F' o tengas 15 o menos de vida. \n");
	printf("Recuerda tambien que pudes recuperar vida agarrando las pociones 'P', estas te dan 15 puntos de vida.\n");
	printf("Buena suerte! :)\n");
}

/*
 * El juego iniciara,transcurrira y terminara de forma correcta
 */
int main(){

	juego_t juego;
	char movimiento;
	char laberinto[TAMANIO][TAMANIO];

	inicializar_laberinto(&juego);

	while(estado_juego(juego)==EN_CURSO){

		mostrar_informacion_juego();

		mostrar_datos_jugador(juego.jugador);

		actualizar_laberinto(juego,laberinto);

		mostrar_laberinto(laberinto);
		
		leer_movimiento(&juego,&movimiento);

		mover_jugador(&(juego.jugador),movimiento);

		mover_rival(&juego);

		actualizar_juego(&juego);

		system("clear");
	}

	mostrar_estado_juego(juego);

	return 0;
}