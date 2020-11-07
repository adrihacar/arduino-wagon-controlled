/**********************************************************
 *  INCLUDES
 *********************************************************/
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <rtems.h>
#include <bsp.h>

#ifdef RASPBERRYPI
#include <bsp/i2c.h>
#endif

#include "displayA.h"

/**********************************************************
 *  Constants
 **********************************************************/
#define MSG_LEN    8
#define SLAVE_ADDR 0x8

/**********************************************************
 *  Global Variables
 *********************************************************/
float speed = 0.0;
struct timespec time_msg = {0,400000000};
int fd_i2c = -1;
int mix = 0;
int gas = 0;
int brk = 0;


/**********************************************************
 *  Function: task_mix
 *********************************************************/
int task_mix()
{
    char request[10];
    char answer[10];

    //--------------------------------
    //  request speed and display it
    //--------------------------------

    //clear request and answer
    memset(request, '\0', 10);
    memset(answer, '\0', 10);

    // request gas
    if (mix == 0){
    	strcpy(request, "MIX: SET\n");
    	mix = 1;
    }else{
    	strcpy(request, "MIX: CLR\n");
    	mix = 0;
    }

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif

    // display speed
    //if (1 == sscanf (answer, "SPD:%f\n", &speed)){
        displayMix(mix);
    //}
    return 0;
}


/**********************************************************
 *  Function: task_gas
 *********************************************************/
int task_gas()
{
    char request[10];
    char answer[10];

    //--------------------------------
    //  request speed and display it
    //--------------------------------

    //clear request and answer
    memset(request, '\0', 10);
    memset(answer, '\0', 10);

    // request gas
    if (speed < 50){
    	strcpy(request, "GAS: SET\n");
    	gas = 1;
    }else{
    	strcpy(request, "GAS: CLR\n");
    	gas = 0;
    }

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif

    // display speed
    //if (1 == sscanf (answer, "SPD:%f\n", &speed)){
        displayGas(gas);
    //}
    return 0;
}

/**********************************************************
 *  Function: task_brk
 *********************************************************/
int task_brk()
{
    char request[10];
    char answer[10];

    //--------------------------------
    //  request speed and display it
    //--------------------------------

    //clear request and answer
    memset(request, '\0', 10);
    memset(answer, '\0', 10);

    // request gas
    if (60 < speed){
    	strcpy(request, "BRK: SET\n");
    }else{
    	strcpy(request, "BRK: CLR\n");
    }

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif

    // display speed
    //if (1 == sscanf (answer, "SPD:%f\n", &speed)){
        displayBrake(brk);
    //}
    return 0;
}


/**********************************************************
 *  Function: task_speed
 *********************************************************/
int task_speed()
{
    char request[10];
    char answer[10];

    //--------------------------------
    //  request speed and display it
    //--------------------------------

    //clear request and answer
    memset(request, '\0', 10);
    memset(answer, '\0', 10);

    // request speed
    strcpy(request, "SPD: REQ\n");

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif

    // display speed
    if (1 == sscanf (answer, "SPD:%f\n", &speed)){
        displaySpeed(speed);
    }
    return 0;
}

//-------------------------------------
//-  Function: task_slope
//-------------------------------------
int task_slope()
{
    char request[10];
    char answer[10];

    //--------------------------------
    //  request slope and display it
    //--------------------------------

    //clear request and answer
    memset(request,'\0',10);
    memset(answer,'\0',10);

    // request slope
    strcpy(request, "SLP: REQ\n");

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif

    // display slope
    if (0 == strcmp(answer, "SLP:DOWN\n")) displaySlope(-1);
    if (0 == strcmp(answer, "SLP:FLAT\n")) displaySlope(0);
    if (0 == strcmp(answer, "SLP:  UP\n")) displaySlope(1);

    return 0;
}

//-------------------------------------
//-  Function: controller
//-------------------------------------
void *controller(void *arg)
{
    // Endless loop
    while(1) {//each cycle should be 45 seconds

    	task_mix(); //takes 0.5 s
    	for(int i = 0; i < 11 ; i++){ //this loop takes 4s and is executed 11 times
            // calling task of speed
            task_speed(); //takes 0.5s

            // calling task of slope
            task_slope(); //takes 0.5s

            // turning on or off the gas
            task_gas(); //takes 0.5s

            // turning on or off the brake
            task_brk(); //takes 0.5s
    	}
    	sleep(0.5);
    }
}

//-------------------------------------
//-  Function: Init
//-------------------------------------
rtems_task Init (rtems_task_argument ignored)
{
    pthread_t thread_ctrl;
    sigset_t alarm_sig;
    int i;

    /* Block all real time signals so they can be used for the timers.
     Note: this has to be done in main() before any threads are created
     so they all inherit the same mask. Doing it later is subject to
     race conditions */
    sigemptyset (&alarm_sig);
    for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
        sigaddset (&alarm_sig, i);
    }
    sigprocmask (SIG_BLOCK, &alarm_sig, NULL);

    // init display
    displayInit(SIGRTMAX);

#ifdef RASPBERRYPI
    // Init the i2C driver
    rpi_i2c_init();

    // bus registering, this init the ports needed for the conexion
    // and register the device under /dev/i2c
    rpi_i2c_register_bus("/dev/i2c", 10000);

    // open device file
    fd_i2c = open("/dev/i2c", O_RDWR);

    // register the address of the slave to comunicate with
    ioctl(fd_i2c, I2C_SLAVE, SLAVE_ADDR);
#endif

    /* Create first thread */
    pthread_create(&thread_ctrl, NULL, controller, NULL);
    pthread_join (thread_ctrl, NULL);
    exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_MAXIMUM_SEMAPHORES 10
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 30
#define CONFIGURE_MAXIMUM_DIRVER 10
#define CONFIGURE_MAXIMUM_POSIX_THREADS 2
#define CONFIGURE_MAXIMUM_POSIX_TIMERS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>



