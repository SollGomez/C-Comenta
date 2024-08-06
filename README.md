# [tp-C-Comenta](https://docs.google.com/document/d/1-AqFTroovEMcA1BfC2rriB5jsLE6SUa4mbcAox1rPec/edit)
## Dependencias

Para poder compilar y ejecutar el proyecto, es necesario tener instalada la
biblioteca [so-commons-library] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
make install
```

## Compilación

Cada módulo del proyecto se compila de forma independiente a través de un
archivo `makefile`. Para compilar un módulo, es necesario ejecutar el comando
`make` desde la carpeta correspondiente.

El ejecutable resultante se guardará en la carpeta `bin` del módulo.

## Importar desde Visual Studio Code

Para importar el workspace, debemos abrir el archivo `tp.code-workspace` desde
la interfaz o ejecutando el siguiente comando desde la carpeta raíz del
repositorio:

```bash
code tp.code-workspace
```

## Entrega
```bash
git clone https://github.com/sisoputnfrba/so-deploy.git
cd so-deploy
./deploy.sh -t=/home/utnso -p=utils -p=kernel -p=cpu -p=memoria -p=entradasalida "tp-2024-1c-CANCH"
```
Completar las [credenciales de acceso...](https://docs.utnso.com.ar/guias/herramientas/deploy#generar-las-credenciales)

Usuario:

Token:


```bash
cd ..
cd tp-2024-1c-CANCH
./hacerDirs.sh
./make.sh
```

nos fijamos las ips de cada maquina y modulo y:
```bash
./ips.sh
```

y configuramos las ips :D
