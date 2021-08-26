//智能車按鍵啟動和蜂鳴器報警
//===============================================================
//#include <Servo.h> 
int Left_motor_go=8;     //左電機前進(IN1)
int Left_motor_back=9;     //左電機後退(IN2)

int Right_motor_go=10;    // 右電機前進(IN3)
int Right_motor_back=11;    // 右電機後退(IN4)

int key=7;//定義按鍵 數字7 接口
int beep=12;//定義蜂鳴器 數字12 接口

void setup()
{
  //初始化電機驅動IO為輸出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(key,INPUT);//定義按鍵接口為輸入接口
  pinMode(beep,OUTPUT);
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

void brake(int time)         //剎車，停車
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  delay(time * 100);//執行時間，可以調整  
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

void keysacn()
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
  back(10); //後退1s
  brake(5); //停止0.5s
  run(10);//前進1s
  brake(5); //停止0.5s
  left(10);//向左轉1s
  right(10);//向右轉1s
  spin_left(20);//向左旋轉2s
  spin_right(20);//向右旋轉2s
  brake(5);	 //停車
}