
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "copa.h"
#include "laberinto.h"

const char JUGADOR='J',RIVAL='G';
const char COPA='C',POCION='P',IMPEDIMENTA='I',RIDDIKULUS='R',ESCREGUTO='E',ACROMANTULA='A',BOGGART='B',ESFINGE='F';
const int MOVIMIENTO=1,MINIMA_DISTANCIA=10;
const int DANIO_ESCREGUTO=20,DANIO_ACROMANUTLA=10,DANIO_BOGGART=15,VIDA_POCION=15,VIDA_JUGADOR=50,VIDA_MOVIMIENTO=3,NO_RECUPERA=0;
const int VIDA_PARA_COPA=15;
const int EN_CURSO=0,GANADO=1,PERDIDO=-1;
const int MAXIMA_CANTIDAD_MOVIMIENTOS=4;
#define DERECHA 'd'
#define IZQUIERDA 'a'
#define ABAJO 's'
#define ARRIBA 'w'
#define CANT_INICIAL_AYUDAS 6
#define CANT_INICIAL_OBSTACULOS 3

/*precondiciones: Recibe los datos del obstaculo, estos son correctos
 *postcondiciones: El obstaculo queda cargado con una posicion aleatoria que no es validada
 */
void iniciar_obstaculo(obstaculo_t* obstaculo,char codigo,int danio){
	obstaculo->codigo=codigo;
	obstaculo->danio=danio;
	obstaculo->posicion=posicion_aleatoria();
}

/*precondiciones: Recibe los datos de la ayuda, estos son correctos
 *postcondiciones: La ayuda queda cargada con una posicion aleatoria que no es validada
 */
void iniciar_ayuda(ayuda_t* ayuda,char codigo,int vida){
	ayuda->codigo=codigo;
	ayuda->vida_a_recuperar=vida;
	ayuda->posicion=posicion_aleatoria();
}

/*precondiciones: El laberinto que recibe esta con objetos cargados, este tiene lugar vacio
 *postcondiciones: Indica si puede poner el objeto en el lugar
 */
bool hay_objeto(char laberinto[TAMANIO][TAMANIO],coordenada_t objeto){
	bool hay_objeto=true;
	if(laberinto[objeto.fil][objeto.col]==VACIO){
		hay_objeto=false;
	}
	return hay_objeto;
}

/*
 *	Calcula la distancia Manhattan respecto del jugador o rival a la copa
 */
int distancia_a_copa(coordenada_t copa,coordenada_t participante){

	int filas=(copa.fil)-(participante.fil);
	int columnas=(copa.col)-(participante.col);
	int distancia=abs(filas)+abs(columnas);

	return distancia;
}

/*precondiciones: Recibe la copa cumpliendo su tipo de dato
 *postcondiciones: La copa es cargada con sus datos, la posicion queda validada
 */
void inicializar_copa(copa_t* copa,char laberinto_temporal[TAMANIO][TAMANIO]){
	copa->codigo=COPA;
	do{
		copa->posicion=posicion_aleatoria();
	}while(hay_objeto(laberinto_temporal,copa->posicion));

	laberinto_temporal[copa->posicion.fil][copa->posicion.col]=copa->codigo;
}

/*precondiciones: La cantidad total de obstaculos a crear no se pasara de la inicial ya puesta, ya que si no, se pasara de los dos vectores con datos
 *postcondiciones: Estara la cantidad esperada con sus datos
 */
void inicializar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS],char laberinto_temporal[TAMANIO][TAMANIO],int* tope_obstaculos){

	char nombres_obstaculos[CANT_INICIAL_OBSTACULOS]={ESCREGUTO,ACROMANTULA,BOGGART};
	int danio_obstaculos[CANT_INICIAL_OBSTACULOS]={DANIO_ESCREGUTO,DANIO_ACROMANUTLA,DANIO_BOGGART};

	(*tope_obstaculos)=0;

	for(int i=0;i<CANT_INICIAL_OBSTACULOS;i++){
		
		do{
			iniciar_obstaculo(&(obstaculos[i]),nombres_obstaculos[i],danio_obstaculos[i]);
		}while(hay_objeto(laberinto_temporal,obstaculos[i].posicion));

		(*tope_obstaculos)++;

		laberinto_temporal[obstaculos[i].posicion.fil][obstaculos[i].posicion.col]=obstaculos[i].codigo;
	}
}

/*precondiciones: La cantidad total de ayudas a crear no se pasara de la inicial ya puesta
 *postcondiciones: Estara la cantidad esperada de ayudas
 */
void inicializar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS],char laberinto_temporal[TAMANIO][TAMANIO],int* tope_ayudas){

	char nombres_ayudas[CANT_INICIAL_AYUDAS]={IMPEDIMENTA,RIDDIKULUS,POCION,POCION,POCION,ESFINGE};
	int vida_ayudas[CANT_INICIAL_AYUDAS]={NO_RECUPERA,NO_RECUPERA,VIDA_POCION,VIDA_POCION,VIDA_POCION,NO_RECUPERA};

	(*tope_ayudas)=0;

	for(int i=0;i<CANT_INICIAL_AYUDAS;i++){
		
		do{
			iniciar_ayuda(&(ayudas[i]),nombres_ayudas[i],vida_ayudas[i]);
		}while(hay_objeto(laberinto_temporal,ayudas[i].posicion));

		(*tope_ayudas)++;

		laberinto_temporal[ayudas[i].posicion.fil][ayudas[i].posicion.col]=ayudas[i].codigo;
	}
}

/*precondiciones: El laberinto para verificar la posicion esta cargado, tambien la posicion de la copa ya es valida
 *postcondiciones: El rival queda ubicado
 */
void inicializar_rival(rival_t* rival,char laberinto_temporal[TAMANIO][TAMANIO],coordenada_t posicion_copa){
	rival->codigo=RIVAL;
	rival->direccion=DERECHA;
	rival->cantidad_pasos=0;
	do{
		rival->posicion=posicion_aleatoria();
	}while(hay_objeto(laberinto_temporal,rival->posicion) || distancia_a_copa(posicion_copa,rival->posicion)<=MINIMA_DISTANCIA);

	laberinto_temporal[rival->posicion.fil][rival->posicion.col]=rival->codigo;
}

/*precondiciones: El laberinto para verificar la posicion esta cargado, tambien la posicion de la copa ya es valida
 *postcondiciones: El jugador queda iniciado y a una distancia mayor a 10 de la copa
 */
void inicializar_jugador(jugador_t* jugador,char laberinto_temporal[TAMANIO][TAMANIO],coordenada_t posicion_copa){
	jugador->codigo=JUGADOR;
	jugador->vida=VIDA_JUGADOR;
	jugador->tope_ayudas=0;
	do{
		jugador->posicion=posicion_aleatoria();
	}while(hay_objeto(laberinto_temporal,jugador->posicion) || distancia_a_copa(posicion_copa,jugador->posicion)<=MINIMA_DISTANCIA);

}

/*
 * Copiara el laberinto original en otro, que sera mostrado o para control
 */
void copiar_laberinto(char laberinto_temporal[TAMANIO][TAMANIO],char laberinto_original[TAMANIO][TAMANIO]){
	for(int i=0;i<TAMANIO;i++){
		for (int j=0;j<TAMANIO;j++){
			laberinto_temporal[i][j]=laberinto_original[i][j];
		}
	}
}

/*
 * Inicializará todas las estructuras con los valores correspondientes,
 * creará el laberinto, posicionará todos los elementos, etc.
 */
void inicializar_laberinto(juego_t* juego){

	srand((unsigned int)time(NULL));

	inicializar_paredes_laberinto(juego->laberinto_original);

	char laberinto_temporal[TAMANIO][TAMANIO];

	copiar_laberinto(laberinto_temporal,juego->laberinto_original);

	inicializar_copa(&(juego->copa),laberinto_temporal);

	inicializar_obstaculos(juego->obstaculos,laberinto_temporal,&(juego->tope_obstaculos));

	inicializar_ayudas(juego->ayudas,laberinto_temporal,&(juego->tope_ayudas));

	inicializar_rival(&(juego->rival),laberinto_temporal,juego->copa.posicion);

	inicializar_jugador(&(juego->jugador),laberinto_temporal,juego->copa.posicion);	
}

/*
 *Verifica que reciba una tecla valida
 */
bool es_caracter_valido(char tecla){
	return (tecla==IZQUIERDA || tecla==DERECHA || tecla==ABAJO || tecla==ARRIBA);
}

/*
 Obtiene las coordeneas a las que se moveria el jugador o rival.
 La tecla ya viene valida y devuelve unas coordenadas nuevas.
 */
coordenada_t nuevas_coordenadas(coordenada_t participante, char tecla){
	coordenada_t movimiento_posible;
	switch(tecla){
		case IZQUIERDA:
			movimiento_posible.fil=participante.fil;
			movimiento_posible.col=participante.col-MOVIMIENTO;
			break;
		case DERECHA:
			movimiento_posible.fil=participante.fil;
			movimiento_posible.col=participante.col+MOVIMIENTO;
			break;
		case ARRIBA:
			movimiento_posible.col=participante.col;
			movimiento_posible.fil=participante.fil-MOVIMIENTO;
			break;
		case ABAJO:
			movimiento_posible.col=participante.col;
			movimiento_posible.fil=participante.fil+MOVIMIENTO;
			break;	
	}

	return movimiento_posible;
}
/*
 * Determina si las coordenadas mandadas estan dentro de la matriz que representa al laberinto.
 */
bool en_limites_laberinto(coordenada_t movimiento){
	return ((0<=movimiento.fil && movimiento.fil<TAMANIO) && (0<=movimiento.col && movimiento.col<TAMANIO));
}

/*
 * Determinará si el movimiento que se quiere hacer cae en una pared, para eso, se fija cual seria el lugar a donde caeria con el movimiento.
 * Si este movimiento cae afuera de los limites del laberinto o en algun lugar que tiene pared, devuelve que no hay espacio para moverse.
 */
bool hay_pasillo(juego_t juego, char tecla){

	bool hay_pasillo=true;

	coordenada_t movimiento_posible=nuevas_coordenadas(juego.jugador.posicion,tecla);

	if((juego.laberinto_original[movimiento_posible.fil][movimiento_posible.col])==PARED || !en_limites_laberinto(movimiento_posible)){
		hay_pasillo=false;
	}

	return hay_pasillo;
}

/*
 * Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o 
 * ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea, 
 * hay pasillo.
 */
bool es_movimiento_valido(juego_t* juego, char tecla){
	bool se_puede_mover=false;
	if(es_caracter_valido(tecla)){
		if(hay_pasillo(*juego,tecla)){
			se_puede_mover=true;
		}
	}
	return se_puede_mover;
}

/*
 * Moverá el jugador hacia la dirección especificada.
 * Dicho movimiento es válido.
 */
void mover_jugador(jugador_t* jugador, char direccion){
	jugador->posicion=nuevas_coordenadas(jugador->posicion,direccion);
}

/* precondiciones: Cuando es llamada la funcion, el rival no se movio todavia, si no, no se movera.
 * Tiene que recibir tambien una direccion alternativa valida en caso de que no pueda con la original
 * postcondiciones: El rival se movera o cambiara de direccion para volver a probar devuelta en otro llamado a la funcion
 */
void intentar_mover_rival(rival_t* rival,char direccion_alternativa,bool* se_movio,char laberinto[TAMANIO][TAMANIO]){
	int fallo_movimiento=0;
	while(!(*se_movio) && fallo_movimiento<MAXIMA_CANTIDAD_MOVIMIENTOS){

		coordenada_t movimiento_posible=nuevas_coordenadas(rival->posicion,rival->direccion);

		if((laberinto[movimiento_posible.fil][movimiento_posible.col])!=PARED && en_limites_laberinto(movimiento_posible)){
			(*se_movio)=true;
			(rival->cantidad_pasos)++;
			rival->posicion=movimiento_posible;
			
			if((rival->cantidad_pasos)%MAXIMA_CANTIDAD_MOVIMIENTOS==0){
				rival->direccion=direccion_alternativa;
			}
			
		}
		else{
			fallo_movimiento++;
		}

	}
	if(fallo_movimiento==MAXIMA_CANTIDAD_MOVIMIENTOS){
		rival->direccion=direccion_alternativa;
		rival->cantidad_pasos=0;
	}
}

/*
 * Moverá el rival a la próxima posición.
 */
void mover_rival(juego_t* juego){
	bool se_movio=false;
	while(!se_movio){
		if(juego->rival.direccion==DERECHA){
			intentar_mover_rival(&(juego->rival),ABAJO,&se_movio,juego->laberinto_original);
		}
		else if(juego->rival.direccion==ABAJO){
			intentar_mover_rival(&(juego->rival),IZQUIERDA,&se_movio,juego->laberinto_original);
		}
		else if(juego->rival.direccion==IZQUIERDA){
			intentar_mover_rival(&(juego->rival),ARRIBA,&se_movio,juego->laberinto_original);
		}
		else if(juego->rival.direccion==ARRIBA){
			intentar_mover_rival(&(juego->rival),DERECHA,&se_movio,juego->laberinto_original);
		}
	}
}

/* 
 * Busca si tiene la ayuda necesaria
 */
bool tiene_ayuda(jugador_t jugador,char ayuda_necesaria){
	bool tiene_ayuda=false;
	int i=0;
	while(i<jugador.tope_ayudas && tiene_ayuda==false){
		if(jugador.ayudas[i].codigo==ayuda_necesaria){
			tiene_ayuda=true;
		}
		i++;
	}
	return tiene_ayuda;
}

/* precondiciones:El danio que recibe no es negativo
 * postcondiciones:El jugador saldra con menos vida
 */
void restar_vida(int* vida,int danio){
	(*vida)-=danio;
}

/* precondiciones:Recibe coordenadas validas
 * postcondiciones:Indica si esas coordenadas se superponen
 */
bool arriba_de_objeto(coordenada_t jugador, coordenada_t objeto){
	bool esta_arriba=false;
	if(jugador.fil==objeto.fil && jugador.col==objeto.col){
		esta_arriba=true;
	}
	return esta_arriba;
}

/* 
 * Buscara si el jugador piso algun obstaculo, en caso de que si, devuelve su posicion en el vector
 */
bool esta_en_obstaculo(juego_t juego,int *posicion_obstaculo){
	(*posicion_obstaculo)=-1;
	bool encontro_obstaculo=false;
	int i=0;
	while(i<juego.tope_obstaculos && encontro_obstaculo==false){
		if(arriba_de_objeto(juego.jugador.posicion,juego.obstaculos[i].posicion)){		
			encontro_obstaculo=true;
			(*posicion_obstaculo)=i;
		}
		i++;
	}
	return encontro_obstaculo;
}

/* precondiciones:Recibe el obstaculo a eliminar junto con una posicion valida
 * postcondiciones:Elimina el obstaculo pegando arriba el siguiente
 */
void eliminar_obstaculo(obstaculo_t obstaculos[TOTAL_OBSTACULOS],int posicion,int* tope){
	for(int i=posicion;i<(*tope)-1;i++){
		obstaculos[i]=obstaculos[i+1];
	}
	(*tope)--;
}

/* precondiciones:Esta arriba de un obstaculo, por lo que recibe una posicion valida
 * postcondiciones:Le resta vida al jugador si no se puede defender
 */
void restar_danio_del_obstaculo(juego_t* juego,int posicion_obstaculo){
	
	if(juego->obstaculos[posicion_obstaculo].codigo==ESCREGUTO){

		if(!tiene_ayuda(juego->jugador,IMPEDIMENTA)){
			restar_vida(&(juego->jugador.vida),juego->obstaculos[posicion_obstaculo].danio);
		}

	}
	else if(juego->obstaculos[posicion_obstaculo].codigo==BOGGART){

		if(!tiene_ayuda(juego->jugador,RIDDIKULUS)){
			restar_vida(&(juego->jugador.vida),juego->obstaculos[posicion_obstaculo].danio);
		}

	}
	else{
		restar_vida(&(juego->jugador.vida),juego->obstaculos[posicion_obstaculo].danio);
	}
}

/* precondiciones:Recibe el juego ya cargado con los datos de las posiciones
 * postcondiciones:En caso de que no este arriba de una ayuda devuelve -1 como posicion
 */
bool esta_en_ayuda(juego_t juego,int* posicion_ayuda){
	(*posicion_ayuda)=-1;
	bool encontro_ayuda=false;
	int i=0;
	while(i<juego.tope_ayudas && encontro_ayuda==false){
		
		if(arriba_de_objeto(juego.jugador.posicion,juego.ayudas[i].posicion)){
			encontro_ayuda=true;
			(*posicion_ayuda)=i;
		}

		i++;
	}
	return encontro_ayuda;
}

/* precondiciones:Recibe una posicion de la ayuda positiva dentro de su limite
 * postcondiciones:Eliminara la ayuda en la que se encuentra el jugador y disminuira el tope
 */
void eliminar_ayuda(ayuda_t ayudas[TOTAL_AYUDAS],int posicion,int* tope){ 	
	for(int i=posicion;i<(*tope)-1;i++){
		ayudas[i]=ayudas[i+1];
	}
	(*tope)--;
}

/* precondiciones:Recibe una posicion de la ayuda positiva dentro de su limite
 * postcondiciones:Guardara la ayuda en la que se encuentra en el vector del jugador, tambien le aumentara su tope
 */
void guardar_ayuda(juego_t* juego,int posicion_ayuda){
	juego->jugador.ayudas[juego->jugador.tope_ayudas]=(juego->ayudas[posicion_ayuda]);
	(juego->jugador.tope_ayudas)++;
}

/* precondiciones:Recibe una ayuda valida
 * postcondiciones:Validara si es una pocion
 */
bool es_pocion(ayuda_t ayuda){
	return ayuda.codigo==POCION;
}

/*
 * Determina si va a superar el limite de vida del jugador
 */
bool supera_vida_maxima(int vida){
	return (vida+VIDA_POCION)>=VIDA_JUGADOR;
}

/*
 * Actualizará el juego. Restará vida si el jugador está sobre un obstáculo 
 * o lo eliminará si cuenta con el hechizo, aprenderá hechizos y todo lo 
 * que pueda suceder luego de un turno.
 */
void actualizar_juego(juego_t* juego){

	restar_vida(&(juego->jugador.vida),VIDA_MOVIMIENTO);

	int posicion_obstaculo,posicion_ayuda;

	if(esta_en_obstaculo(*juego,&posicion_obstaculo)){
		restar_danio_del_obstaculo(juego,posicion_obstaculo);
		eliminar_obstaculo(juego->obstaculos,posicion_obstaculo,&(juego->tope_obstaculos));
	}
	else if(esta_en_ayuda(*juego,&posicion_ayuda)){
		if(es_pocion(juego->ayudas[posicion_ayuda]) && supera_vida_maxima(juego->jugador.vida)){
			juego->jugador.vida=VIDA_JUGADOR;
		}
		else if(es_pocion(juego->ayudas[posicion_ayuda])){
			juego->jugador.vida+=(juego->ayudas[posicion_ayuda].vida_a_recuperar);
		}
		else{
			guardar_ayuda(juego,posicion_ayuda);
		}
		eliminar_ayuda(juego->ayudas,posicion_ayuda,&(juego->tope_ayudas));
	}
}

/*
 * Devolverá el estado del juego, 1 ganado, 0 en curso, -1 perdido.
 */
int estado_juego(juego_t juego){

	int estado=EN_CURSO;

	if(arriba_de_objeto(juego.jugador.posicion,juego.copa.posicion)){
		estado=GANADO;
	}

	if(arriba_de_objeto(juego.rival.posicion,juego.copa.posicion) || juego.jugador.vida<=0){
		estado=PERDIDO;
	}

	return estado;
}

/*
 * Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
 * No valida que dicha coordenada coincida con un pasillo ni que exista 
 * otro objeto en esa posición.
 */
coordenada_t posicion_aleatoria(){
	coordenada_t posicion;
	
	posicion.fil=rand()%TAMANIO;
	posicion.col=rand()%TAMANIO;

	return posicion;
}



/* precondiciones: La posicion de la copa ya es valida,
 * postcondiciones: La mostrara si cumple con las condiciones
 */
void mostrar_copa(jugador_t jugador, copa_t copa,char laberinto[TAMANIO][TAMANIO]){
	if((jugador.vida)<=VIDA_PARA_COPA || tiene_ayuda(jugador,ESFINGE)){
		laberinto[copa.posicion.fil][copa.posicion.col]=copa.codigo;
	}
}

/* precondiciones: Las coordenadas son validas, el tope no puede ser negativo
 * postcondiciones: Se mostraran todos los obstaculos
 */
void mostrar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS],int tope_obstaculos,char laberinto[TAMANIO][TAMANIO]){
	for(int i=0; i<tope_obstaculos;i++){
		laberinto[obstaculos[i].posicion.fil][obstaculos[i].posicion.col]=obstaculos[i].codigo;
	}
}

/* precondiciones: Las coordenadas son validas, el tope no puede ser negativo
 * postcondiciones: Se mostraran todas las ayudas
 */
void mostrar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS],int tope_ayudas,char laberinto[TAMANIO][TAMANIO]){
	for(int i=0; i<tope_ayudas;i++){
		laberinto[ayudas[i].posicion.fil][ayudas[i].posicion.col]=ayudas[i].codigo;
	}
}


/*
 * Actualizará la matriz mostrada al usuario, con los elementos presentes 
 * en el juego.
 */
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO]){

	copiar_laberinto(laberinto,juego.laberinto_original);

	mostrar_copa(juego.jugador,juego.copa,laberinto);
	
	mostrar_obstaculos(juego.obstaculos,juego.tope_obstaculos,laberinto);

	mostrar_ayudas(juego.ayudas,juego.tope_ayudas,laberinto);
	
	laberinto[juego.rival.posicion.fil][juego.rival.posicion.col]=juego.rival.codigo;

	laberinto[juego.jugador.posicion.fil][juego.jugador.posicion.col]=juego.jugador.codigo;
}

/*
 * Mostrará el laberinto por pantalla.
 */
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO]){
	for(int i=0;i<TAMANIO;i++){
		for (int j=0;j<TAMANIO;j++){
			printf(" %c",laberinto[i][j]);
		}
		printf("\n");
	}
}
