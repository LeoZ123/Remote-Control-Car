#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>

//define io
#define Trig	28
#define Echo	29
#define LEFT	11
#define RIGHT	10
#define BUFSIZE 512

//define motor motion instruction
#define MOTOR_GO_FORWARD   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_BACK	   digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT	   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_LEFT	   digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_STOP	   digitalWrite(1, LOW);digitalWrite(4,LOW);digitalWrite(5, LOW);digitalWrite(6,LOW)

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
void left()      
{
  softPwmWrite(4,250); //left wheel turn left
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //right wheel turn left
	softPwmWrite(5,250); 
	//delay(time * 300); //set delay time to execute next instruciton
}


//right turn instruction
void right()       
{
  softPwmWrite(4,0); //left wheel turn right
	softPwmWrite(1,250); 
	softPwmWrite(6,250); //right wheel turn right
	softPwmWrite(5,0); 
  //delay(time * 300);	//set delay time to execute next instruciton
}


//back turn instruction
void back()        
{
  softPwmWrite(4,250); //left wheel back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //left wheel back
  softPwmWrite(5,0); 
  //delay(time *200);  //set delay time to execute next instruciton
}


int main(int argc, char *argv[])
{

  float dis;

  // char buf[BUFSIZE]={0xff,0x00,0x00,0x00,0xff};

	int SR;
	int SL;
    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    // pinMode (27, OUTPUT);	//
    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
    // softPwmCreate(27,1,50);	
    //softPwmWrite(27,1);

  while(1)
  {
    //having signal: set to LOW, no signal: set to HIGH
    SR = digitalRead(RIGHT);
    SL = digitalRead(LEFT);
    if (SL == LOW && SR==LOW){
      printf("BACK");  //if both sensors find object
      back(); //execute BACK instruction 
      delay(300); //delay 300mss
      left(); //execute
      delay(601);   
    }
    else if (SL == HIGH && SR == LOW){
	    printf("RIGHT"); //if right sensor find object
	    left();	  //turn left
    }
    else if (SR == HIGH&&SL == LOW) {
      printf("LEFT");  //if left sensor find object
      right (); //turn right
    }
    else {
      printf("GO"); //else: no sensor find object
      run(); //keep going 
    }
  }

  return 0;

}

