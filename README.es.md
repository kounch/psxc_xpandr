# psxc_xpandr
Un método seguro para expandir y/o reemplazar virtualmente juegos de una PlayStation Classic usando un disco externo USB, sin riesgo. Una vez que se retira el disco USB y se hace un apagado y encendio completo, la consola funciona como si no se hubiera hecho nada.

Características
* Modificación mínima del almacenamiento interno de la PlayStation Classic
* Puede mantener o reemplazar virtualmente los juegos instalados originalmente (cargándolos desde USB en vez de la memoria interna)
* Puede añadir nuevos juegos (que se cargan también desde la unidad USB)
* Extrae automáticamente los ficheros necesarios cuando se utiliza en una unidad USB vacía
* Añade soporte para acceder al menú del emulador de la consola usando simultáneamente los botones `Select` y `Triángulo` de un mando
* Opcionalmente compatible con dos mandos usando un hub USB
* Soporte opcional para modificar la interfaz gráfica
* Soporte opcional para cheatpops.db

*Lea esto en otros idiomas: [English](README.md), [Español](README.es.md).*


## Instalación

Formatee un disco USB compatible usando el sistema de archivos FAT32, y ponga el nombre SONY.

Descargue la última versión del software [desde aquí](https://github.com/kounch/psc_xpandr/releases/latest). Extraiga el contenido, y copie, al menos, la carpeta `691843bb-62d6-4423-a105-19c06af91a8c` a la raíz de la unidad.

Copie los archivos de los juegos en carpetas numeradas, dentro de otra carpeta llamada `games` en la raíz de la unidad, tal y como se explica [más adelante en este texto](#GameINI).

## Uso

### Primer Uso

La primera vez que se utiliza con una unidad USB vacía, las siguientes carpetas y ficheros se copian desde el almacenamiento interno a la unidad USB:
```
/
|
+-data
|  +-GR
|  |  +-Acid_B_BTN.png
|  |  (...)
|  |  +-X_Btn_ICN.png
|  |
|  +-system
|    +-20-joystick.rules
|    +-custom.pre
|    +-pcsx
|    +-sonyapp-copylink
|    (...)
|
+-games
    +-custom.db
```

Estos archivos se pueden modificar para cambiar la interfaz de usuario, añadir soporte para un segundo mando de juego si se utiliza un hub USB, etc. Siga leyendo para ver más instrucciones.

### Personalización

#### <a name="GameINI"></a>Añadir un nuevo juego utilizando archivos BIN/CUE y un fichero `Game.ini`
Dentro de `games` cree una nueva carpeta con un identificador numérico (`GAME_ID`), que ha de ser único. Utilice un número entre 1 y 20 si desea reemplazar virtualmente un juego existente. Utilice un número mayor de 20 si desea añadir virtualmente un juego nuevo. Para los juegos nuevos, procure no dejar huecos en la numeración de `GAME_ID` (por ej. usar 21,22,23... y no 21,25...).

Dentro de dicha carpeta, ha de poner al menos los siguientes archivos:
* `BASENAME.cue`: Fichero CUE del disco (normalmente utilice como `BASENAME` el identificador del disco original)
* `BASENAME.bin`: Fichero BIN del disco (ha de tener el mismo nombre `BASENAME` que el fichero CUE)
* `BASENAME.png`: Imagen PNG image de menos de 512x521 pixels de tamaño (ha de seguir usando el mismo `BASENAME`)
* `pcsx.cfg`: Fichero de configuración del juego PCSX (un fichero de ejemplo está disponible para descarga [aquí](https://raw.githubusercontent.com/kounch/psxc_xpandr/master/pcsx.cfg))
* `Game.ini`: Fichero de descripcion del juego con el siguiente formato:

```
[Game]
Discs=...
Title=...
Publisher=...
Players=...
Year=...
```

Donde cada línea debe completarse de la siguiente manera:

| Campo     | Descripción                                                           |
|-----------|-----------------------------------------------------------------------|
| Discs     | Nombre (en orden) del fichero o ficheros BIN/CUE, separados por comas |
| Title     | Título del juego                                                      |
| Publisher | Editor del juego                                                      |
| Players   | Número de jugadores (1 ó 2)                                           |
| Year      | Año de lanzamiento del juego                                          |

Por ejemplo, para un juego llamado "Res Evl 2", con dos discos con ficheros BIN/CUE llamados `SUES-00972.bin`, `SUES-00972.cue`, `SUES-10972.bin`, `SUES-10972.cue`, de un jugador y publicado en 1997 por "Pepi Interactive", sería así:
```
[Game]
Discs=SUES-00972,SUES-10972
Title=Res Evl 2
Publisher=Pepi Interactive
Players=1
Year=1997
```

#### Alterar la interfaz de usuario

Edite los archivos en la carpeta `data/GR` . Por ejemplo, para cambiar la imagen del fondo, cambie el fichero `JP_US_BG.png`.

#### Añadir soporte para un segundo mando usando un hub USB

Cambie el archivo `data/system/20-joystick.rules`, reemplazando el contenido por

    KERNEL=="js0",SUBSYSTEMS=="input",SYMLINK+="input/joystick0"
    KERNEL=="js1",SUBSYSTEMS=="input",SYMLINK+="input/joystick1"

#### Aumentar el máximo de juegos a más de 25

Modifique el archivo `data/system/sonyapp-copylink`, cambiando el número 25 de la segunda línea (`COUNT_MAX=25`) por otro número mayor.

#### Añadiendo códigos de trampas

Copie o cree un fichero `cheatpops.db` dentro de la carpeta `/games` de la unidad USB.

#### Usando una versión personalizada del emulador pcsx

Puede sustituir por otro el fichero ejecutable del emulador extraido en `data/system/pcsx`, usando, por ejemplo, uno que tenga aplicado el parche mipmap_patch (ver [aquí](https://www.reddit.com/r/PlaystationClassic/comments/a7uhwm/remove_default_bilinear_filter/) para más información).

#### Cambiar o quitar la imagen de carga

Se puede modificar la imagen de carga cambiando el fichero `splash.jpg` de la carpeta `691843bb-62d6-4423-a105-19c06af91a8c`. También se puede hacer que no aparezca ninguna imagen si se borra el fichero.

#### Añadir un juego nuevo editando manualmente `custom.db`
Si se prefiere, es posible añadir juegos uno mismo a la base de datos. En este caso, no cree un fichero `Game.ini` file y modifique `games/custom.db` en el programa editor de SQLite que prefiera, añadiendo nuevas entradas a la tabla `GAME` de la siguiente manera:

| Campo                | Descripción del contenido               |
|----------------------|-----------------------------------------|
| GAME_ID              | Identificador numérico, ha de ser único |
| GAME_TITLE_STRING    | Nombre del juego                        |
| PUBLISHER_NAME       | Nombre del editor del juego             |
| RELEASE_YEAR         | Año de lanzamiento del juego            |
| PLAYERS              | Número de jugadores                     |
| RATING_IMAGE         | Ha de ser `CERO_A`                      |
| GAME_MANUAL_QR_IMAGE | Ha de ser `QR_Code_GM`                  |
| LINK_GAME_ID         | Debe estar vacío                        |

A continuación añada las entradas correspondientes para cada disco de cada juego en la tabla `DISC`:

| Campo        | Descripción del contenido                             |
|--------------|-------------------------------------------------------|
|  GAME_ID     | GAME_ID de la tabla GAME                              |
|  DISC_NUMBER | Número del disco                                      |
|  BASENAME    | Nombre los ficheros cue/bin, sin la extensión         |

Por último, se han de añadir los ficheros de juego. Dentro de la carpeta `games` cree una nueva carpeta con el identificador numérico (`GAME_ID`) de los pasos anteriores.

Dentro de dicha carpeta, ha de poner, al menos, los siguientes archivos:
* `BASENAME.cue`: Fichero CUE del disco (utilice el valor `BASENAME` de la tabla `DISC`)
* `BASENAME.bin`: Fichero BIN del disco (ha de tener el mismo nombre `BASENAME` que el fichero CUE)
* `BASENAME.png`: Imagen PNG image de menos de 512x521 pixels de tamaño (ha de seguir usando el mismo `BASENAME`)
* `pcsx.cfg`: Fichero de configuración del juego PCSX (un fichero de ejemplo está disponible para descarga [aquí](https://raw.githubusercontent.com/kounch/psxc_xpandr/master/pcsx.cfg))

#### Reemplazar un juego existente cambiando `custom.db`
Para reemplazar un juego que ya exista, edite `games/custom.db` tal y como se explica anterioremente, modificando los datos existentes las tablas `GAME` y `DISC`, manteniendo el formato.

Después, dentro de la carpeta `games`, cree una nueva carpeta y añada todos los ficheros necesarios.

## Créditos

Basado en:
* lolhack, un lanzador creado por [madmonkey](https://github.com/madmonkey1907)
* La versión original de [gpghax de justMaku](https://github.com/justMaku/gpghax)
* Acceso al menú del emulador usando Select + Triangle del mando, creado por andshrew
* PCSX ReARMed, versión para ARM del emulador PCSX por [Notaz](https://notaz.gp2x.de) (información sobre cheatpops.db)
* Explicación del usuario rubixcube6 en Reddit de la [creación de temas personalizados](https://redd.it/a5g5kx)
* Explicación del usuario NonyaDB en Reddit para [aumentar el límite de 25 juegos](https://www.reddit.com/r/PlaystationClassic/comments/a44ka6/add_custom_games_on_usb_storage_with_gpghax/ebci4hg/)
* [Instrucciones para compilar de forma cruzada para ARM](https://neerajcodes.wordpress.com/2017/08/29/toolchain-cross-compilation-using-crosstool-ng/) por Neeraj Kumar
* [Instrucciones para compilar de forma cruzada SQLite3](https://vicente-hernando.appspot.com/sqlite3-cross-compile-arm-howto) de Vicente Hernando

## Preguntas frecuentes

### ¿Modifica esto mi consola?

El script intenta mantener al mínimo el número de archivos y directorios modificados en el almacenamiento interno de la consoa, montando de forma virtual la totalidad de la información desde la unidad USB. Sin embargo, existe un mínimo de datos se han de crear para que funcione.

Estos son los archivos o directorios que el script crea en el almacenamiento interno:
* Para cualquier carpeta que haya en `/games`, si no existe, se crea un directorio vacío con el mismo nombre en el almacenamiento interno en `/gaadata`
* Para cualquier carpeta que haya en `/games`, si no existe, se crea un directorio vacío con el mismo nombre en `/data/AppData/sony/pcsx/`
* Para cualquier carpeta que haya en `/games`, se crea un enlace simbólico con el nombre `.pcsx/pcsx.cfg` dentro del directorio con el mismo nombre en `/data/AppData/sony/pcsx/`, y apuntando al fichero `pcsx.cfg` de la unidad USB.
* Si se añade un fichero `cheatpops.db`, se crea un fichero vacío con el mismo nombre dentro del directorio `/data/AppData/sony/pcsx`.

### ¿Puede estropear mi consola?

Las probabilidades son casi nulas, ya que no se altera ningún archivo original de la consola. Se ha puesto mucho esfuerzo para que sea lo más seguro posible, sin embargo, no se ofrece ninguna garantía y se renuncia a toda obligación y responsabilidad por cualquier daño que pueda sufrir su consola al utilizarlo. Por favor, no use este software si no está seguro de lo que está haciendo.

### ¿Cómo funciona?

El archivo `LUPDATA.bin` que está dentro de la carpeta `691843bb-62d6-4423-a105-19c06af91a8` es un script de Shell (código fuente disponible [aquí](https://github.com/kounch/psxc_xpandr)), cifrado y firmado de tal manera que el sistema interno de actualizaciones de una PlayStation Classic original lo reconoce, lo copia a un directorio temporal y lo ejecuta.
A continuación, el script para el menú de la consola, monta virtualmente ficheros y carpetas del disco USB, reconfigura temporalmente el sistema, editando si fuera necesario la copia realizada de la base de datos, utilizando el ejecutable de sqlite3 incorporado, y lanza de nuevo el programa del menú principal.
De esta manera, si se apaga completamente la PlayStation Classic y se vuelve a encender sin la unidad USB, todo debería funcionar como si no se hubiera realizado ninguna modificación.

### He borrado una carpeta de un juego, pero todavía aparece

Si está utilizando ficheros `Game.ini`, simplemente borre el archivo `custom.db` del disco USB y apague completamente, desconectando el cable de alimentación. Conecte de nuevo el cable de alimentación y vuelva a encender la consola con la unidad USB conectada. La base de datos de juegos se volverá a crear desde cero, usando sólo las carpetas existentes. En otro caso, si modificó manualmente el archivo de base de datos, tendrá que eliminar los datos correspondientes de las tablas `GAME` y `DISC`.

### ¿Se modifica el fichero `custom.db` todas las veces que se utiliza psxc_xpandr?

Sí, siempre que el script encuentra un archivo `Game.ini`, se edita el fichero de base de datos según la información encontrada. Si no desea que sea así, puede borrar o renombrar los ficheros `Game.ini` de cada juego afectado.

## Copyright

Copyright (c) 2019, kounch
Todos los derechos reservados.

La redistribución y uso en forma de código fuente o binario, con o sin
modificaciones, están permitidas siempre y cuando se cumplan las siguientes
condiciones:

* La redistribución del código fuente debe contener el aviso de copyright,
esta lista de condiciones y el siguiente texto.

* La redistribución en formato binario debe reproducir el aviso de copyright, 
esta lista de codiciones y el siguiente texto en la documentación y/o en otros
materiales que acompañen a la distribución.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
