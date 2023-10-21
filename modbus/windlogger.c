
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <wiringSerial.h>
#include <wiringPi.h>

#include <modbus.h>

#define EN_485 4

#define LOOP          1
#define SERVER_ID     0xFF
#define WINDSPEED_REGISTER 0x000C
#define WINDDIRECTION_REGISTER 0x000D

float read_windspeed(modbus_t *ctx, uint16_t *tab_rq_registers) {
  int nb = 20;
  float windspeed = 0;
  int speedX100;
  int rc = modbus_read_registers(ctx, WINDSPEED_REGISTER, 2, tab_rq_registers);
  if (rc == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    windspeed = -1;
  } else {
    speedX100 = (int) tab_rq_registers[0];
    windspeed = speedX100 / 100.0;
  }
  memset(tab_rq_registers, 0, nb * sizeof(uint16_t));
  return windspeed;
}


float read_direction(modbus_t *ctx, uint16_t *tab_rq_registers) {
  int nb = 20;
  float direction = 0;
  int dirX10;
  int rc = modbus_read_registers(ctx, WINDDIRECTION_REGISTER, 2, tab_rq_registers);
  if (rc == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    direction = -1;
  } else {
    dirX10 = (int) tab_rq_registers[0];
    direction = dirX10 / 10.0;
  }

  memset(tab_rq_registers, 0, nb * sizeof(uint16_t));
  return direction;
}

int main(void)
{
    modbus_t *ctx;
    int nb;
    uint16_t *tab_rq_registers;
    float windspeed;
    float winddirection;
    char linebuf[120];

    if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
        printf("set wiringPi lib failed !!! \r\n");
        return 1;
    } else {
        printf("set wiringPi lib success  !!! \r\n");
    }
    pinMode(EN_485, OUTPUT);
    digitalWrite(EN_485,HIGH);

    // int fd;
    // if((fd = serialOpen ("/dev/ttyAMA0",9600)) < 0) {
    //    printf("serial err\n");
    //     return -1;
    // }

    // serialFlush(fd);
    // serialPrintf(fd,"\r");

    int fd_log = open("wind2023i1018.txt", O_RDWR);
    /* RTU */
    ctx = modbus_new_rtu("/dev/ttyS0", 9600, 'N', 8, 1);
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    modbus_set_slave(ctx, SERVER_ID);
    modbus_set_response_timeout(ctx, 1, 0);
    // modbus_set_debug(ctx, TRUE);

    digitalWrite(EN_485,HIGH);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    digitalWrite(EN_485,LOW);

    /* Allocate and initialize the different memory spaces */
    nb = 20;

    tab_rq_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rq_registers, 0, nb * sizeof(uint16_t));

    while(TRUE) {	
      winddirection = read_direction(ctx, tab_rq_registers);
      sleep(1);
      windspeed = read_windspeed(ctx, tab_rq_registers);
      memset(tab_rq_registers, 0, nb * sizeof(uint16_t));
      time_t rawtime;
      time ( &rawtime );
      sprintf(linebuf, " %f,   %f,  %s,", windspeed, winddirection, ctime(&rawtime));
      write(fd_log, linebuf, strlen(linebuf));
      printf("%s", linebuf);
      sleep(1);
      sleep(3);
    }
    /* Free the memory */
    free(tab_rq_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
