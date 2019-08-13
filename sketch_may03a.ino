#include <IRremote.h>
#define UP 16736925kiuytljo
#define DOWN 16754775
#define LEFT 16720605
#define RIGHT 16761405
#define S_TIME 2000
#define T_TIME 200

String recv="";
const int MA1=5; //Motor-A
const int MA2=6;
const int MB1=9; //Motor-B
const int MB2=10;
const int outputPin=11; // 定义超声波信号发出接口
const int inputPin=12; // 定义超声波信号接收接口
const int recvPin=7;  //红外接收接口

IRrecv irrecv(recvPin);
decode_results results;
//控制电机的函数
void forward()
{
  analogWrite(MA1,128);
  analogWrite(MA2,0);
  analogWrite(MB1,128);
  analogWrite(MB2,0);
}

void backward()
{
  analogWrite(MA1,0);
  analogWrite(MA2,128);
  analogWrite(MB1,0);
  analogWrite(MB2,128);
}

void turnRight()
{
  analogWrite(MA1,128);
  analogWrite(MA2,0);
  digitalWrite(MB1,HIGH);
  digitalWrite(MB2,HIGH);
}

void turnLeft()
{
  analogWrite(MB1,128);
  analogWrite(MB2,0);
  digitalWrite(MA1,HIGH);
  digitalWrite(MA2,HIGH);
}

void brake()
{
  digitalWrite(MA1,HIGH);
  digitalWrite(MA2,HIGH);
  digitalWrite(MB1,HIGH);
  digitalWrite(MB2,HIGH);
}

void stopo()
{
  digitalWrite(MA1,LOW);
  digitalWrite(MA2,LOW);
  digitalWrite(MB1,LOW);
  digitalWrite(MB2,LOW);
}

int ranging()
{
  digitalWrite(outputPin,LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin,HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin,LOW); // 保持发出超声波信号接口低电平
  int distance=pulseIn(inputPin,HIGH); // 读出脉冲时间
  distance=distance/58; // 将脉冲时间转化为距离（单位：厘米）
  return distance;
}

void readcmd()
{
  if(Serial.available()>0)
  {
    while(Serial.available()>0)
    {
      recv+=char(Serial.read());
    }
  }
}

void act()
{
  int times=recv[1]-'0';
  int pieces=50*times;
  int piece=S_TIME*times/pieces;
  if(recv[0]=='F')
  {
    for(int i=1;i<=pieces;++i)
    {
      if(ranging()<30)
        turnRight();
      else
        forward();
      delay(piece);
    }
  }

  else if(recv[0]=='B')
  {
    for(int i=1;i<=pieces;++i)
    {
      if(ranging()<30)
        turnRight();
      else
        backward();
      delay(piece);
    }
  }

  else if(recv[0]=='L')
  {
    turnLeft();
    delay(times*T_TIME);
  }

  else if(recv[0]=='R')
  {
    turnRight();
    delay(times*T_TIME);
  }
  brake();
  stopo();
  delay(10);
  recv="";
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); //初始化红外遥控
  pinMode(13,OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(MA1,OUTPUT);
  pinMode(MA2,OUTPUT);
  pinMode(MB1,OUTPUT);
  pinMode(MB2,OUTPUT);
  Serial.flush();
}

void loop()
{
  readcmd();
  act();
  Serial.flush();
}
