//*****************************************************************
/*
Archivo: Broker.c
Realizado por Santiago Chaustre y Andres Contreras
Proyecto Sistemas Operativos 2017-30
Compilacion: gcc Broker.c Utils.c List.c -o broker
Observaciones: para ejecutar correctamente este programa los datos se deben 
	introducir en el siguiente orden:
    - nombre del broker
    - nombre del pipe stockmarket
    - archivo de recursos
    - monto
Descripcion: Este archivo contiene la inplementacion de la parte de broker del proyecto,
    donde se encargar de validar las entradas enviar y recibir ordenes
Temas principales: uso de pipes, señaes y llamadas al sistema
Fecha ultima modificacion: 16/11/2017 
*/
//*****************************************************************

#include "Utils.h"

//*****************************************************************
//DECLARACIÓN DE FUNCIONES
//*****************************************************************
/*hilos*/
/*se encarga de recibir las respuestas asincronas*/
void *respuestaAsin(void *datos);
/*se encargar de recibir los comandos del usuario*/
void *manejoUsuario();
/*se encarga de leer los datos del archivo*/
void leerDatos(char *arch);
/*se encarga que los datos esten correctos para despues enviarlos
si son correctos retorna 1*/
Orden *validarEntrada(char *arch);
/*envia los datos al stockMarket*/
int enviarDatos(Orden *orden);
void printRespuesta(Respuesta respu);
void sig_handler(int sengnal);
void estadoBroker();
int validarEmpresa(char *empresa, int acciones);
void manejoCompra(Respuesta respu);
void manejoVenta(Respuesta respu);
void manejoConsulta(Respuesta respu);
int maxAcciones(char *empresa);
Datos *datos;
int fd1, fd;

int main(int argc, char const *argv[])
{
  signal(SIGUSR1, sig_handler);
  mode_t fifo_mode = S_IRUSR | S_IWUSR;
  char *comando;
  pthread_t thread1, thread2;
  int continuar, creado;
  int i;
  /*fin de variables*/

  if (argc < 5)
  {
    printf("\nUso incorrecto \n");
    printf("Uso: %s brokername pipestockmarket recursos_iniciales monto \n", argv[0]);
    exit(1);
  }

  datos = Datos_t(atoi(argv[4]), (char *)(argv[1]), (char *)argv[2]);
  leerDatos((char *)argv[3]);
  printf("Datos del broker cargados \n");
  for (i = 0; i < datos->tam; i++)
  {
    printf("NOOMBRE: %s \n", (datos->empresas)[i].nombre);
    printf("ACCIONES: %d \n", (datos->empresas)[i].acciones);
  }
  /*creacion de pipe */
  unlink(datos->nombre);
  if (mkfifo(datos->nombre, fifo_mode) == -1)
  {
    perror("Client  mkfifo");
    exit(1);
  }

  printf("\n");
  printf("Datos ingresados correctamente\n");

  /*crear hilo recepcion respuesta  asincrona*/
  /* crear hilo para comandos */

  /*if (pthread_create(&thread1, NULL, respuestaAsin, (void *)&datos) != 0)
  {
  perror("Hilo : ");
  exit(2);
}
if (pthread_create(&thread2, NULL, manejoUsuario, (void *)&datos) != 0)
{
perror("Hilo : ");
exit(2);
}



int *joinh;
if (pthread_join(thread1, (void **)&joinh) != 0)
{
perror("Error: ");
exit(2);
}
if (pthread_join(thread2, (void **)&joinh) != 0)
{
perror("Error: ");
exit(2);
}

*/
  //respuestaAsin();
  manejoUsuario();
  exit(0);
}
void *respuestaAsin(void *datos)
{
  /*
  signal(SIGUSR1,sig_handler);
  while(1)
  {
  pause();
}
*/
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void *manejoUsuario()
{
  signal(SIGUSR1, sig_handler);
  int continuar;
  Orden *orden;
  char *comando = (char *)malloc(sizeof(char) * TAMNOMBRE);
  continuar = 1;
  //fd1 = open(datos->pipename , O_RDONLY | O_NONBLOCK);
  //if(fd1 <= 0){
  //  printf("\n No me abri");
  //}else{
  //  printf("\n Me abri con %d ",fd1);
  //}
  printf("%s\n", "Ingrese un comando");
  while (continuar)
  {
    printf("~$ ");
    fgets(comando, TAMNOMBRE, stdin);
    to_lowercase(comando);

    orden = validarEntrada(comando);
    if (orden != NULL)
    {
      enviarDatos(orden);
    }
   
  }
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void leerDatos(char *arch)
{
  // archivo
  FILE *archivo;
  char *linea;
  char *nomEmpr;
  Empresa *empresa;
  char *token;
  char *s1 = " ";
  int acciones;

  nomEmpr = (char *)malloc(sizeof(char) * maxchar);
  linea = (char *)malloc(sizeof(char) * maxchar);

  archivo = fopen(arch, "r");
  /*revisar perror*/
  if (archivo == NULL)
  {
    perror("Error: ");
    exit(1);
  }
  else
  {
    printf("\nEl contenido del archivo de prueba es \n\n");
    while (feof(archivo) == 0)
    {
      fgets(linea, maxchar, archivo);
      printf("LINEA: %s \n", linea);
      /* get the first token */
      token = strtok(linea, s1);
      printf("Token1: %s \n", token);
      /* walk through other tokens */
      strcpy(nomEmpr, token);
      token = strtok(NULL, s1);
      printf("Token2: %s \n", token);
      acciones = atoi(token);

      empresa = Empresa_t(acciones, nomEmpr);
      add_empresa(datos, empresa);
    }
  }

  fclose(archivo);
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
Orden *validarEntrada(char *comando)
{
  /*oden de llegada de datos
  -   tipo de operacion
  -   empresa
  -   acciones
  -   precio
  */
  char *s = ":";
  char *tipo;
  char *empresa;
  char *acciones;
  char *precio;

  char *token;
  Orden *orden;
  printf("COM: %s \n", comando);
  /*se capturan los datos*/
  token = strtok(comando, s);
  if (token != NULL && strcmp(token, "\n") != 0)
  {
    tipo = malloc(sizeof(char) * TAMNOMBRE);
    strcpy(tipo, token);
  }
  else
  {
    tipo = NULL;
  }
  token = strtok(NULL, s);
  if (token != NULL && strcmp(token, "\n") != 0)
  {
    empresa = malloc(sizeof(char) * TAMNOMBRE);
    strcpy(empresa, token);
  }
  else
  {
    empresa = NULL;
  }
  token = strtok(NULL, s);
  if (token != NULL && strcmp(token, "\n") != 0)
  {
    acciones = malloc(sizeof(char) * TAMNOMBRE);
    strcpy(acciones, token);
  }
  else
  {
    acciones = NULL;
  }

  token = strtok(NULL, s);
  if (token != NULL && strcmp(token, "\n") != 0)
  {
    precio = malloc(sizeof(char) * 30);
    strcpy(precio, token);
  }
  else
  {
    precio = NULL;
  }
  /*validacion de datos
  los datos que el broker puede validar son
  el tipo de comando
  si va a vender que el nombre de la emrpesas exista
  si el tipo es monto se muestras de una ves
  para las demas validaciones lo hace el stockmarket*/
  if ((strcmp(tipo, "venta") == 0) || (strcmp(tipo, "compra") == 0) ||
      (strcmp(tipo, "consulta") == 0) || (strcmp(tipo, "monto") == 0))
  {
    if (strcmp(tipo, "monto") == 0)
    {
      estadoBroker();
      return NULL;
    }
    printf("CCCCC1\n");
    if (strcmp(tipo, "venta") == 0)
    {
      // existen dos tipos
      if (empresa != NULL)
      {
        if (acciones == NULL && precio == NULL)
        {

          return Orden_t('T', empresa, maxAcciones(empresa), -1, datos->nombre);
        }
        else
        {
          if (validarEmpresa(empresa, atoi(acciones)) == 0)
          {
            printf("Error: no se puede realizar la venta\n");
            printf("no existe el nombre de la empresa o el no se tienen la cantida de acciones a vender\n");
            return NULL;
          }
          else{
            return Orden_t('V', empresa, atoi(acciones), atoi(precio), datos->nombre);
          }
          
        }
      }
      else
      {
        printf("Venta invalida \n");
        return NULL;
      }
       
    }
    printf("CCCCC2\n");
    if ((strcmp(tipo, "consulta") == 0))
    {
      if ((empresa != NULL))
      {
        return Orden_t('Q', empresa, -1, -1, datos->nombre);
      }
      else
      {
        printf("Consulta invalida\n");
        return NULL;
      }
    }
    

    printf("CCCCC3\n");
    if ((strcmp(tipo,"compra") == 0))
    {
      if(empresa != NULL && acciones != NULL && precio != NULL)
      {
        printf("MONTO %d MULTI %d\n", datos->monto, (atoi(acciones) * atoi(precio)));
        if ((atoi(acciones) * atoi(precio)) <= datos->monto)
        {
          return Orden_t('C', empresa, atoi(acciones), atoi(precio), datos->nombre);
        }
        else
        {
          printf("No dispone de dinero suficiente para realizar la transaccion\n");
          return NULL;
        }
      }
      else
      {
        printf("Compra invalida\n");
      }
    }
    else
    {
      printf("Compra invalida\n");
    }
  }
  else
  {
    printf("\tCOMANDO INVALIDO\n");
    printf("Comandos disponibles: \n");
    printf("    *) compra\n");
    printf("    *) venta\n");
    printf("    *) consulta\n");
    printf("    *) monto\n");
    return NULL;
  }
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
int enviarDatos(Orden *orden)
{
  Mensaje *mensaje;

  mensaje = Mensaje_t(orden, datos->pid, datos->nombre);
  int creado, n;
  creado = 0;

  do
  {
    fd = open(datos->pipename, O_WRONLY | O_NONBLOCK);

    if (fd == -1)
    {
      perror(" Cliente  Abr*iendo el segundo pipe. Se volvera a intentar ");
      sleep(5);
    }
    else
    {
      creado = 1;
      printf("\n Eh abierto el pipe \n");
    }
  } while (creado == 0);

  if (write(fd, mensaje, sizeof(Mensaje)) == -1)
  {
    perror(" Write ");
    exit(2);
  }
  close(fd);
  printf("Se ha enviado la orden al stock market \n");
  return 1;
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
int validarEmpresa(char *empresa, int acciones)
{
  int i, band = 0;
  int tam = datos->tam;
  for (i = 0; i < tam && band == 0; i++)
  {

    if (strcmp(((datos->empresas)[i]).nombre, empresa) == 0)
    {
      if (((datos->empresas)[i]).acciones >= acciones)
      {
        band = 1;
      }
    }
  }

  return band;
}
/*utlilzado cuando se escribe el comando monto*/
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void estadoBroker()
{
  printf("Su monto actual es: %d", datos->monto);
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void sig_handler(int sengnal)
{
  printf("%s\n", "Entre en el handler ");
  int creado, n;
  Respuesta respuesta;

  creado = 0;
  do
  {
    fd1 = open(datos->nombre, O_RDONLY);
    if (fd1 == -1)
    {
      perror("pipe");
      printf(" Se volvera a intentar despues\n");
      //sleep(5);
    }
    else
    {
      creado = 1;
    }
  } while (creado == 0);

  do
  {
    n = read(fd1, &respuesta, sizeof(Respuesta));
  } while (n <= 0);
  printf("---> %c\n", respuesta.tipo);
  close(fd1);
  printRespuesta(respuesta);
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void printRespuesta(Respuesta respu)
{

  if (respu.tipo == 'C')
  {
    printf("===============================================\n");
    printf("Se ha realizado la compra exitosa de: \n");
    printf("Acciones de la empresa: %s \n", (char *)respu.empresa);
    printf("acciones compradas: %d \n", respu.acciones);
    printf("con un monto total de: %d \n", (respu.acciones) * (respu.monto));
    printf("por medio del broker: %s \n", (char *)respu.brokers);
    printf("===============================================\n");
    manejoCom*pra(respu);
  }
  if (respu.tipo == 'V')
  {
    if (respu.acciones == -1)
    {
      printf("no se pudo realizar la venta: no se econtro el precio mas alto en compras para dicha empresa \n");
    }
    else
    {
      printf("===============================================\n");
      printf("se ha  realizado la venta exitosa de: \n");
      printf("acciones de la empresa: %s \n", (char *)respu.empresa);
      printf("acciones vendidas: %d \n", respu.acciones);
      printf("con un monto total de: %d \n", (respu.acciones * respu.monto));
      printf("por medio del broker: %s \n", (char *)respu.brokers);
      printf("===============================================\n");
      manejoVenta(respu);
    }
  }
  if (respu.tipo == 'Q')
  {
    manejoConsulta(respu);
  }
}

/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void manejoVenta(Respuesta respu)
{
  int i;
  int ban;
  ban = 0;
  datos->monto += respu.monto;
  for (i = 0; i < datos->tam && ban == 0; i++)
  {
    if (strcmp((datos->empresas)[i].nombre, respu.empresa) == 0)
    {
      (datos->empresas)[i].acciones -= respu.acciones;
      ban = 1;
    }
  }
  if (ban == 1)
  {
    printf("se realizo correctamente la acutalizacion de datos\n");
  }
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void manejoCompra(Respuesta respu)
{
  int i;
  int ban;
  Empresa *empresa;
  ban = 0;
  datos->monto -= respu.monto;
  for (i = 0; i < datos->tam && ban == 0; i++)
  {
    if (strcmp((datos->empresas)[i].nombre, respu.empresa) == 0)
    {
      (datos->empresas)[i].acciones += respu.acciones;
      ban = 1;
    }
  }
  if (ban == 1)
  {
    printf("Se realizo correctamente la acutalizacion de datos\n");
  }
*  else
  {
    empresa = Empresa_t(respu.acciones,respu.nombre);
    add_empresa(datos, empresa);
    printf("Se agrego una nueva empresa a su lista de empresas\n");
  }
}
/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
void manejoConsulta(Respuesta respu)
{
  if (respu.acciones == -1)
  {
    printf("No existe la empresa actualmente en le sistema \n");
  }
  else
  {
    printf("====================================================\n");
    printf("El precio de acciones de la empresa %s es:\n", respu.empresa);
    printf("Precio venta:  %d", respu.acciones);
    printf("Precio compra: %d", respu.monto);
    printf("====================================================\n");
  }
}

/*
  ***************************************************************************************************************
  Funcion: Primos
  Descripcion: Es la fucion que sera ejecutada por cada hilo,
	verificara que cada numero del rango que le corresponde sea primo
  Parametros de entrada:
  -datos:	Nodo donde se encuentra almacenados los datos y el arrglos de primos del hilo

  Parametro que devuelve: No devuelve nada
  ***************************************************************************************************************
  */
int maxAcciones(char *empresa)
{
  int i;
  for (i = 0; i < datos->tam; i++)
  {
    if (strcmp((datos->empresas)[i].nombre, empresa) == 0)
    {
      return (datos->empresas)[i].acciones;
    }
  }
}
