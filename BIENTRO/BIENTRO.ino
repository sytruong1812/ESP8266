#define LED1 D1
#define LED2 D2
#define LED3 D3

void setup() 
{
  Serial.begin(9600);  //Mở cổng Serial để giap tiếp | tham khảo Serial
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}
 
void loop() 
{
  int value = analogRead(A0);   //đọc giá trị điện áp ở chân A0 (value luôn nằm trong khoảng 0-1023)
  if(value < 300)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }
  else if(300 < value < 600)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);  
  }
  else
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);  
  }
  
  Serial.println(value);  //xuất ra giá trị vừa đọc
  
  Serial.println();     //xuống hàng
  delay(200);           //đợi 0.2 giây
  
}
