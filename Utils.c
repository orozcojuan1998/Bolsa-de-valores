#include "Utils.h"


Empresa* Empresa_t(int acciones ,char* nombre ){
  Empresa *empresa = (Empresa*)malloc(sizeof(struct Empresa)) ;
  empresa->acciones = acciones ;
  empresa->nombre = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(empresa->nombre, 0, sizeof empresa->nombre);
  strncpy(empresa->nombre, nombre, sizeof empresa->nombre - 1);
  return empresa ;
}
Broker* Broker_t(char* nombre , char* nompipe , int pid){
  Broker *broker = (Broker*)malloc(sizeof(struct InfBroker)) ;
  broker->Broker = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(broker->Broker, 0, sizeof broker->Broker);
  strncpy(broker->Broker, nombre, sizeof broker->Broker - 1);
  broker->pipe = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(broker->pipe, 0, sizeof broker->pipe);
  strncpy(broker->pipe, nompipe, sizeof broker->pipe - 1);
  broker->pid = pid;
  return broker ;
}
Orden* Orden_t( char tipo , char* nombre_emp , int cant ,  int precio , char* nombreb){
  Orden *orden = (Orden*)malloc(sizeof(struct Orden)) ;
  orden->empresa = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(orden->empresa, 0, sizeof orden->empresa);
  strncpy(orden->empresa, nombre_emp, sizeof orden->empresa - 1);
  orden->precio = precio ;
  orden->cantidad = cant ;
  orden->broker = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(orden->broker, 0, sizeof orden->broker);
  strncpy(orden->broker, nombreb, sizeof orden->broker - 1);
  orden->tip = tipo;
  return orden ;
}
Datos* Datos_t(int monto , char* nombre , char* nombrepipe){
  Datos *dato = (Datos*)malloc(sizeof(struct Datos)) ;
  dato->empresas =  (Empresa*)malloc( sizeof(struct Empresa));
  dato->nombre = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(dato->nombre, 0, sizeof dato->nombre);
  strncpy(dato->nombre, nombre, sizeof dato->nombre - 1);
  dato->pipename = (char*)malloc((sizeof (char))*TAMNOMBRE) ;
  memset(dato->pipename, 0, sizeof dato->pipename);
  strncpy(dato->pipename, nombrepipe, sizeof dato->pipename - 1);
  return dato  ;
}
int comparator_venta(const void *a1 , const void *b1){
  Orden a = *(Orden *)a1;
  Orden b = *(Orden *)b1;

  if(a.precio < b.precio){
    return -1 ;
  }else{
    if(a.precio > b.precio){
      return 1 ;
    }else{
      return 0 ;
    }
  }
}
int comparator_compra(const void *a1 , const void *b1){
  Orden a = *(Orden *)a1;
  Orden b = *(Orden *)b1;

  if(a.precio < b.precio){
    return 1 ;
  }else{
    if(a.precio > b.precio){
      return -1 ;
    }else{
      return 0 ;
    }
  }
}
void print_t(const void *elemento){
  Orden a = *(Orden *)a1;
  /* PENDIENTE*/
}
