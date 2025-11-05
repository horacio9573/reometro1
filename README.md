# reometro1
Código para emulador de un reómetro eritrositario en un ESP32
# Simulador de Reómetro para ESP32

## 📝 Descripción del Proyecto

Este proyecto es un **simulador de comportamiento de reómetro** (o dispositivo de medición similar) implementado en un **ESP32**. Su función principal es **generar secuencias de datos de prueba** (Tiempo, Variable R y Variable T) con diferentes patrones de respuesta (exponencial de crecimiento, exponencial de decrecimiento y oscilatorio sinusoidal/cosenoidal) y enviarlos a través de la comunicación serial a un sistema de adquisición o interfaz de usuario.

El simulador responde a comandos seriales específicos para iniciar la generación de diferentes perfiles de datos.

## ⚙️ Instalación

1.  **Software Requerido:** Necesitarás el **IDE de Arduino** instalado.
2.  **Hardware:** Un **ESP32** (o compatible).
3.  **Carga del Código:**
    * Abre el archivo `Simulador_reo_nuevo.ino` en el IDE de Arduino.
    * Selecciona la placa correcta (`Arduino Nano`) y el puerto serial.
    * Sube el código a la placa.
4.  **Configuración Serial:** La comunicación serial está configurada a **9600 baudios**.

## 🚀 Uso y Comandos

El Arduino Nano espera comandos a través del puerto serial. Al recibir uno de los comandos listados a continuación, genera y transmite el conjunto de **4096 registros** de datos correspondiente.

Los comandos se estructuran en el formato `[TIPO][XX][YY]`, donde:
* **[TIPO]**: Tipo de respuesta (`EC`, `ED`, `EO`).
* **[XX]**: Identificador de velocidad/perfil (70, 35, 17).
* **[YY]**: Parámetro adicional (00 para exponencial, 05, 10, 15 para oscilatorio).

### 1. Comandos de Respuesta Exponencial de Crecimiento (`ECXX00`)

Genera una curva exponencial donde **R aumenta** y **T disminuye** después del registro 1200.

| Comando | Descripción |
| :--- | :--- |
| **EC7000** | Crecimiento Exponencial (Perfil 70) |
| **EC3500** | Crecimiento Exponencial (Perfil 35) |
| **EC1700** | Crecimiento Exponencial (Perfil 17) |

### 2. Comandos de Respuesta Exponencial de Decrecimiento (`EDXX00`)

Genera una curva exponencial donde **R disminuye** y **T aumenta** después del registro 1200.

| Comando | Descripción |
| :--- | :--- |
| **ED7000** | Decrecimiento Exponencial (Perfil 70) |
| **ED3500** | Decrecimiento Exponencial (Perfil 35) |
| **ED1700** | Decrecimiento Exponencial (Perfil 17) |

### 3. Comandos de Respuesta Oscilatoria (`EOXXYY`)

Genera una curva basada en funciones trigonométricas (**R** senoidal, **T** cosenoidal). La señal se genera con un offset de 150.0.

| Comando | Frecuencia (Hz) | Paso de Tiempo ($\mu s$) |
| :--- | :--- | :--- |
| **EOX X05** | 0.5 Hz | 3906.25 |
| **EOX X10** | 1.0 Hz | 1953.125 |
| **EOX X15** | 1.5 Hz | 976.5625 |

*(Donde XX puede ser 70, 35 o 17. Ejemplo: `EO7015`)*

### 4. Comandos de Control (Eco)

Estos comandos simplemente responden con un eco al puerto serial, indicando que el comando fue recibido.

| Comando | Respuesta Serial |
| :--- | :--- |
| **M00000** | `#M00000\n` |
| **M07000** | `#M07000\n` |
| **M03500** | `#M03500\n` |
| **M01700** | `#M01700\n` |
| **A** | `#A\n` |
| **Z** | `#Z\n` |

---

## 📊 Formato de Datos de Salida

Una vez que se ejecuta un comando, el simulador envía primero un **encabezado** y luego **4096 filas de datos** seguidas de un **mensaje de finalización**.

### Estructura de la Fila de Datos

Cada fila de datos enviada por serial sigue el siguiente formato delimitado por punto y coma (`;`):
