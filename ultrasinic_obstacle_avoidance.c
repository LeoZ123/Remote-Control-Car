#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <wiringPi.h>

//define input io
#define Trig	28
#define Echo	29
#define BUFSIZE 512

//define sensors signal
void ultraInit(void)
{
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

//measure distancetance
float distanceMeasure(void)
{
  struct timeval tv1;
  struct timeval tv2;
  long start, stop;
  float distance;

  digitalWrite(Trig, LOW);
  delayMicroseconds(2);

  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);	  //send ultrasonic pulse 
  digitalWrite(Trig, LOW);
  
  while(!(digitalRead(Echo) == 1));
  gettimeofday(&tv1, NULL);		    //get current time for send signal

  while(!(digitalRead(Echo) == 0));
  gettimeofday(&tv2, NULL);		   //get current time for back signal

  start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //convert to ms
  stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;

  distance = (float)(stop - start) / 1000000 * 34000 / 2;  //calculate distancetance

  return distance;
}

// go instruction
void run()   
{
  softPwmWrite(4,0); //left wheel go forward
	softPwmWrite(1,250); 
	softPwmWrite(6,0); //right wheel go forward
	softPwmWrite(5,250);  
}

// stop instruction
void brake(int time)        
{
  softPwmWrite(1,0); //left wheel stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //right wheel stop
	softPwmWrite(6,0); 
  delay(time * 100); //set delay time to execute next instruciton
}

//left turn instruction
void left(int time)      
{
  softPwmWrite(1,0); //left wheel turn left
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //right wheel turn left
	softPwmWrite(6,250); 
	delay(time * 300);
}


//right turn instruction
void right(int time)     
{
  softPwmWrite(1,0); //left wheel turn right
	softPwmWrite(4,250); 
	softPwmWrite(5,0); //right wheel turn rignt
	softPwmWrite(6,0); 
  delay(time * 300);	//set delay time to execute next instruciton
}


//back turn instruction
void back(int time)        
{
  softPwmWrite(4,250); //left wheel back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //right wheel back
  softPwmWrite(5,0); 
  delay(time *200);    //set delay time to execute next instruciton
}


int main(int argc, char *argv[])
{
    float distance;

    // char buf[BUFSIZE]={0xff,0x00,0x00,0x00,0xff};

    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);

  while(1){
    distance = distanceMeasure();
    printf("distancetance = %0.2f cm\n",distance);//print current distancetance between object and the car
     if(distance<30){   //if the distancetance less than 30cm
	     back(4);//back 400ms
	     left(4);//trun left 400ms
     }
     else {
        run();  //else keep going
    }
  }
  return 0;

}

