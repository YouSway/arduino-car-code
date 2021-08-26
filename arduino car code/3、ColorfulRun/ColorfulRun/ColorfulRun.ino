//  智能車指定花式動作
//===============================================================
//#include <Servo.h> 
int Left_motor_go=8;     //左電機前進(IN1)
int Left_motor_back=9;     //左電機後退(IN2)

int Right_motor_go=10;    // 右電機前進(IN3)
int Right_motor_back=11;    // 右電機後退(IN4)

void setup()
{
  //初始化電機驅動IO為輸出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
}

void brake(int time)         //剎車，停車
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  delay(time * 100);//執行時間，可以調整  
}
void run(int time)     // 前進
{
  digitalWrite(Right_motor_go,HIGH);  // 右電機前進
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,200);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);  // 左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Left_motor_back,200);
  delay(time * 100);   //執行時間，可以調整  
}
void left(int time)         //左轉(左輪不動，右輪前進)
{
  digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,200); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  delay(time * 100);	//執行時間，可以調整  
}
void spin_left(int time)         //左轉(左輪後退，右輪前進)
{
  digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,200); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,HIGH);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,200); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  delay(time * 100);	//執行時間，可以調整  
}

void right(int time)        //右轉(右輪不動，左輪前進)
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,200);//PWM比例0~255調速
  delay(time * 100);	//執行時間，可以調整  
}

void spin_right(int time)        //右轉(右輪後退，左輪前進)
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,200);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,200);//PWM比例0~255調速
  delay(time * 100);	//執行時間，可以調整    
}

void back(int time)          //後退
{
  digitalWrite(Right_motor_go,LOW);  //右輪後退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,150);//PWM比例0~255調速
  digitalWrite(Left_motor_go,HIGH);  //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,150);
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  delay(time * 100);     //執行時間，可以調整  
}

void loop()
{    
  int i;
  delay(2000); //延時2s後啟動
  run(10);
  back(10);//全速前進急停後退
  brake(5);

  for(i=0;i<5;i++)
  {
    run(10);//小車間斷性前進5步
    brake(1);
  }

  for(i=0;i<5;i++)
  {
    back(10);//小車間斷性後退5步
    brake(1);
  }

  for(i=0;i<5;i++)
  {
    left(10);//大彎套小彎連續左旋轉
    spin_left(5);
  }

  for(i=0;i<5;i++)
  {
    right(10);//大彎套小彎連續右旋轉
    spin_right(5);
  }

  for(i=0;i<10;i++)
  {
    right(1);//間斷性原地右轉彎
    brake(1);
  }

  for(i=0;i<10;i++)
  {
    left(1);//間斷性原地左轉彎
    brake(1);
  }

  for(i=0;i<10;i++)
  {
    left(3);//走S形前進
    right(3);
  }

  for(i=0;i<10;i++)
  {
    spin_left(3);//間斷性原地左打轉
    brake(3);
  }

  for(i=0;i<10;i++)
  {
    spin_right(3);//間斷性原地右打轉
    brake(3);
  }

}