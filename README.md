# TAPINATOR 9KPP

Clasificadora de tapitas por color basada en Arduino desarrollada como proyecto
para la Universidad Católica del Uruguay. Este proyecto se realiza para la clase
de Taller Interdiciplinario de Introducción a la Ingenieria (TI3) en el 1er semestre
del 2021.

Aqui encontrara una descripcion del diseño final, incluyendo los componentes usados
y su ensamblaje para llegar a la unidad prototipo.

### Concepto

La clasificadora utiliza como cerebro a un Arduino UNO. Por medio de un sensor
de color, un micro servomotor, varios motores DC, y un motor paso a paso, este
realiza todo el proceso de clasificacion.

1. Las tapitas son introducidas en un contenedor en la parte superior.
2. Estas caen por una rampa con orificios con la medida justa para solo dejar pasar las tapitas que se pueden clasificar.
3. Las tapitas seleccionadas son revueltas por un motor DC hasta que caigan una por una en la posicion correcta por un pasaje.
4. Cada tapita cae en un disco operado por el motor paso a paso con 12 orificios.
5. El disco gira una posicion (1/12 de vuelta) en sentido horario, posicionando la tapita que cayo en frente al sensor de color.
6. En esta nueva posicion, el sensor lee el color de la tapita.
7. El servo motor gira para preparar el camino a seguir (en base del color) de la tapita procesada.
8. El disco gira una posicion mas, dejando a la tapita sobre un orificio por el cual esta cae.
9. La tapita cae y viaja por el camino seleccionado con el servomotor.
10. Como paso final, la tapita cae por el orificio del color leido, terminando en el recipiente apropiado.

### Materiales

Los materiales utilizados para la unidad prototipo final fueron placas de MDF cortadas
por laser, carton duro, y piezas de diseño 3D para impresion. Para el diseño y sus
medidas se especifico un grosor de 3mm para todas las placas y laminas.

### Piezas 3D

### Placas

### Componentes

Se utilizaron varios componentes electronicos obtenidos del kit inicial provisto por la Universidad,
mas otros adicionales obtenidos individualmente por el equipo. Entre estos se encuentran:

- Arduino UNO
- Protoboard/Breadboard
- Switch tactil x 4
- Resistencia de 10K Ohm x 4
- Resistencia de 200 Ohm x 1
- Resistencia de 1M Ohm x 1
- LED RGD x 1
- Buzzer x 1
- Sensor de color TCS34725 x 1
- Pantalla LCD de 1602 + Modulo I2C x 1
- Micro Servomotor SG90 x 1
- Motor Paso a Paso 28byj-48 + Driver ULN2003 x 1
- Codificador rotatorio KY-040 x 1
- Relay de 5V con 1 canal x 1
- Motor DC de 3V x 4
- Placa de PCB x 1
- Switch de 2 posiciones x 1
- Modulo de reduccion CC-CC (Convertidor XL7015 V2 DC-DC)
- Transformador 220AC a 12V 2A x 1

Y muchos cablecitos.

### Circuito

El circuito utilizado fue el siguiente:

![Diagrama](/resources/img/circuit.png)

### Programacion del Arduino

Debido a haber varias funcionalidades necesarias para realizar el procedimiento
de clasificacion, se busco separar cada una y convertirla en una biblioteca a la 
cual referenciar luego desde el sketch principal.

Las librerias desarrolladas fueron:
- common_stuff (Funciones, enumerables, clases, y etc. en comun para todo el programa)
- sensor_control (control del sensor de color)
- stepper_control (control del motor paso a paso)
- servo_control (control del micro servo motor)
- rgb_control (control de LEDs RGB)
- simple_output_control (control de estado para pines de salida, usado para leds, reles, y buzzers)
- display_control (control del display LCD)
- menu_control (control del menu del programa)
- sorter_control (control del script de clasificacion automatica e interface entre el menu y los componentes)

Adicionalmente se utilizaron las librerias de 3er siguientes:
- AccelStepper
- LiquidCrystal_I2C
- PROGMEM_readAnything
- Servo
- Arduino

Por ultimo, la Universidad nos provio de las siguientes librerias para operar el sensor de color:
- COLOR
- Adafruit_TCS34725
- ColorConverterLib.h

### Librerias

* `common_stuff.h`: Presenta funciones, enumeraciones, y estructuras de uso comun para todo el programa.
  * Funciones:
    * `char* ConvertColorCategoryToChar(ColorCategory)`: Convierte un dato ColorCategory a c-string.
    * `char* ConvertRGBColorToChar(RGBColor)`: Convierte un dato RGBColor a c-string.
  * Clases:
    *  `ButtonState()`: Maneja el estado de una boton asociado a un pin usando attach(int pin, int debounce_delay);
  * Enumeraciones:
    * `MenuUserActions`: Botones disponibles.
    * `RotationDirection`: A reloj o contra. Para el paso a paso.
    * `ButtonAction`: Estado de un boton.
    * `TextAlignment`: Justificacion para textos.
    * `SensorAuxLEDMode`: Modo de operacion del LED del sensor.
    * `RGBColors`: Colores para LEDs RGB.
    * `SorterActions`: Acciones individuales del sistema.
    * `SorterPrograms`: Programas de operacion que se pueden ejecutar.
    * `ColorCategories`: Categorias de colores a clasificar.
    * `MenuActions`: Acciones disponibles para el boton 'entrar' en el menu.

* `stepper_control.h`: Definicion de clases asociadas al motor paso a paso.
  * Clases:
    * `StepperControl(int, int, int, int)`: Controla e inicializa el motor.
      * `initialize()`: Inicializa el motor con maxima velocidad en 500 y posicion 0.
      * `processState()`: Opera el motor segun el estado del objecto.
      * `doCycling()`: Cambia el estado para realizar el ciclaje continuo del motor.
      * `doCapStep()`: Cambia el estado para realizar un giro de 30º.
      * `stopActions()`: Cambia el estado para detener todas las acciones.
      * `getCurrentAction()`: Devuelve la accion actual.
      * 'isBusy()': Devuelve verdadero si el estado actual es de descanzo.
  * Enumeraciones:
    * `StepperActions`: Operaciones que puede realizar el motor paso a paso.

* `simple_output_control.h`: Operacion de digital outputs.
  * Clase:
    * `SOutputControl(int)`: Definicion, inicializacion, y operacion de digtal outputs.
      * `initialize()`: Inicializa la salida en el pin y lo configura.
      * `processState()`: Activa o desactiva el pin segun el estado dle objeto.
      * `on()`: Cambia el estado a HIGH.
      * `off()`: Cambia el estado a LOW.


### Ensamblado

### Operacion


