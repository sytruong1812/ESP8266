/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLDWMWRc5c"
#define BLYNK_DEVICE_NAME "Home"
#define BLYNK_AUTH_TOKEN "y6mLNYt_rIYaVCy39zOBPzX8uXzQBK94"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "y6mLNYt_rIYaVCy39zOBPzX8uXzQBK94";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "2.4G_Phuongngoc lầu 2";
char pass[] = "10111960";


#define LED1 D1
#define LED2 D2
#define LED3 D3


#define Button1 D5
#define Button2 D6
#define Button3 D7

//......Biến lưu trạng thái button.....
// Mục đích để lấy một trạng thái (LOW or HIGH) từ button khi nhấn nút
// Nếu không có thì lúc nhấn nút led sẽ nháy sáng rồi tắt ngay
// Kiểu dữ liệu boolean là kiểu luận lý
// Kiểu bool chỉ nhận 2 giá trị true(1 or -1 or 1001 là các giá trị khác 0) hoặc false(0)
boolean bt1_state = HIGH;
boolean bt2_state = HIGH;
boolean bt3_state = HIGH;

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);

  
  Blynk.begin(auth, ssid, pass);
  //You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

BLYNK_CONNECTED() 
{
  Blynk.syncVirtual(V1,V2,V3);
}

//...................Điều khiển Button trên WEB và APP Blynk..........................
BLYNK_WRITE(V1)
{
  int Value1 = param.asInt();    //Lấy giá trị V1 là Button trên Web về
  digitalWrite(LED1, Value1);
  Blynk.virtualWrite(V0, Value1);   //Gửi giá trị điều khiển LED 1 hiển thị trên web sáng
}

BLYNK_WRITE(V2)
{
  int Value2 = param.asInt();
  digitalWrite(LED2, Value2);
}

BLYNK_WRITE(V3)
{
  int Value3 = param.asInt();
  digitalWrite(LED3, Value3);

}

//...............Điều khiển Button thủ công.............................
void check_button()
{
  if(digitalRead(Button1) == LOW)  //LOW: Nhấn cái ăn ngay  //HIGH: Nhấn thả mới ăn
  {
    if(bt1_state == HIGH)
    {
      int value = digitalRead(LED1);
      Serial.println(value);
      //Ban đầu:  LED1=0 
      digitalWrite(LED1, !digitalRead(LED1));         //Điều khiển LED sáng LED1 = 1
      //LED1 new cập nhật:  LED1=1
      Blynk.virtualWrite(V1, digitalRead(LED1));    //Gửi giá trị điều khiển Button1 trên web ON/OFF theo trạng thái LED1 new
      Blynk.virtualWrite(V0, digitalRead(LED1));   //Gửi giá trị điều khiển LED 1 hiển thị trên web sáng tắt theo LED1 new
      bt1_state = LOW;
      delay(200);
    }
  }
  else
  {
    bt1_state = HIGH; 
  }
  if(digitalRead(Button2) == LOW)
  {
    if(bt2_state == HIGH)
    {
      digitalWrite(LED2, !digitalRead(LED2));
      Blynk.virtualWrite(V2, digitalRead(LED2));
      bt2_state = LOW;
      delay(200);
    }
  }
  else
  {
    bt2_state = HIGH;  
  }
  if(digitalRead(Button3) == LOW)
  {
    if(bt3_state == HIGH)
    {
      digitalWrite(LED3, !digitalRead(LED3));
      Blynk.virtualWrite(V3, digitalRead(LED3));
      bt3_state = LOW;
      delay(200);
    }
  }
  else
  {
    bt3_state = HIGH;  
  }   
}
void loop()
{
  Blynk.run();
  check_button();
}
