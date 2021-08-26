//  智能車超聲波避障(有舵機)
//  程式中電腦輸出數值部分都註解了，印出會影響智能車遇到障礙物的反應速度
//  調試時可以刪除註解內容Serial.print，輸出測到的距離
//  控制速度的pwm值和延時均有調節，但還是配合實際情況，實際電量調節數值
//=============================================================================
//#include <Servo.h> 
#include <LiquidCrystal.h> //申明1602液晶的函數庫
//申明1602液晶的引腳所連接的Arduino數字端口，8線或4線數據模式，任選其一
//LiquidCrystal lcd(12,11,10,9,8,7,6,5,4,3,2);   //8數據口模式連線聲明
LiquidCrystal lcd(13,12,7,6,5,4,3); //4數據口模式連線聲明 P13--LCD 4腳  P12--LCD 5腳  
              //P7--LCD 6腳   P6--LCD 11腳  P5--LCD 12腳  P4--LCD 13腳  P3--LCD 14腳  

int Echo = A1;  // Echo迴聲腳(P2.0)
int Trig =A0;  //  Trig 觸發腳(P2.1)

int Front_Distance = 0;//
int Left_Distance = 0;
int Right_Distance = 0;

int Left_motor_go=8;     //左電機前進(IN1)
int Left_motor_back=9;     //左電機後退(IN2)

int Right_motor_go=10;    // 右電機前進(IN3)
int Right_motor_back=11;    // 右電機後退(IN4)

int key=A2;//定義按鍵 A2 接口
int beep=A3;//定義蜂鳴器 A3 接口

//const int SensorRight = 3;   	//右循跡紅外傳感器(P3.2 OUT1)
//const int SensorLeft = 4;     	//左循跡紅外傳感器(P3.3 OUT2)

//const int SensorRight_2 = 6;   	//右紅外傳感器(P3.5 OUT4)
//const int SensorLeft_2 = 5;     //左紅外傳感器(P3.4 OUT3)

//int SL;    //左循跡紅外傳感器狀態
//int SR;    //右循跡紅外傳感器狀態
//int SL_2;    //左紅外傳感器狀態
//int SR_2;    //右紅外傳感器狀態

int servopin=2;//設置舵機驅動腳到數字口2
int myangle;//定義角度變量
int pulsewidth;//定義脈寬變量
int val;

void setup()
{
  Serial.begin(9600);     // 初始化串口
  //初始化電機驅動IO為輸出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(key,INPUT);//定義按鍵接口為輸入接口
  pinMode(beep,OUTPUT);
  // pinMode(SensorRight, INPUT); //定義右循跡紅外傳感器為輸入
  // pinMode(SensorLeft, INPUT); //定義左循跡紅外傳感器為輸入
  //pinMode(SensorRight_2, INPUT); //定義右紅外傳感器為輸入
  //pinMode(SensorLeft_2, INPUT); //定義左紅外傳感器為輸入
  //初始化超聲波引腳
  pinMode(Echo, INPUT);    // 定義超聲波輸入腳
  pinMode(Trig, OUTPUT);   // 定義超聲波輸出腳
  lcd.begin(16,2);      //初始化1602液晶工作                       模式
  //定義1602液晶顯示範圍為2行16列字符  
  pinMode(servopin,OUTPUT);//設定舵機接口為輸出接口
}
//=======================智能小車的基本動作=========================
//void run(int time)     // 前進
void run()     // 前進
{
  digitalWrite(Right_motor_go,HIGH);  // 右電機前進
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,165);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);  // 左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);//PWM比例0~255調速，左右輪差異略增減
  analogWrite(Left_motor_back,160);
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

void left(int time)         //左轉(左輪不動，右輪前進)
//void left()         //左轉(左輪不動，右輪前進)
{
  digitalWrite(Right_motor_go,HIGH);	// 右電機前進
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,180); 
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

void right(int time)
//void right()        //右轉(右輪不動，左輪前進)
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
  analogWrite(Right_motor_back,150);//PWM比例0~255調速
  digitalWrite(Left_motor_go,LOW);//左電機前進
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,150);//PWM比例0~255調速
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
      digitalWrite(beep,LOW);          //蜂鳴器停止
  }
}

float Distance_test()   // 量出前方距離 
{
  digitalWrite(Trig, LOW);   // 給觸發腳低電平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 給觸發腳高電平10μs，這里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持續給觸發腳低電
  float Fdistance = pulseIn(Echo, HIGH);  // 讀取高電平時間(單位：微秒)
  Fdistance= Fdistance/58;       //為什麼除以58等於厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  //Serial.print("Distance:");      //輸出距離（單位：厘米）
  //Serial.println(Fdistance);         //顯示距離
  //Distance = Fdistance;
  return Fdistance;
}  

void Distance_display(int Distance)//顯示距離
{
  if((2<Distance)&(Distance<400))
  {
    lcd.home();        //把光標移回左上角，即從頭開始輸出   
    lcd.print("    Distance: ");       //顯示
    lcd.setCursor(6,2);   //把光標定位在第2行，第6列
    lcd.print(Distance);       //顯示距離
    lcd.print("cm");          //顯示
  }
  else
  {
    lcd.home();        //把光標移回左上角，即從頭開始輸出  
    lcd.print("!!! Out of range");       //顯示
  }
  delay(250);
  lcd.clear();
}

void servopulse(int servopin,int myangle)/*定義一個脈衝函數，用來模擬方式產生PWM值舵機的範圍是0.5MS到2.5MS 1.5MS 佔空比是居中周期是20MS*/ 
{
  pulsewidth=(myangle*11)+500;//將角度轉化為500-2480 的脈寬值 這裡的myangle就是0-180度  所以180*11+50=2480  11是為了換成90度的時候基本就是1.5MS
  digitalWrite(servopin,HIGH);//將舵機接口電平置高                                      90*11+50=1490uS  就是1.5ms
  delayMicroseconds(pulsewidth);//延時脈寬值的微秒數  這裡調用的是微秒延時函數
  digitalWrite(servopin,LOW);//將舵機接口電平置低
 // delay(20-pulsewidth/1000);//延時周期內剩餘時間  這裡調用的是ms延時函數
  delay(20-(pulsewidth*0.001));//延時周期內剩餘時間  這裡調用的是ms延時函數
}

void front_detection()
{
  //此處循環次數減少，為了增加小車遇到障礙物的反應速度
  for(int i=0;i<=5;i++) //產生PWM個數，等效延時以保證能轉到響應角度
  {
    servopulse(servopin,90);//模擬產生PWM
  }
  Front_Distance = Distance_test();
  //Serial.print("Front_Distance:");      //輸出距離（單位：厘米）
 // Serial.println(Front_Distance);         //顯示距離
 //Distance_display(Front_Distance);
}

void left_detection()
{
  for(int i=0;i<=15;i++) //產生PWM個數，等效延時以保證能轉到響應角度
  {
    servopulse(servopin,175);//模擬產生PWM
  }
  Left_Distance = Distance_test();
  //Serial.print("Left_Distance:");      //輸出距離（單位：厘米）
  //Serial.println(Left_Distance);         //顯示距離
}

void right_detection()
{
  for(int i=0;i<=15;i++) //產生PWM個數，等效延時以保證能轉到響應角度
  {
    servopulse(servopin,5);//模擬產生PWM
  }
  Right_Distance = Distance_test();
  //Serial.print("Right_Distance:");      //輸出距離（單位：厘米）
  //Serial.println(Right_Distance);         //顯示距離
}
//===========================================================
void loop()
{
  keysacn();	   //調用按鍵掃描函數
  while(1)
  {
    front_detection();//測量前方距離
    if(Front_Distance < 30)//當遇到障礙物時
    {
      back(2);//後退減速
      brake(2);//停下來做測距
      left_detection();//測量左邊距障礙物距離
      Distance_display(Left_Distance);//液晶屏顯示距離
      right_detection();//測量右邊距障礙物距離
      Distance_display(Right_Distance);//液晶屏顯示距離
      if((Left_Distance < 30 ) &&( Right_Distance < 30 ))//當左右兩側均有障礙物靠得比較近
        spin_left(0.7);//旋轉掉頭
      else if(Left_Distance > Right_Distance)//左邊比右邊空曠
      {      
        left(3);//左轉
        brake(1);//剎車，穩定方向
      }
      else//右邊比左邊空曠
      {
        right(3);//右轉
        brake(1);//剎車，穩定方向
      }
    }
    else
    {
      run(); //無障礙物，直行     
    }
  } 
}











