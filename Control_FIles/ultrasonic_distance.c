#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

//define io
#define Trig	28
#define Echo	29

//define sensors signal
void ultraInit(void)
{
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

//measure distance
float disMeasure(void)
{
  struct timeval tv1;
  struct timeval tv2;
  long start, stop;
  float dis;

  digitalWrite(Trig, LOW);
  delayMicroseconds(2);

  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);	  //send ultrasonic pulse 
  digitalWrite(Trig, LOW);
  
  while(!(digitalRead(Echo) == 1));
  gettimeofday(&tv1, NULL);		   //get current time for send signal

  while(!(digitalRead(Echo) == 0));
  gettimeofday(&tv2, NULL);		   //get current time for back signal

  start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //convert to ms
  stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;

  dis = (float)(stop - start) / 1000000 * 34000 / 2;  //calculate distance

  return dis;
}

int main(void)
{
  float dis;

  if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
    printf("setup wiringPi failed !");
    return 1; 
  }

  ultraInit();
  
  while(1){
    dis = disMeasure();
    printf("distance = %0.2f cm\n",dis);
    delay(1000);
  }

  return 0;
}