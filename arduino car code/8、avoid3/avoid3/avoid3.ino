//智能車紅外避障3（帶後退掉頭避障）
//===============================================================
//#include <Servo.h> 
int Left_motor_go=8;     //左電機前進(IN1)
int Left_motor_back=9;     //左電機後退(IN2)

int Right_motor_go=10;    // 右電機前進(IN3)
int Right_motor_back=11;    // 右電機後退(IN4)

int key=7;//定義按鍵 數字7 接口
int beep=12;//定義蜂鳴器 數字12 接口

const int SensorRight = 3;   	//右循跡紅外傳感器(P3.2 OUT1)
const int SensorLeft = 4;     	//左循跡紅外傳感器(P3.3 OUT2)

const int SensorRight_2 = 5;   	//右紅外傳感器(P3.4 OUT3)
const int SensorLeft_2 = 6;     //左紅外傳感器(P3.5 OUT4)

int SL;    //左循跡紅外傳感器狀態
int SR;    //右循跡紅外傳感器狀態
int SL_2;    //左紅外傳感器狀態
int SR_2;    //右紅外傳感器狀態

void setup()
{
  //初始化電機驅動IO為輸出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(key,INPUT);//定義按鍵接口為輸入接口
  pinMode(beep,OUTPUT);
  pinMode(SensorRight, INPUT); //定義右循跡紅外傳感器為輸入
  pinMode(SensorLeft, INPUT); //定義左循跡紅外傳感器為輸入
  pinMode(SensorRight_2, INPUT); //定義右紅外傳感器為輸入
  pinMode(SensorLeft_2, INPUT); //定義左紅外傳感器為輸入
}
//=======================智能小車的基本動作=========================
//void run(int time)     // 前進
void run()     // 前進
{
  digitalWrite(Right_motor_go,HIGH);  // 右電機前進
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,130);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);  // 左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Left_motor_back,130);
  //delay(time * 100);   //執行時間，可以調整  
}

void brake(int time)  //剎車，停車
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  delay(time * 100);//執行時間，可以調整  
}

//void left(int time)         //左轉(左輪不動，右輪前進)
void left()         //左轉(左輪不動，右輪前進)
{
   digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,130); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  //delay(time * 100);	//執行時間，可以調整  
}

void spin_left(int time)         //左轉(左輪後退，右輪前進)
{
   digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,130); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,HIGH);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,130); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  delay(time * 100);	//執行時間，可以調整    
}

//void right(int time)
void right()        //右轉(右輪不動，左輪前進)
{
   digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,130);//PWM比例0~255調速
  //delay(time * 100);	//執行時間，可以調整  
}

void spin_right(int time)        //右轉(右輪後退，左輪前進)
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,130);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,130);//PWM比例0~255調速
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
//==========================================================

void keysacn()//按鍵掃描
{
  int val;
  val=digitalRead(key);//讀取數字7 口電平值賦給val
  while(!digitalRead(key))//當按鍵沒被按下時，一直循環
  {
    val=digitalRead(key);//此句可省略，可讓循環跑空
  }
  while(digitalRead(key))//當按鍵被按下時
  {
    delay(10);	//延時10ms
    val=digitalRead(key);//讀取數字7 口電平值賦給val
    if(val==HIGH)  //第二次判斷按鍵是否被按下
    {
      digitalWrite(beep,HIGH);		//蜂鳴器響
      while(!digitalRead(key))	//判斷按鍵是否被鬆開
        digitalWrite(beep,LOW);		//蜂鳴器停止
    }
    else
      digitalWrite(beep,LOW);//蜂鳴器停止
  }
}

void loop()
{
  keysacn();	   //調用按鍵掃描函數
  while(1)
  {
    //有信號為LOW  沒有信號為HIGH
    SR_2 = digitalRead(SensorRight_2);
    SL_2 = digitalRead(SensorLeft_2);
    if (SL_2 == HIGH&&SR_2==HIGH)
      run();   //調用前進函數
    else if (SL_2 == HIGH & SR_2 == LOW)// 右邊探測到有障礙物，有信號返回，向左轉 
        left();
    else if (SR_2 == HIGH & SL_2 == LOW) //左邊探測到有障礙物，有信號返回，向右轉  
      right();
    else // 都是有障礙物, 後退
    {
      back(4.5);//後退
      spin_right(4.5);//有旋轉，調整方向
    }
  }
}