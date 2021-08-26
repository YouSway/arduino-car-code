//智能小車超聲波測距實驗
//===============================================================
#include <LiquidCrystal.h> //申明1602液晶的函數庫
//申明1602液晶的引腳所連接的Arduino數字端口，8線或4線數據模式，任選其一
//LiquidCrystal lcd(12,11,10,9,8,7,6,5,4,3,2);   //8數據口模式連線聲明
LiquidCrystal lcd(13,12,7,6,5,4,3); //4數據口模式連線聲明 P13--LCD 4腳  P12--LCD 5腳  
              //P7--LCD 6腳   P6--LCD 11腳  P5--LCD 12腳  P4--LCD 13腳  P3--LCD 14腳  

int Echo = A1;  // Echo迴聲腳(P2.0)
int Trig =A0;  //  Trig 觸發腳(P2.1)

int Distance = 0;

void setup()
{
  Serial.begin(9600);     // 初始化串口
  //初始化電機驅動IO為輸出方式
  //初始化超聲波引腳
  pinMode(Echo, INPUT);    // 定義超聲波輸入腳
  pinMode(Trig, OUTPUT);   // 定義超聲波輸出腳
  lcd.begin(16,2);      //初始化1602液晶工作模式
  //定義1602液晶顯示範圍為2行16列字符  
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
  Distance_test();
  if((2<Distance)&(Distance<400))//超聲波測距範圍2cm到400cm
  {
    lcd.home();        //把光標移回左上角，即從頭開始輸出   
    lcd.print("    Distance: ");       //顯示
    lcd.setCursor(6,2);   //把光標定位在第2行，第6列
    lcd.print(Distance);       //顯示距離
    lcd.print("cm");        //顯示
  }
  else
  {
    lcd.home();        //把光標移回左上角，即從頭開始輸出  
    lcd.print("!!! Out of range");       //顯示超出距離
  }
  delay(250);
  lcd.clear();
}


