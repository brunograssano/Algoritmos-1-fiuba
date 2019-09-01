#include <stdio.h>
#include <stdbool.h>

const int MINIMO_EDAD=17,VALOR_MINIMO_CATEGORIA=0,VALOR_MAXIMO_CATEGORIA=10;
const char BUENA='B',REGULAR='R',MALA='M',NO='N';
const bool DATO_CORRECTO=true,DATO_INCORRECTO=false,ES_MEJOR_AL_CAMPEON=true,NO_ES_MEJOR_AL_CAMPEON=false,CUMPLE_EDAD=true,NO_CUMPLE_EDAD=false;

  /* Funcion cumple_minimo_edad
    PRECONDICIONES:
      Recibe un numero positivo
    POSTCONDICIONES:
      Si es mayor de edad, devuelve que la cumple
  */
  bool cumple_minimo_edad(unsigned int edad){
    if (MINIMO_EDAD<=edad){
      return CUMPLE_EDAD;
    }
    else{
      return NO_CUMPLE_EDAD;
    }
  }

  /* Funcion desea_ingresar_otro_alumno
    PRECONDICIONES:
      Recibe un parametro que sea de tipo char
    POSTCONDICIONES:
      Lee el parametro y lo remplaza mediante puntero
  */
  void desea_ingresar_otro_alumno(char* otro_alumno){
    printf("Desea ingresar otro alumno? (N para NO, cualquier otra tecla para sí): \n");
    scanf(" %c", otro_alumno);
  }

  /* Funcion controlar_dato_entero
    PRECONDICIONES:
      Recibe un numero entero
    POSTCONDICIONES:
      Devuelve si un valor esta dentro de los valores limite
  */
  bool controlar_dato_entero(int dato){
    if (VALOR_MINIMO_CATEGORIA<=dato && dato<=VALOR_MAXIMO_CATEGORIA) {
      return DATO_CORRECTO;
    }
    else{
      return DATO_INCORRECTO;
    }
  }

  /* Funcion controlar_dato_caracter
    PRECONDICIONES:
      Recibe un dato que sea caracter
    POSTCONDICIONES:
      Devuelve que es correcto si el caracter es igual a alguno de los tres posibles
  */
  bool controlar_dato_caracter(char dato){
    if (dato==BUENA || dato==REGULAR || dato==MALA) {
      return DATO_CORRECTO;
    }
    else{
      return DATO_INCORRECTO;
    }
  }

  /* Funcion pedir_inteligencia
    PRECONDICIONES:
      El valor que va a leer de la inteligencia es un entero
    POSTCONDICIONES:
      El valor que quede va a estar entre el 0 y el 10
  */
  void pedir_inteligencia(int* inteligencia){

    while(controlar_dato_entero(*inteligencia)==DATO_INCORRECTO){

      printf("Escriba su nivel de inteligencia (del 0 al 10)\n");
      scanf(" %i", inteligencia);

      if(controlar_dato_entero(*inteligencia)==DATO_INCORRECTO){
        printf("Escribió un valor que no esta dentro de los permitidos. \n");
      }

    }

  }

  /* Funcion pedir_fuerza
    PRECONDICIONES:
      El valor que va a leer de la fuerza es un entero
    POSTCONDICIONES:
      El valor que quede va a estar entre el 0 y el 10
  */
  void pedir_fuerza(int* fuerza){

    while(controlar_dato_entero(*fuerza)==DATO_INCORRECTO){

      printf("Escriba su nivel de fuerza (del 0 al 10)\n");
      scanf(" %i",fuerza);

      if(controlar_dato_entero(*fuerza)==DATO_INCORRECTO){
        printf("Escribió un valor que no esta dentro de los permitidos. \n");
      }

    }

  }

  /* Funcion pedir_capacidad_magica
    PRECONDICIONES:
      El valor que va a leer de la capacidad magica va a ser un solo caracter
    POSTCONDICIONES:
      Va a quedar con uno de los tres valores posibles
  */
  void pedir_capacidad_magica(char* capacidad_magica){

    while(controlar_dato_caracter(*capacidad_magica)==DATO_INCORRECTO){

      printf("Escriba su nivel de capacidad magica en un caracter (puede ser B,R,M)\n");
      scanf(" %c", capacidad_magica);

      if(controlar_dato_caracter(*capacidad_magica)==DATO_INCORRECTO){
        printf("Escribió un caracter que no esta dentro de los permitidos. \n");
      }

    }

  }

  /* Funcion comparacion_con_campeon
    PRECONDICIONES:
      Recibe los parametros ya controlados, de tal forma que la capacidad magica no sea por ejemplo una 'W' o la fuerza 200
      Al usarse por primera vez, el primer participante pasa a ser el campeon
    POSTCONDICIONES:
      En la comparacion, uno de los dos va a terminar siendo mejor, en caso de ser iguales el que ya era campeon sigue siendolo
  */
  bool comparacion_con_campeon(int inteligencia,int fuerza,char capacidad_magica,int inteligencia_campeon,int fuerza_campeon,char capacidad_magica_campeon){
    if(inteligencia>inteligencia_campeon){
      return ES_MEJOR_AL_CAMPEON;
    }
    else if (inteligencia==inteligencia_campeon) {
      if (fuerza>fuerza_campeon){
        return ES_MEJOR_AL_CAMPEON;
      }
      else if (fuerza==fuerza_campeon) {
        if ( (capacidad_magica==BUENA && (capacidad_magica_campeon==REGULAR || capacidad_magica_campeon==MALA) ) || (capacidad_magica==REGULAR && capacidad_magica_campeon==MALA)){
          return ES_MEJOR_AL_CAMPEON;
        }
        else{
          return NO_ES_MEJOR_AL_CAMPEON;
        }
      }
      else{
        return NO_ES_MEJOR_AL_CAMPEON;
      }
    }
    else{
      return NO_ES_MEJOR_AL_CAMPEON;
    }

  }

  /* Funcion cambiar_campeon
    PRECONDICIONES:
      Recibe todos los datos
    POSTCONDICIONES:
      Remplaza los datos del campeon viejo por el nuevo
  */
  void cambiar_campeon(unsigned edad,int inteligencia,int fuerza,char capacidad_magica,unsigned contador_alumnos,unsigned *edad_campeon,int *inteligencia_campeon,int *fuerza_campeon,char *capacidad_magica_campeon, unsigned *campeon){

    (*edad_campeon)=edad;
    (*inteligencia_campeon)=inteligencia;
    (*fuerza_campeon)=fuerza;
    (*capacidad_magica_campeon)=capacidad_magica;
    (*campeon)=contador_alumnos;

  }

  /* Funcion mostrar_campeon
    PRECONDICIONES:
      Recibe un numero que representaria al campeon, junto con sus datos
    POSTCONDICIONES:
      Muestra a un campeon si lo hay, si no, muestra que no lo hay
  */
  void mostrar_campeon(unsigned int campeon,unsigned edad,int inteligencia,int fuerza,char capacidad_magica){
    if (campeon==0){
      printf("No hay campeón :( \n");
    }
    else{
      printf("El campeón es el número %i :) \n", campeon );
      printf("Sus datos son: \n Edad %i \n Inteligencia %i \n Fuerza %i \n Capacidad magica %c \n ", edad, inteligencia, fuerza, capacidad_magica);
    }
  }

  /* Funcion leer_edad
    PRECONDICIONES:
      La edad no puede ser negativa
    POSTCONDICIONES:
      Se leyo la edad
  */
  void leer_edad(unsigned* edad){
    printf("Escriba su edad \n");
    scanf(" %u",edad);
  }

  /* Funcion main
    PRECONDICIONES:
      Empieza sin campeon y sin participantes
      Los datos del campeon inicial, son tales que los valores del primer ingresante pasen y se vuelva campeon
    POSTCONDICIONES:
      Muestra si hay campeon o no
  */
int main(){

  int inteligencia_campeon=-1,fuerza_campeon=-1;
  unsigned int  campeon=0,contador_alumnos=0;
  unsigned int edad=0, edad_campeon=0;
  char capacidad_magica_campeon;
  char otro_alumno;

  printf("Quiere ingresar un alumno? (N para no, cualquier otro caracter para si) \n");
  scanf(" %c",&otro_alumno);

  if(otro_alumno!=NO){

    while (otro_alumno!=NO){

      leer_edad(&edad);

      contador_alumnos=contador_alumnos+1;

      if(cumple_minimo_edad(edad)==CUMPLE_EDAD){

        int inteligencia=-1,fuerza=-1;   //Empiezan aca asi se reinician, de esta forma entran en sus respectivos controles al momento de pedir los datos devuelta
        char capacidad_magica='';

        pedir_inteligencia(&inteligencia);
        pedir_fuerza(&fuerza);
        pedir_capacidad_magica(&capacidad_magica);

        if (comparacion_con_campeon(inteligencia,fuerza,capacidad_magica,inteligencia_campeon,fuerza_campeon,capacidad_magica_campeon)==ES_MEJOR_AL_CAMPEON){
          cambiar_campeon(edad,inteligencia,fuerza,capacidad_magica,contador_alumnos,&edad_campeon,&inteligencia_campeon,&fuerza_campeon,&capacidad_magica_campeon,&campeon);
        }

      }
      else{
        printf("No cumple con el minimo de edad, por lo que no puede participar. \n");
      }

      desea_ingresar_otro_alumno(&otro_alumno);

    }

  }

  mostrar_campeon(campeon,edad_campeon,inteligencia_campeon,fuerza_campeon,capacidad_magica_campeon);

  return 0;

}
