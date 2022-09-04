#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"
#include "nrf24_driver.h"
#include "hardware/gpio.h"

//Variables para la SD
FRESULT fr;
FATFS fs;
FIL fil_acel;
FIL fil_gyro;
FIL fil_magnet;
FIL fil_angles;
int ret;
char buf[100];
char filename[] = "acelData.txt";
char filename2[] = "gyroData.txt";
char filename3[] = "magnetData.txt";
char filename4[] = "anglesData.txt";
char str[100];

//Funciones
void sys_init(void);
void sys_stop(void);

//Pines
const uint interruptPin = 22;

int main()
{
    sys_init();

    //Archivos donde se guardaran los datos
    sd_openfileW(fr, &fil_acel, filename);
    sd_openfileW(fr, &fil_gyro, filename2);
    sd_openfileW(fr, &fil_magnet, filename3);
    sd_openfileW(fr, &fil_angles, filename4);

    while(1){
        //Escribiendo datos del acelerometro
        sprintf(str, "Escribiendo en el archivo del acelerometro\n");
        sd_writefile(ret, &fil_acel, str);

        //Escribiendo datos del giroscopio
        sprintf(str, "Escribiendo en el archivo del giroscopio\n");
        sd_writefile(ret, &fil_gyro, str);

        //Escribiendo datos del magnetometro
        sprintf(str, "Escribiendo en el archivo del magnetometro\n");
        sd_writefile(ret, &fil_magnet, str);

        //Escribiendo angulos
        sprintf(str, "Escribiendo en el archivo de los angulos\n");
        sd_writefile(ret, &fil_angles, str);

        if(gpio_get(interruptPin) == 1){ //Haciendo polling al botón de parar de guardar los datos (PENDIENTE POR INTERRUPCION)
            sys_stop();
        }

        sleep_ms(100);
    }
}

void sys_init(void){
    stdio_init_all();
    gpio_init(interruptPin);
    gpio_set_dir(interruptPin, GPIO_IN);

    sleep_ms(6000);
    //Inicializar todo el apartado de la SD
    initialize_sd();
    mount_drive(fr, &fs);
}

void sys_stop(void){
    sd_closefile(fr, &fil_acel);
    sd_closefile(fr, &fil_gyro);
    sd_closefile(fr, &fil_magnet);
    sd_closefile(fr, &fil_angles);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("Datos escritos");
    while (1);
}