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


#include "displayC.h"

#define BILLION  1E9
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

/*PART 1*/
int mix = 0;
int gas = 0;
int brk = 0;

/*PART 2*/
int ligth = 0; //% ligth
int is_dark = 0; // 0 --> no dark,  1--> dark

/*PART 3*/
int mode= 0; //0->NORMAL, 1-->BRAKING MODE, 2 --> STOPPED MODE
int distance = -1000;
int distance_limit = 11000; //distance limit
#define NORMAL_MODE 0
#define BRAKE_MODE 1
#define STOPPED_MODE 2

/*PART 3*/
#define EMERGENCY_MODE 3
int emergency_sent = 0;

/**********************************************************
 *  Function: task_ligth
 *********************************************************/
int task_ligth()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
    char request[10];
    char answer[10];

    //--------------------------------
    //  request slope and display it
    //--------------------------------

    //clear request and answer
    memset(request,'\0',10);
    memset(answer,'\0',10);

    // request slope
    strcpy(request, "LIT: REQ\n");

#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif
    if (1 == sscanf (answer, "LIT: %i\n", &ligth)){
    	if(ligth <= 50){
    		is_dark=1;
    	}else{
    		is_dark=0;
    	}
    	displayLightSensor(is_dark);
    }
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
    return 0;
}

/**********************************************************
 *  Function: task_lamp
 *********************************************************/
int task_lamp()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
    char request[10];
    char answer[10];

    //--------------------------------
    //  request slope and display it
    //--------------------------------

    //clear request and answer
    memset(request,'\0',10);
    memset(answer,'\0',10);


    if (is_dark == 1){
    	strcpy(request, "LAM: SET\n");
    }else{
    	strcpy(request, "LAM: CLR\n");
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
    displayLamps(is_dark);
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
    return 0;
}

/**********************************************************
 *  Function: task_mix
 *********************************************************/
int task_mix()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
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
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}

/**********************************************************
 *  Function: task_gas
 *********************************************************/
int task_gas()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
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
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}

/**********************************************************
 *  Function: task_brk
 *********************************************************/
int task_brk()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
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
		brk=1;
	}else{
		strcpy(request, "BRK: CLR\n");
		brk=0;
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
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}


/**********************************************************
 *  Function: task_speed
 *********************************************************/
int task_speed()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
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

	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}

//-------------------------------------
//-  Function: task_slope
//-------------------------------------
int task_slope()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
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

	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}


//read the distance and changes the mode accordinly
int task_check_distance()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------

	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	// request distance
	strcpy(request, "DS:  REQ\n");

#ifdef RASPBERRYPI
	// use Raspberry Pi I2C serial module
	write(fd_i2c, request, MSG_LEN);
	nanosleep(&time_msg, NULL);
	read(fd_i2c, answer, MSG_LEN);
#else
	//Use the simulator
	simulator(request, answer);
#endif

	// display distance
	if (1 == sscanf (answer, "DS:%i\n", &distance)){
		if(distance == 0 && mode != EMERGENCY_MODE){
			mode = STOPPED_MODE;
		}
		else if(distance < distance_limit && mode != EMERGENCY_MODE){
			mode = BRAKE_MODE;
		}
		displayDistance(distance);
	}
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;


}

//read if moving and change state to stop.
int task_check_moving(){
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------

	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	// request distance
	strcpy(request, "STP: REQ\n");

#ifdef RASPBERRYPI
	// use Raspberry Pi I2C serial module
	write(fd_i2c, request, MSG_LEN);
	nanosleep(&time_msg, NULL);
	read(fd_i2c, answer, MSG_LEN);
#else
	//Use the simulator
	simulator(request, answer);
#endif

	// display distance
	if (0 == strcmp(answer, "STP:  GO\n") && mode != EMERGENCY_MODE){
		mode = NORMAL_MODE;
		displayStop(0);
	}
	if (0 == strcmp(answer, "STP:STOP\n")){
		displayStop(1);
	}
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;

}

int task_on_lamps(){
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
    char request[10];
    char answer[10];

    //--------------------------------
    //  request slope and display it
    //--------------------------------

    //clear request and answer
    memset(request,'\0',10);
    memset(answer,'\0',10);


    strcpy(request, "LAM: SET\n");
#ifdef RASPBERRYPI
    // use Raspberry Pi I2C serial module
    write(fd_i2c, request, MSG_LEN);
    nanosleep(&time_msg, NULL);
    read(fd_i2c, answer, MSG_LEN);
#else
    //Use the simulator
    simulator(request, answer);
#endif
    displayLamps(is_dark);
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
    return 0;
}

//break mode
int task_brk_mode_brake(){
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------
	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	//avoid go less than 5 m/s
	if (8 < speed){
		brk=1;
		strcpy(request, "BRK: SET\n");
	}else{
		brk=0;
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
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}

int task_gas_mode_brake()
{
	long elapsed_time = 0;
	struct timespec taskstart, taskend;
	clock_gettime(CLOCK_REALTIME, &taskstart);
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------

	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	// request gas
	if (speed < 5){
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
	clock_gettime(CLOCK_REALTIME, &taskend);
	task_time = ( taskend.tv_sec - taskstart.tv_sec ) + ( taskend.tv_nsec - taskstart.tv_nsec )/ BILLION;
	if (0,9 < task_time){
		mode = EMERGENCY_MODE;
	}
	return 0;
}

//emergency mode
void task_send_emergency(){
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------

	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	// request gas
	strcpy(request, "ERR: SET\n");

#ifdef RASPBERRYPI
	// use Raspberry Pi I2C serial module
	write(fd_i2c, request, MSG_LEN);
	nanosleep(&time_msg, NULL);
	read(fd_i2c, answer, MSG_LEN);
#else
	//Use the simulator
	simulator(request, answer);
#endif
	return 0;
}

int task_brk_mode_emergency(){
	char request[10];
	char answer[10];

	//--------------------------------
	//  request speed and display it
	//--------------------------------
	//clear request and answer
	memset(request, '\0', 10);
	memset(answer, '\0', 10);

	//avoid go less than 5 m/s
	brk=1;
	strcpy(request, "BRK: SET\n");


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

int task_gas_mode_emergency()
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
	strcpy(request, "GAS: CLR\n");
	gas = 0;

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


void normal_mode(){
	int secondary_cycle = 0;
	long elapsed_time = 0;
	struct timespec start, end;
	// Endless loop. Main cycle 45 seconds, secondary cycle 9 seconds
	while(mode == NORMAL_MODE){
		clock_gettime(CLOCK_REALTIME, &start);
		switch(secondary_cycle){
			case 0:
				task_mix();
				task_slope();
				task_speed();
				task_check_distance();
				task_gas();
				task_brk();
				task_ligth();
				task_lamp();
				break;
			case 1:
				task_slope();
				task_speed();
				task_check_distance();
				task_gas();
				task_brk();
				task_ligth();
				task_lamp();
				break;
		}
		secondary_cycle = (secondary_cycle + 1) % 2;
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed_time = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec )/ BILLION;
		sleep(9 - elapsed_time);
	}
}

void brake_mode(){
    int secondary_cycle = 0;
	long elapsed_time = 0;
	struct timespec start, end;
	// Endless loop. Main cycle 45 seconds, secondary cycle 9 seconds
	while(mode == BRAKE_MODE){
		clock_gettime(CLOCK_REALTIME, &start);
		switch(secondary_cycle){
			case 0:
				task_mix();
				task_slope();
				task_speed();
				task_check_distance();
				task_gas_mode_brake();
				task_brk_mode_brake();
				task_on_lamps();
				break;
			case 1:
				task_slope();
				task_speed();
				task_check_distance();
				task_gas_mode_brake();
				task_brk_mode_brake();
				task_on_lamps();
				break;
		}
		secondary_cycle = (secondary_cycle + 1) % 2;
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed_time = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec )/ BILLION;
		sleep(7 - elapsed_time);
	}
}


void stopped_mode(){
    int secondary_cycle = 0;
	long elapsed_time = 0;
	struct timespec start, end;
	// Endless loop. Main cycle 45 seconds, secondary cycle 9 seconds
	while(mode == STOPPED_MODE){
		clock_gettime(CLOCK_REALTIME, &start);
		switch(secondary_cycle){
			case 0:
				task_mix();
				task_check_moving();
				task_on_lamps();
				break;
			case 1:
				task_check_moving();
				task_on_lamps();
				break;
			case 2:
				task_check_moving();
				task_on_lamps();
				break;
		}
		secondary_cycle = (secondary_cycle + 1) % 3;
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed_time = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec )/ BILLION;
		sleep(5 - elapsed_time);
	}

}

void emergency_mode(){
    int secondary_cycle = 0;
	long elapsed_time = 0;
	struct timespec start, end;
	// Endless loop. Main cycle 45 seconds, secondary cycle 9 seconds
	while(1){
		clock_gettime(CLOCK_REALTIME, &start);
		switch(secondary_cycle){
			case 0:
				if (emergency_sent == 0){
					task_send_emergency();
				}
				task_mix();
				task_slope();
				task_speed();
				task_check_distance();
				task_gas_mode_emergency();
				task_brk_mode_emergency();
				task_on_lamps();
				break;
			case 1:
				task_slope();
				task_speed();
				task_check_distance();
				task_gas_mode_brake();
				task_brk_mode_brake();
				task_on_lamps();
				break;
		}
		secondary_cycle = (secondary_cycle + 1) % 2;
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed_time = ( end.tv_sec - start.tv_sec ) + ( end.tv_nsec - start.tv_nsec )/ BILLION;
		sleep(7 - elapsed_time);
	}
}

//-------------------------------------
//-  Function: controller
//------------------------------------
void *controller(void *arg)
{

    while(1) {
    	switch(mode){
    		case NORMAL_MODE:
    			normal_mode();
    			break;
    		case BRAKE_MODE:
    			brake_mode();
    			break;
    		case STOPPED_MODE:
    			stopped_mode();
    			break;
			case EMERGENCY_MODE:
    			emergency_mode();
    			break;
    	}

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
