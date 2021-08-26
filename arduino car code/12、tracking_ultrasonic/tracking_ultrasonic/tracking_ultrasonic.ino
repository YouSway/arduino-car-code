//  智能車超聲波避障(有舵機)
//  程式中電腦輸出數值部分都註解了，印出會影響智能車遇到障礙物的反應速度
//  調試時可以刪除註解內容Serial.print，輸出測到的距離
//  控制速度的pwm值和延時均有調節，但還是配合實際情況，實際電量調節數值
//=============================================================================
//#include <Servo.h> 
int Left_motor_go=8;     //左電機前進(IN1)
int Left_motor_back=9;     //左電機後退(IN2)
int Right_motor_go=10;    // 右電機前進(IN3)
int Right_motor_back=11;    // 右電機後退(IN4)

int key=7;//定義按鍵 數字7 接口
int beep=12;//定義蜂鳴器 數字12 接口

const int SensorRight = 3;   	//右循跡紅外傳感器(P3.2 OUT1)
const int SensorLeft = 4;     	//左循跡紅外傳感器(P3.3 OUT2)

int SL;    //左循跡紅外傳感器狀態
int SR;    //右循跡紅外傳感器狀態

int Echo = A1;  // Echo迴聲腳(P2.0)
int Trig =A0;  //  Trig 觸發腳(P2.1)

int Distance = 0;

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
  Serial.begin(9600);     // 初始化串口
    pinMode(Echo, INPUT);    // 定義超聲波輸入腳
  pinMode(Trig, OUTPUT);   // 定義超聲波輸出腳
}

//=======================智能小車的基本動作=========================
//void run(int time)     // 前進
void run()
{
  digitalWrite(Right_motor_go,HIGH);  // 右電機前進
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,160);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);  // 左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Left_motor_back,160);
  //delay(time * 100);   //執行時間，可以調整  
}

//void brake(int time)  //剎車，停車
void brake()
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  //delay(time * 100);//執行時間，可以調整  
}

//void left(int time)         //左轉(左輪不動，右輪前進)
void left()
{
  digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,140); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
  //delay(time * 100);	//執行時間，可以調整  
}

void spin_left()         //左轉(左輪後退，右輪前進)
{
  digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,160); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,HIGH);   //左輪後退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,100); 
  analogWrite(Left_motor_back,0);//PWM比例0~255調速
 // delay(time * 100);	//執行時間，可以調整    
}

//void right(int time)        //右轉(右輪不動，左輪前進)
void right()
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,140);//PWM比例0~255調速
  //delay(time * 100);	//執行時間，可以調整  
}

void spin_right()        //右轉(右輪後退，左輪前進)
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,100);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,160);//PWM比例0~255調速
 //delay(time * 100);	//執行時間，可以調整    
}

//void back(int time)          //後退
void back(int time)
{
  digitalWrite(Right_motor_go,LOW);   //右電機後退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,200);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,200);//PWM比例0~255調速
  //delay(time * 100);	//執行時間，可以調整     
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
      digitalWrite(beep,LOW);          //蜂鳴器停止
  }
}

void Distance_test()   // 量出前方距離 
{
  digitalWrite(Trig, LOW);   // 給觸發腳低電平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 給觸發腳高電平10μs，這里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持續給觸發腳低電
  float Fdistance = pulseIn(Echo, HIGH);  // 讀取高電平時間(單位：微秒)
  Fdistance= Fdistance/58;       //為什麼除以58等於厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  Serial.print("Distance:");      //輸出距離（單位：厘米）
  Serial.println(Fdistance);         //顯示距離
  Distance = Fdistance;
}

void loop()
{ 
  keysacn();//調用按鍵掃描函數  
  while(1)
  {
    Distance_test();//測試前方距離
  //有信號為LOW  沒有信號為HIGH
  SR = digitalRead(SensorRight);//有信號表明在白色區域，車子底板上L3亮；沒信號表明壓在黑線上，車子底板上L3滅
  SL = digitalRead(SensorLeft);//有信號表明在白色區域，車子底板上L2亮；沒信號表明壓在黑線上，車子底板上L2滅
  if((Distance < 10)||(Distance > 400))//如果距離小於10cm或大於400cm（小於2cm數值也大於400）距離都可自行調節，另跑道比較空曠，可去掉Distance > 400這一條件
  brake();//停車
  else
  {
  if (SL == LOW&&SR==LOW)
    run();   //調用前進函數
  else if (SL == HIGH & SR == LOW)// 左循跡紅外傳感器,檢測到信號，車子向右偏離軌道，向左轉 
    spin_left();
  else if (SR == HIGH & SL == LOW) // 右循跡紅外傳感器,檢測到信號，車子向左偏離軌道，向右轉  
    spin_right();
  else // 都是黑色, 停止
  brake();
  }
  }
}








