/*
  Wiring:
  ---------------------
  | VS1053  | ESP8266 |
  ---------------------
  |   SCK   |   D5    |
  |   MISO  |   D6    |
  |   MOSI  |   D7    | 
  |   XRST  |   RST   |
  |   CS    |   D1    |
  |   DCS   |   D0    |
  |   DREQ  |   D3    |
  |   5V    |   5V    |
  |   GND   |   GND   |
  ---------------------
*/

#include <VS1053.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#define VS1053_CS     D1
#define VS1053_DCS    D0
#define VS1053_DREQ   D3
#endif

// Default volume
#define VOLUME  80

// Button chuyển bài
#define Button1 D5
#define Button2 D6

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
WiFiClient client;

// WiFi settings
//const char *ssid = "SmartControl.Vn.2.4G";
//const char *password = "1900886806";

const char *ssid = "Redmi7";
const char *password = "hldck2001";

//char host[5]={"airspectrum.cdnstream1.com" , "airspectrum.cdnstream1.com" , "162.244.80.118" , "wowccm.iptime.org" , "wowccm.iptime.org"};
//char path[5]={"/1648_128" , "/1261_192" , "/stream/1/" , "/stream/1/" , "/stream/1/"};
//int httpPort[5]={8144, 8000, 3092, 8000, 8000};


//  http://host:httpPort/path
//const char *host = "airspectrum.cdnstream1.com";
//const char *path = "/1648_128";
//int httpPort = 8114;
//............................................................
//const char *host = "airspectrum.cdnstream1.com";
//const char *path = "/1261_192";
//int httpPort = 8000;
//.......................VIỆT NAM.................................
const char *host = "162.244.80.118";
const char *path = "/stream/1/";
int httpPort = 3092;
//........................KOREA...................................
//const char *host = "wowccm.iptime.org";
//const char *path = "/stream/1/";
//int httpPort = 8000;
//.........................................................
//const char *host = "wowccm.iptime.org";
//const char *path = "/stream/1/";
//int httpPort = 8000;
//..............................................................


// Kích thước bộ đệm 64 có vẻ là tối ưu. Ở 32 và 128, âm thanh có thể bị rè.
uint8_t mp3buff[64];

void setup() {
    pinMode(Button1, INPUT_PULLUP);
    pinMode(Button2, INPUT_PULLUP);
    
    for(int i=0; i< 5; i++)
    {
      
    }
    Serial.begin(115200);
    delay(3000);
    Serial.println("\n\n..............ESP INTERNET RADIO.............");

    SPI.begin();

    player.begin();
    if (player.getChipVersion() == 4) {
        player.loadDefaultVs1053Patches(); 
    }
    player.switchToMp3Mode();
    player.setVolume(VOLUME);  //TĂNG GIẢM VOLUME

    Serial.print("Connecting to SSID ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("connecting to: ");
    Serial.println(host);

    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed!");
        return;
    }

    Serial.print("Requesting stream: ");
    Serial.println(path);

    client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
}

void loop() {
    if (!client.connected()) {
        Serial.println("Reconnecting...");
        if (client.connect(host, httpPort)) {
            client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        }
    }

    if (client.available() > 0) {
        // The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
        uint8_t bytesread = client.read(mp3buff, 64);
        player.playChunk(mp3buff, bytesread);
    }
}
