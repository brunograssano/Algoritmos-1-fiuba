
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "funciones_azkaban.h"

const int FALLO=-1;

#define BUENA 'B'
#define REGULAR 'R'
#define MALA 'M'
#define FORMATO_LIBERADO "%[^\n]\n"
#define FORMATO_TEXTO_IMPRESO "%s\n"
#define LECTURA "r"
#define ESCRITURA "w"

/*
 * Lee un preso de un archivo ya abierto y devuelve la cantidad leida.
 */
size_t leer_preso(preso_t* preso,FILE* file){
	return fread(preso,sizeof(preso_t),1,file);
}

/*
 * Escribe el preso en el archivo enviado abierto, no revisa si se escribio en el caso de falta de permisos o memoria
 */
void escribir_preso(preso_t preso,FILE* file){
	fwrite(&preso,sizeof(preso_t),1,file);
}
/*
 * Esccribe el resto de presos en un archivo ya abierto en otro.
 */
void escribir_resto(FILE* file_viejo,FILE* file_nuevo,size_t leidos,preso_t preso){
	while(0<leidos){
		escribir_preso(preso,file_nuevo);
		leidos = leer_preso(&preso,file_viejo);
	}
}

/* precondiciones: Los archivos estan abiertos y ordenados por nombre.
 * postcondiciones: Los que esten solo una vez en los archivos apareceran en el archivo perdonables. Los que esten en ambos no. Este archivo queda ordenado por nombre.
 */
void juntar_posibles_perdonables(FILE* file_crucio,FILE* file_imperius,FILE* file_perdonables){
	
	preso_t preso_crucio,preso_imperius;
	
	size_t leidos_crucio=leer_preso(&preso_crucio,file_crucio);
	size_t leidos_imperius=leer_preso(&preso_imperius,file_imperius);

	while(0<leidos_crucio && 0<leidos_imperius){

		if(strcmp(preso_crucio.nombre,preso_imperius.nombre)<0){
			escribir_preso(preso_crucio,file_perdonables);
			leidos_crucio=leer_preso(&preso_crucio,file_crucio);
		}
		else if(strcmp(preso_crucio.nombre,preso_imperius.nombre)>0){
			escribir_preso(preso_imperius,file_perdonables);
			leidos_imperius=leer_preso(&preso_imperius,file_imperius);
		}
		else{
			leidos_crucio=leer_preso(&preso_crucio,file_crucio);
			leidos_imperius=leer_preso(&preso_imperius,file_imperius);
		}
	}
	escribir_resto(file_crucio,file_perdonables,leidos_crucio,preso_crucio);
	escribir_resto(file_imperius,file_perdonables,leidos_imperius,preso_imperius);

}

/* precondiciones: Tiene que recibir un nombre valido que no supere el limite
 * postcondiciones: Crea el archivo con el nombre enviado, si ya existe se sobreescribe.
 */
int perdonables(char nombre_perdonables[MAX_NOMBRE]){

	FILE* file_crucio = fopen("crucio.dat",LECTURA);
	if(file_crucio==NULL){
		return FALLO;
	}

	FILE* file_imperius = fopen("imperius.dat",LECTURA);
	if(file_imperius==NULL){
		fclose(file_crucio);
		return FALLO;								
	}

	FILE* file_perdonables = fopen(nombre_perdonables,ESCRITURA);

	if(file_perdonables==NULL){
		fclose(file_crucio);
		fclose(file_imperius);
		return FALLO;
	}

	juntar_posibles_perdonables(file_crucio,file_imperius,file_perdonables);

	fclose(file_crucio);
	fclose(file_imperius);
	fclose(file_perdonables);

	return 0;
}

/* precondiciones: El nombre del archivo a verificar ya viiene armado.
 * postcondiciones: Si lo puede abrir significa que ya existe el archivo, caso contrario no existe.
 */
bool existe_archivo(char nombre_liberados[MAX_NOMBRE]){
	FILE* file_liberados = fopen(nombre_liberados,LECTURA);
	if(file_liberados==NULL){
		return false;
	}
	else{
		fclose(file_liberados);
		return true;
	}
}

/* precondiciones: Recibe una fecha correcta.
 * postcondiciones: El nombre queda armado.
 */
void armar_nombre_liberados(char fecha[MAX_FECHA],char nombre_liberados[MAX_NOMBRE]){
	strcpy(nombre_liberados,"liberados_");
	strcat(nombre_liberados,fecha);
	strcat(nombre_liberados,".txt");
}

/* precondiciones: La conducta es valida para el preso y la requerida.
 * postcondiciones: Devuelve si cumplen las condiciones en cuanto a conducta para su liberacion.
 */
bool cumple_con_conducta(char conducta_preso,char conducta_requerida){
	bool cumple=false;
	switch(conducta_requerida){
		case BUENA:
			if(conducta_preso==BUENA){
				cumple=true;
			}
			break;
		case REGULAR:
			if(conducta_preso==BUENA || conducta_preso==REGULAR){
				cumple=true;
			}
			break;
		case MALA:
			cumple=true;
			break;
	}

	return cumple;
}

/* precondiciones: El archivo se encuentra abierto.
 * postcondiciones: Lee el nombre del archivo y devuelve la cantidad que leyo.
 */
int leer_liberado(FILE* file_liberados,char nombre[MAX_NOMBRE]){
	return fscanf(file_liberados,FORMATO_LIBERADO,nombre);
}

/* precondiciones: El archivo a escribir el preso ya se encuentra abierto, su nombre es correcto.
 * postcondiciones: Quedara escrito el preso si es que hay espacio y permisos.
 */
void escribir_liberado(FILE* file_liberados,char nombre[MAX_NOMBRE]){
	fprintf(file_liberados,FORMATO_TEXTO_IMPRESO,nombre);
}

/* precondiciones: La conducta viene validada, los archivos abiertos, y la fecha es correcta.
 * postcondiciones: Escribe los presos que cumplan las condicioens en el archivo enviado.
 */
void presos_a_liberar(FILE* file_perdonables,FILE* file_liberados,char fecha[MAX_FECHA],char conducta){
	
	preso_t preso_perdonable;
	size_t leidos_perdonables=leer_preso(&preso_perdonable,file_perdonables);

	while(0<leidos_perdonables){

		if(strcmp(preso_perdonable.fecha,fecha)<=0 && cumple_con_conducta(preso_perdonable.conducta,conducta)){
			escribir_liberado(file_liberados,preso_perdonable.nombre);
		}

		leidos_perdonables=leer_preso(&preso_perdonable,file_perdonables);
	}
}

/* precondiciones: Recibe un caracter solo.
 * postcondiciones: Verifica que sea uno correcto.
 */
bool es_conducta_correcta(char conducta){
	return (conducta==BUENA || conducta==REGULAR || conducta==MALA);
}

/* precondiciones: Tiene que recibir el nombre de un archivo perdonables generado anteriormente. La fecha tambien tiene que ser valida. La conducta es validada.
 * 				   Los archivos se encuentran en el mismo directorio. Si ya existe un archivo con la fecha mandada no lo sobreescribira.
 * postcondiciones: Se generara un archivo de texto con el nombre de los presos a ser liberados
 */
int liberar(char nombre_perdonables[MAX_NOMBRE], char fecha[MAX_FECHA],char* conducta){

	if(!es_conducta_correcta(*conducta)){
		return FALLO;
	}

	FILE* file_perdonables = fopen(nombre_perdonables,LECTURA);
	if(file_perdonables==NULL){
		return FALLO;
	}

	char nombre_liberados[MAX_NOMBRE];
	armar_nombre_liberados(fecha,nombre_liberados);
	if(existe_archivo(nombre_liberados)){
		fclose(file_perdonables);
		return FALLO;
	}

	FILE* file_liberados = fopen(nombre_liberados,ESCRITURA);
	if(file_liberados==NULL){
		fclose(file_perdonables);
		return FALLO;		
	}

	presos_a_liberar(file_perdonables,file_liberados,fecha,*conducta);

	fclose(file_perdonables);
	fclose(file_liberados);

	return 0;
}

/* precondiciones: Los archivos vienen abiertos y ordenados, de forma tal de que haga la lectura una vez.
 * postcondiciones: En los archivos nuevos no van a aparecer los presos liberados.
 */
void escribir_diferencia_varios_archivos(FILE* file_liberados,FILE* file_crucio,FILE* file_imperius,FILE* nuevo_crucio,FILE* nuevo_imperius,size_t* leidos_crucio,size_t* leidos_imperius,int* leidos_liberados,preso_t* preso_crucio,preso_t* preso_imperius,preso_t* preso_liberado){
	while(0<(*leidos_liberados) && 0<(*leidos_crucio) && 0<(*leidos_imperius)){
		if(strcmp(preso_crucio->nombre,preso_liberado->nombre)==0){
			(*leidos_liberados) = leer_liberado(file_liberados,preso_liberado->nombre);
			(*leidos_crucio) = leer_preso(preso_crucio,file_crucio);
		}
		else if(strcmp(preso_imperius->nombre,preso_liberado->nombre)==0){
			(*leidos_liberados) = leer_liberado(file_liberados,preso_liberado->nombre);
			(*leidos_imperius) = leer_preso(preso_imperius,file_imperius);
		}
		else{
			if(strcmp(preso_crucio->nombre,preso_liberado->nombre)<0){
				escribir_preso((*preso_crucio),nuevo_crucio);
				(*leidos_crucio) = leer_preso(preso_crucio,file_crucio);
			}
			if(strcmp(preso_imperius->nombre,preso_liberado->nombre)<0){
				escribir_preso((*preso_imperius),nuevo_imperius);
				(*leidos_imperius) = leer_preso(preso_imperius,file_imperius);
			}
		}
	}
}

/* precondiciones: Los archivos vienen abiertos y ordenados, de forma tal de que haga la lectura una vez.
 * postcondiciones: En los archivos nuevos no van a aparecer los presos liberados.
 * 					Una vez terminado el while de la funcion, ya no van a quedar presos libres que procesar.
 */
void escribir_diferencia_individual(FILE* file_viejo,FILE* file_nuevo,FILE* file_libres,size_t* leidos_preso ,int leidos_libres,preso_t* preso,preso_t preso_libre){
	while(0<leidos_libres && 0<(*leidos_preso)){
		if(strcmp(preso->nombre,preso_libre.nombre)==0){
			leidos_libres = leer_liberado(file_libres,preso_libre.nombre);
			(*leidos_preso) = leer_preso(preso,file_viejo);
		}
		else{
			escribir_preso((*preso),file_nuevo);
			(*leidos_preso) = leer_preso(preso,file_viejo);
		}
	}
}

/* precondiciones: Los archivos estan abiertos
 * postcondiciones: Los datos de los presos a eliminar ya no estaran en los nuevos archivos
 */
void actualizar_archivos_prisioneros(FILE* file_liberados,FILE* file_crucio,FILE* file_imperius,FILE* nuevo_crucio,FILE* nuevo_imperius){
	
	preso_t preso_liberado,preso_crucio,preso_imperius;

	int leidos_liberados = leer_liberado(file_liberados,preso_liberado.nombre);
	size_t leidos_crucio = leer_preso(&preso_crucio,file_crucio);
	size_t leidos_imperius = leer_preso(&preso_imperius,file_imperius);

	escribir_diferencia_varios_archivos(file_liberados,file_crucio,file_imperius,nuevo_crucio,nuevo_imperius,&leidos_crucio,&leidos_imperius,&leidos_liberados,&preso_crucio,&preso_imperius,&preso_liberado);
	
	escribir_diferencia_individual(file_crucio,nuevo_crucio,file_liberados,&leidos_crucio,leidos_liberados,&preso_crucio,preso_liberado);
	escribir_diferencia_individual(file_imperius,nuevo_imperius,file_liberados,&leidos_imperius,leidos_liberados,&preso_imperius,preso_liberado);
	
	escribir_resto(file_crucio,nuevo_crucio,leidos_crucio,preso_crucio);
	escribir_resto(file_imperius,nuevo_imperius,leidos_imperius,preso_imperius);
}
/*
 * Borra los archivos desactualizados y les cambia el nombre a los actualizados por el nombre correcto.
 */
void intercambiar_archivos(){
	remove("crucio.dat");
	remove("imperius.dat");

	rename("auxiliar_cruxio.dat","crucio.dat");
	rename("auxiliar_imperius.dat","imperius.dat");
}

/* precondiciones: Recibe una fecha con el formato valido, esta no tiene que superar la cantidad de cacaracteres indicada.
 * 				   El archivo con la fecha ya tiene que estar creado.
 * 				   Tienen que estar los archivos a actualizar (crucio.dat, imperius.dat), si no enviara una numero representando que fallo.
 * postcondciones: Los archivos quedaran actualizados, ya que no tendran los presos en el archivo de liberados con la fecha enviada.
 */
int actualizar(char fecha[MAX_FECHA]){

	char nombre_liberados[MAX_NOMBRE];
	armar_nombre_liberados(fecha,nombre_liberados);
	
	FILE* file_liberados = fopen(nombre_liberados,LECTURA);
	if(file_liberados==NULL){
		return FALLO;
	}	

	FILE* file_crucio = fopen("crucio.dat",LECTURA);
	if (file_crucio==NULL){
		fclose(file_liberados);
		return FALLO;
	}

	FILE* file_imperius = fopen("imperius.dat", LECTURA);
	if(file_imperius==NULL){
		fclose(file_liberados);
		fclose(file_crucio);
		return FALLO;
	}

	FILE* nuevo_crucio = fopen("auxiliar_cruxio.dat",ESCRITURA);
	if(nuevo_crucio==NULL){
		fclose(file_liberados);
		fclose(file_crucio);
		fclose(file_imperius);
		return FALLO;
	}

	FILE* nuevo_imperius = fopen("auxiliar_imperius.dat",ESCRITURA);
	if(nuevo_imperius==NULL){
		fclose(file_liberados);
		fclose(file_crucio);
		fclose(file_imperius);
		fclose(nuevo_crucio);
		return FALLO;
	}

	actualizar_archivos_prisioneros(file_liberados,file_crucio,file_imperius,nuevo_crucio,nuevo_imperius);

	fclose(file_liberados);
	fclose(file_crucio);
	fclose(file_imperius);
	fclose(nuevo_crucio);
	fclose(nuevo_imperius);

	intercambiar_archivos();

	return 0;
}
/*
 * Muestra un mensaje indicando si hay presos a liberar.
 */
void ver_estado_liberados(int leidos_liberados){
	if(0<leidos_liberados){
		printf("Los presos a ser liberados son los siguientes: \n");
	}
	else{
		printf("No hay presos a ser liberados. \n");
	}
}

/*
 * Imprime por pantalla los nombres de los presos, el archivo esta abierto.
 */
void imprimir_liberados(int leidos_liberados,FILE* file_liberados,char nombre_preso[MAX_NOMBRE]){
	
	while(0<leidos_liberados){
		printf("	->"FORMATO_TEXTO_IMPRESO,nombre_preso);
		leidos_liberados = leer_liberado(file_liberados,nombre_preso);
	}
}

/* precondiciones: El archivo ya viene abierto.
 * postcondiciones: Imprime el nombre de los presos.
 */
void mostrar_presos_libres(FILE* file_liberados){
	char nombre_preso[MAX_NOMBRE];

	int leidos_liberados = leer_liberado(file_liberados,nombre_preso);

	ver_estado_liberados(leidos_liberados);

	imprimir_liberados(leidos_liberados,file_liberados,nombre_preso);
}

/*precondiciones: Recibe una fecha con el formato valido, esta no tiene que superar la cantidad de cacaracteres indicada.
 * Esta fecha se utiliza para buscar un archivo ya generado anteriormente. 
 *postcondicones: Mostrara por pantalla los presos a ser liberados si hay alguno, si ocurre un error le avisa al usuario. 
 */
void mostrar_liberados(char fecha[MAX_FECHA]){

	char nombre_liberados[MAX_NOMBRE];
	armar_nombre_liberados(fecha,nombre_liberados);
	FILE* file_liberados = fopen(nombre_liberados,LECTURA);
	if(file_liberados==NULL){
		printf("Ha habido un error intentando abrir el archivo de liberados con la fecha indicada. \n");
	}
	else{
		mostrar_presos_libres(file_liberados);
		fclose(file_liberados);
	}	
}

/*
 * Muestra informacion del comando perdonables
 */
void mostrar_informacion_perdonables(){
	printf("	-> perdonables <nombre_archivo>\n");
	printf("		 Genera un archivo con los presos que pueden llegar a ser liberados, es decir, aquellos que solamente hicieron una maldicion. \n");
	printf(" 		 El nombre del archivo va en el lugar de <nombre_archivo>\n");
	printf("		 Ejemplo: ./azkaban perdonables posibles_perdonables \n \n");
}

/*
 * Muestra informacion del comando liberar
 */
void mostrar_informacion_liberar(){
	printf("	-> liberar <archivo_perdonables> aaaamm conducta\n");
	printf("		 Genera un archivo con los presos que deben ser liberados en el mes y anio puesto en (aaaamm) con conducta mejor o igual a la recibida.\n");
	printf("		 La conducta puede ser B (Buena), R (Regular), M (Mala)\n");
	printf("		 El archivo a generar surge de <archivo_perdonables> \n");
	printf("		 Ejemplo: ./azkaban liberar posibles_perdonables.dat 201906 B \n \n");
}

/*
 * Muestra informacion del comando actualizar
 */
void mostrar_informacion_actualizar(){
	printf("	-> actualizar aaaamm\n");
	printf("		 Actualiza los archivos imperius.dat y crucio.dat sacandoles los presos que ya esten en el archivo de liberados. \n");
	printf("		 Ejemplo: ./azkaban actualizar 201906 \n \n");
}

/*
 * Muestra informacion del comando mostrar_liberados
 */
void mostrar_informacion_liberados(){
	printf("	-> mostrar_liberados aaaamm\n");
	printf("		 Se muestran los nombres de los presos a ser liberados. Se le manda la fecha del archivo que se quiere mostrar. \n");
	printf("		 Ejemplo: ./azkaban mostrar_liberados 201906 \n \n");
}

/*
 * Muestra informacion del comando ayuda
 */
void mostrar_informacion_ayuda(){
	printf("	-> ayuda\n");
	printf("		  Muestra esta informacion\n \n");
}

/*
 * Mostrara informacion correspondiente a todos los comandos disponibles en el programa
 */
void mostrar_ayuda(){
	printf("El programa tiene 5 comandos.\n \n");
	mostrar_informacion_perdonables();
	mostrar_informacion_liberar();
	mostrar_informacion_actualizar();
	mostrar_informacion_liberados();
	mostrar_informacion_ayuda();
}