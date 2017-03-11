#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>

//define input io
#define Trig	28
#define Echo	29
#define LEFT	27
#define RIGHT	26
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
void brake()    
{
  softPwmWrite(1,0); //left wheel stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //rigjt wheel stop
	softPwmWrite(6,0); 
  
}

//left turn instruction
void left()         
{
  softPwmWrite(4,250); //left wheel turn left
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //right wheel turn left
	softPwmWrite(5,250); 
}


//right turn instruction
void right()     
{
  softPwmWrite(4,0); //left wheel turn right
	softPwmWrite(1,250); 
	softPwmWrite(6,250); //right wheel turn right
	softPwmWrite(5,0); 

}

//back turn instruction
void back()        
{
  softPwmWrite(1,250); //left wheel back
	softPwmWrite(4,0); 
	softPwmWrite(5,250); //right wheel back
  softPwmWrite(6,0); 
 
}

int main(int argc, char *argv[])
{
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
    // pinMode (27, OUTPUT);	// Steering engine outputr
    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
  //softPwmCreate(27,1,50);	
	//softPwmWrite(27,1);

  while(1)
  {
    //having signal to LOW, no signal to HIGH
    //having signal -> car in the white area -> light L light
    //no light -> car on the black line -> light L off
    SR = digitalRead(RIGHT);
    SL = digitalRead(LEFT);
    if (SL == LOW&&SR==LOW){
      printf("GO");
      run();
    }
    else if (SL == HIGH&&SR == LOW){
	   printf("RIGHT");
	   left();  
    }
    else if (SR == HIGH&&SL == LOW) {
      printf("LEFT");  
      right ();
    }
    else {
      printf("STOP");
      brake();
    }
  }

  return 0;

}

