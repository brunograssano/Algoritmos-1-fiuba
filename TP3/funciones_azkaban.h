#ifndef __FUNCIONES_AZKABAN_H__
#define __FUNCIONES_AZKABAN_H__

#include <string.h>

#define MAX_NOMBRE 200
#define MAX_FECHA 7

typedef struct preso{
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta;	//B buena,R regular o M mala
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA];	//formato aaaamm
	int maldicion_realizada;	//1 crucio, 2 imperius
}preso_t;


/* precondiciones: Tiene que recibir un nombre valido que no supere el limite
 * postcondiciones: Crea el archivo con el nombre enviado, si ya existe se sobreescribe.
 */
int perdonables(char nombre_perdonables[MAX_NOMBRE]);

/* precondiciones: Tiene que recibir el nombre de un archivo perdonables generado anteriormente. La fecha tambien tiene que ser valida. La conducta es validada.
 * 				   Los archivos se encuentran en el mismo directorio. Si ya existe un archivo con la fecha mandada no lo sobreescribira.
 * postcondiciones: Se generara un archivo de texto con el nombre de los presos a ser liberados
 */
int liberar(char nombre_perdonables[MAX_NOMBRE], char fecha[MAX_FECHA], char* conducta);


/* precondiciones: Recibe una fecha con el formato valido, esta no tiene que superar la cantidad de cacaracteres indicada.
 * 				   El archivo con la fecha ya tiene que estar creado.
 * 				   Tienen que estar los archivos a actualizar (crucio.dat, imperius.dat), si no enviara una numero representando que fallo.
 * postcondiciones: Los archivos quedaran actualizados, ya que no tendran los presos en el archivo de liberados con la fecha enviada.
 */
int actualizar(char fecha[MAX_FECHA]);

/* precondiciones: Recibe una fecha con el formato valido, esta no tiene que superar la cantidad de cacaracteres indicada.
 * 				   Esta fecha se utiliza para buscar un archivo ya generado anteriormente. 
 * postcondiciones: Mostrara por pantalla los presos a ser liberados si hay alguno, si ocurre un error le avisa al usuario. 
 */
void mostrar_liberados(char fecha[MAX_FECHA]);

/*
 * Mostrara informacion correspondiente a todos los comandos disponibles en el programa
 */
void mostrar_ayuda();

#endif /*__FUNCIONES_AZKABAN_H__*/
