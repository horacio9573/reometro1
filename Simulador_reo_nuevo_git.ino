#include <Arduino.h>
#include <math.h>

/*
 * Definiciones de Constantes
 * NR: Número de registros (puntos de datos) a generar.
 * TR: Umbral de registros para el cambio de comportamiento (transición).
 * CT: Constante de tiempo utilizada en las funciones exponenciales.
 */
[cite_start]#define NR 4096  // Número de registros de datos a generar [cite: 1]
[cite_start]#define TR 1200  // Umbral de registros para el cambio de comportamiento [cite: 1]
[cite_start]#define CT 250.0 // Constante de tiempo para la respuesta exponencial [cite: 1]

// Arrays para almacenar los datos generados
float tiempo[NR], R[NR], T[NR]; [cite_start]// tiempo (microsegundos), variable R, variable T [cite: 1]
float paso_tiempo;             [cite_start]// Intervalo de tiempo entre puntos de datos (paso de tiempo) [cite: 1]
//String Buffer[NR+1];          // Buffer de datos (comentado en el código original)

int i; [cite_start]// Variable de índice para bucles [cite: 1]
// Variables globales para la función EOXXYY (Ondas)
double FRECUENCIA, FASE; [cite_start]// Frecuencia y Fase para las señales sinusoidales/cosenoidales [cite: 2]

void setup()
{
    // Inicializa la comunicación serial a 115200 baudios
    Serial.begin(115200);
}

// --------------------------------------------------------------------------------
// ## Funciones de Generación de Datos
// --------------------------------------------------------------------------------

/*
 * Genera datos de tipo "ECXX00" (Respuesta Exponencial de Crecimiento)
 *
 * @param rpm Valor que determina el encabezado de salida (70, 35, o 17).
 * También podría representar "Revoluciones Por Minuto" o un ID de perfil.
 */
void ECXX00(byte rpm)
{
    // Envía el encabezado de respuesta serial según el valor de 'rpm'
    switch (rpm)
    {
    case 70:
        [cite_start]Serial.print("#EC7000\n"); [cite: 2]
        [cite_start]break; [cite: 3]
    case 35:
        [cite_start]Serial.print("#EC3500\n"); [cite: 3]
        break;
    case 17:
        [cite_start]Serial.print("#EC1700\n"); [cite: 4]
        break;
    }
    
    int i;
    paso_tiempo = 81.29; [cite_start]// Paso de tiempo en microsegundos [cite: 5, 6]

    /* Crea los datos de Salida (Respuesta Exponencial de Crecimiento) */
    for (i = 0; i < NR; i++) 
    {
        [cite_start]tiempo[i] = paso_tiempo * i; [cite: 6] // Calcula el tiempo
        
        // Antes del tiempo de transición (TR): valores constantes
        [cite_start]if (i < TR) { [cite: 7]
            [cite_start]R[i] = T[i] = 100.0; [cite: 7, 8]
        }
        // Después del tiempo de transición (TR): Respuesta exponencial
        else {
            // R: Crecimiento exponencial asintótico a 150.0 (100.0 + 50.0)
            [cite_start]R[i] = 100.0 + 50.0 * (1 - exp(-1 * (i - TR) / CT)); [cite: 8]
            // T: Decrecimiento exponencial asintótico a 50.0 (100.0 - 50.0)
            [cite_start]T[i] = 100.0 - 50.0 * (1 - exp(-1 * (i - TR) / CT)); [cite: 9]
        }
        
        // Formatea y envía la fila de datos por Serial
        char temp[40];
        String fila;
        
        dtostrf(tiempo[i], 10, 2, temp); [cite_start]// tiempo (2 decimales) [cite: 10]
        [cite_start]fila = String(temp) + ";"; [cite: 11]
        
        dtostrf(R[i], 10, 6, temp); // R (6 decimales)
        fila += String(temp) + ";";
        
        dtostrf(T[i], 10, 6, temp); [cite_start]// T (6 decimales) [cite: 11]
        fila += String(temp) + ";"+ "200\n"; // Columna de dato constante "200"
        
        Serial.print(fila);
        
    }

    // Envía el mensaje de finalización de datos
    [cite_start]Serial.print("#M0OK00\n"); [cite: 12]
}

/*
 * Genera datos de tipo "EDXX00" (Respuesta Exponencial de Decrecimiento)
 *
 * @param rpm Valor que determina el encabezado de salida (70, 35, o 17).
 */
void EDXX00(byte rpm)
{
    // Envía el encabezado de respuesta serial según el valor de 'rpm'
    switch (rpm)
    {
    case 70:
        [cite_start]Serial.print("#ED7000\n"); [cite: 13]
        [cite_start]break; [cite: 14]
    case 35:
        [cite_start]Serial.print("#ED3500\n"); [cite: 14]
        break;
    case 17:
        [cite_start]Serial.print("#ED1700\n"); [cite: 15]
        [cite_start]break; [cite: 16]
    }
    
    int i;
    paso_tiempo = 81.29; [cite_start]// Paso de tiempo en microsegundos [cite: 17]

    /* Crea los datos de Salida (Respuesta Exponencial de Decrecimiento/Crecimiento) */
    // Nota: El bucle va hasta NR-1, a diferencia de ECXX00
    for (i = 0; i < NR-1; i++)
    {
        [cite_start]tiempo[i] = paso_tiempo * i; [cite: 17] // Calcula el tiempo
        
        // Antes del tiempo de transición (TR): valores constantes
        if (i < TR) 
        [cite_start]{ [cite: 18]
            [cite_start]R[i] = 150.0; [cite: 18, 19]
            [cite_start]T[i] = 50.0; [cite: 19]
        }
        // Después del tiempo de transición (TR): Respuesta exponencial
        else 
        {
            // R: Decrecimiento exponencial asintótico a 100.0 (150.0 - 50.0)
            [cite_start]R[i] = 150.0 - 50.0 * (1 - exp(-1 * (i - TR) / CT)); [cite: 20]
            // T: Crecimiento exponencial asintótico a 100.0 (50.0 + 50.0)
            [cite_start]T[i] = 50.0 + 50.0 * (1 - exp(-1 * (i - TR) / CT)); [cite: 21]
        }
        
        // Formatea y envía la fila de datos por Serial
        char temp[40];
        String fila;
        
        [cite_start]dtostrf(tiempo[i], 10, 2, temp); [cite: 21, 22]
        [cite_start]fila = String(temp) + ";"; [cite: 22]
        
        dtostrf(R[i], 10, 6, temp);
        fila += String(temp) + ";";
        
        [cite_start]dtostrf(T[i], 10, 6, temp); [cite: 23]
        fila += String(temp) + ";" + [cite_start]"200\n"; [cite: 23]
        
        Serial.print(fila);
    }

    // Envía el mensaje de finalización de datos
    [cite_start]Serial.print("#M0OK00\n"); [cite: 24]
}

/*
 * Genera datos de tipo "EOXXYY" (Respuesta Oscilatoria / Ondas Senoidales/Cosenoidales)
 *
 * @param rpm Valor que determina el perfil de frecuencia y paso de tiempo (5, 10, o 15).
 */
void EOXXYY(byte rpm)

{
    // Configura FRECUENCIA, FASE y paso_tiempo según el valor de 'rpm'
    switch (rpm)
    {
    case 5:
        //Serial.print("#EO7005\n"); // Comentado en el código original
        FRECUENCIA = 0.5; [cite_start]// Frecuencia en Hz [cite: 25]
        FASE = -0.21;     [cite_start]// Fase inicial en radianes [cite: 25]
        paso_tiempo = 3906.25; [cite_start]/* En micro segundos */ [cite: 25]
        
        for (i = 0; i < NR; i++)
        {
           [cite_start]tiempo[i] = paso_tiempo * i; [cite: 26]
           // R: Señal Senoidal con Offset 150.0 y Amplitud 75
           [cite_start]R[i] = 150.0 + 75 * sin(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 26]
           // T: Señal Cosenoidal con Offset 150.0 y Amplitud 25
           [cite_start]T[i] = 150.0 + 25 * cos(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 27]
           
           // Formatea y envía la fila de datos por Serial
           char temp[40];
           [cite_start]String fila; [cite: 28]
           dtostrf(tiempo[i], 10, 2, temp);
           fila = String(temp) + ";";
           dtostrf(R[i], 10, 6, temp);
           fila += String(temp) + ";";
           [cite_start]dtostrf(T[i], 10, 6, temp); [cite: 29]
           fila += String(temp) + ";" + "200\n";
           Serial.print(fila);
                
        }
       [cite_start]break; [cite: 30]
        
    case 10:
        //Serial.print("#EO7010\n"); // Comentado en el código original
        [cite_start]FRECUENCIA = 1.0; [cite: 30] // Frecuencia en Hz
        FASE = -0.65;     // Fase inicial en radianes
        paso_tiempo = 1953.125; [cite_start]/* En micro segundos */ [cite: 31]
        
        for (i = 0; i < NR; i++)
        {
            [cite_start]tiempo[i] = paso_tiempo * i; [cite: 32]
            // R: Señal Senoidal
            [cite_start]R[i] = 150.0 + 75 * sin(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 32]
            // T: Señal Cosenoidal
            [cite_start]T[i] = 150.0 + 25 * cos(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 33]
            
            // Formatea y envía la fila de datos por Serial
            [cite_start]char temp[40]; [cite: 33]
            [cite_start]String fila; [cite: 34]
            dtostrf(tiempo[i], 10, 2, temp);
            fila = String(temp) + ";";
            dtostrf(R[i], 10, 6, temp);
            fila += String(temp) + ";";
            [cite_start]dtostrf(T[i], 10, 6, temp); [cite: 35]
            fila += String(temp) + ";" + "200\n";
            Serial.print(fila);
        [cite_start]} [cite: 36]
        break;
  
    case 15:
        //Serial.print("#EO7015\n"); // Comentado en el código original
        [cite_start]FRECUENCIA = 1.5; [cite: 37] // Frecuencia en Hz
        FASE = -1.15;     // Fase inicial en radianes
        paso_tiempo = 976.5625; [cite_start]/* En micro segundos */ [cite: 37]
        
        for (i = 0; i < NR; i++)
        {
            [cite_start]tiempo[i] = paso_tiempo * i; [cite: 38]
            // R: Señal Senoidal
            [cite_start]R[i] = 150.0 + 75 * sin(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 38]
            // T: Señal Cosenoidal
            [cite_start]T[i] = 150.0 + 25 * cos(2 * M_PI * FRECUENCIA * tiempo[i] / 1.0e6 + FASE); [cite: 39]
            
            // Formatea y envía la fila de datos por Serial
            [cite_start]char temp[40]; [cite: 39]
            [cite_start]String fila; [cite: 40]
            dtostrf(tiempo[i], 10, 2, temp);
            fila = String(temp) + ";";
            dtostrf(R[i], 10, 6, temp);
            fila += String(temp) + ";";
            [cite_start]dtostrf(T[i], 10, 6, temp); [cite: 41]
            fila += String(temp) + ";" + "200\n";
            Serial.print(fila);
        [cite_start]} [cite: 42]
        break;
    }

    // Envía el mensaje de finalización de datos
    [cite_start]Serial.print("#M0OK00\n"); [cite: 43]
}

// --------------------------------------------------------------------------------
// ## Función Principal (loop)
// --------------------------------------------------------------------------------

void loop()
{
    // Bucle principal para verificar y procesar datos seriales entrantes
    while (Serial.available())
    {
        // Esto parece ser un eco o señal de actividad, aunque el flujo no lo permite ver en la implementación original
        [cite_start]Serial.print('.'); [cite: 44]
        // Se llama a ECXX00(70) al inicio del while, esto parece ser un error o comportamiento no deseado
        [cite_start]// ECXX00(70); [cite: 44] 
        
        // Lee la cadena de datos hasta un salto de línea
        [cite_start]String data = Serial.readStringUntil('\n'); [cite: 44]
        
        // El bucle original tenía un ECXX00(70) antes de la lectura, se ha comentado para evitar doble ejecución no deseada
        // y se mantienen solo las llamadas dentro de los 'if'.

        // Procesamiento de comandos para EC (Crecimiento Exponencial)
        if (data == "EC7000")
        {
            [cite_start]ECXX00(70); [cite: 45]
        }
        if (data == "EC3500")
        {
            [cite_start]ECXX00(35); [cite: 50]
        }
        if (data == "EC1700")
        {
            [cite_start]ECXX00(17); [cite: 55]
        }

        // Procesamiento de comandos para ED (Decrecimiento/Crecimiento Exponencial)
        if (data == "ED7000")
        {
            [cite_start]EDXX00(70); [cite: 46]
        }
        if (data == "ED3500")
        {
            [cite_start]EDXX00(35); [cite: 51]
        }
        if (data == "ED1700")
        {
            [cite_start]EDXX00(17); [cite: 56]
        }

        // Procesamiento de comandos para EO (Ondas Oscilatorias)
        // Se envía un eco del comando recibido y luego se genera la onda
        
        // EOXX05 (0.5 Hz)
        if (data == "EO7005")
        {
            [cite_start]Serial.print("#EO7005\n"); [cite: 47]
            [cite_start]EOXXYY(05); [cite: 47]
        }
        if (data == "EO3505")
        {
            [cite_start]Serial.print("#EO3505\n"); [cite: 52]
            [cite_start]EOXXYY(05); [cite: 52]
        }
        if (data == "EO1705")
        {
            [cite_start]Serial.print("#EO1705\n"); [cite: 57]
            [cite_start]EOXXYY(05); [cite: 57]
        }

        // EOXX10 (1.0 Hz)
        if (data == "EO7010")
        {
            [cite_start]Serial.print("#EO7010\n"); [cite: 48]
            [cite_start]EOXXYY(10); [cite: 48]
        }
        if (data == "EO3510")
        {
            [cite_start]Serial.print("#EO3510\n"); [cite: 53]
            [cite_start]EOXXYY(10); [cite: 53]
        }
        if (data == "EO1710")
        {
            [cite_start]Serial.print("#EO1710\n"); [cite: 58]
            [cite_start]EOXXYY(10); [cite: 58]
        }

        // EOXX15 (1.5 Hz)
        if (data == "EO7015")
        {
            [cite_start]Serial.print("#EO7015\n"); [cite: 49]
            [cite_start]EOXXYY(15); [cite: 49]
        }
        if (data == "EO3515")
        {
            [cite_start]Serial.print("#EO3515\n"); [cite: 54]
            [cite_start]EOXXYY(15); [cite: 54]
        }
        if (data == "EO1715")
        {
            [cite_start]Serial.print("#EO1715\n"); [cite: 59]
            [cite_start]EOXXYY(15); [cite: 59]
        }

        // Procesamiento de comandos de control (M, A, Z)
        if (data == "M00000")
        {
            [cite_start]Serial.print("#M00000\n"); [cite: 60]
        }
        if (data == "M07000")
        {
            [cite_start]Serial.print("#M07000\n"); [cite: 61]
        }
        if (data == "M03500")
        {
            [cite_start]Serial.print("#M03500\n"); [cite: 62]
        }
        if (data == "M01700")
        {
            [cite_start]Serial.print("#M01700\n"); [cite: 63]
        }
        if (data == "A")
        {
            [cite_start]Serial.print("#A\n"); [cite: 64]
        }
        if (data == "Z")
        {
            [cite_start]Serial.print("#Z\n"); [cite: 65]
        }
    }
}
