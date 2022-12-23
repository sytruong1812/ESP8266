/*
  XSwitch - Phần mềm điều khiển công tắc qua mobile
  Copyright (C) 2016 NHX
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.
  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROM.h"
#define  BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
//12: Green
//13: Blue
//15: Red
 
ESP8266WebServer server(80);
 
const char*     ssid           = "XSwitch";
const char*     passphrase     = "123456789";
String          blynk          = "a4c47feas2e542f7ad7d5b2d5a4300a5";
int             inRelay1       = 4; //Chan GPIO14 gắn vào relay
String          st;
String          content;
int             statusCode;
 
 
void setup()
{
    Serial.begin(115200);
    pinMode(inRelay1, OUTPUT);
    //Delay 5m, Red blink
    pinMode(13, OUTPUT);
    pinMode(15, OUTPUT);
    for (int i = 1; i < 3; i++)
    {
        digitalWrite(15, HIGH);
        delay(500);
        digitalWrite(15, LOW);
        delay(500);
    }
 
    EEPROM.begin(512);
    delay(10);
    Serial.println("Startup");
    // read eeprom for ssid, pass and blynk
    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i)
    {
        esid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(esid.c_str());
    esid.trim();
 
    Serial.println("Reading EEPROM pass");
    String epass = "";
    for (int i = 32; i < 96; ++i)
    {
        epass += char(EEPROM.read(i));
    }
    Serial.print("PASS: ");
    Serial.println(epass.c_str());
    epass.trim();
 
    Serial.println("Reading EEPROM blynk");
    String eblynk = "";
    for (int i = 96; i < 128; ++i)
    {
        eblynk += char(EEPROM.read(i));
    }
    Serial.print("BLYNK: ");
    Serial.println(eblynk.c_str());
    eblynk.trim();
 
    if ( esid.length() > 1 )
    {
        WiFi.begin(esid.c_str(), epass.c_str());
        if (testWifi())
        {
            launchWeb(0);
            WiFi.disconnect();
 
            char * auth_ = new char[eblynk.length() + 1];
            eblynk.toCharArray(auth_, eblynk.length() + 1);
            Blynk.begin(auth_, esid.c_str(), epass.c_str());
            EEPROM.end();
            return;
        }
    }
    setupAP();
    EEPROM.end();
}
 
bool testWifi(void)
{
    int c = 0;
    Serial.println("Xin vui long doi ket noi WIFI");
    while ( c < 20 )
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(1000);
        Serial.print(WiFi.status());
        c++;
    }
    Serial.println("");
    Serial.println("Thoi gian ket noi cham, Mo AP");
    return false;
}
 
void launchWeb(int webtype)
{
    Serial.println("");
    Serial.println("WiFi ket noi");
    Serial.print("Dia chi IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer(webtype);
    // Start the server
    server.begin();
    Serial.println("May chu bat dau");
}
 
void setupAP(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("Tim hoan tat");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i)
    {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    Serial.println("softap");
    Serial.println(ssid);
    Serial.println(passphrase);
    WiFi.softAP(ssid, passphrase, 6);
 
    launchWeb(1);
    Serial.println("over");
}
 
void createWebServer(int webtype)
{
    if ( webtype == 1 )
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            content = "<!DOCTYPE HTML>\r\n<html><h2>XSwitch</h2>";
            //content += ipStr;
            //content += "<form method='get' action='setting'><table width='100%' border='1'><tr><td width=\"30%\"><label>Wifi</label></td><td width=\"70%\><input name='ssid' length=32 width='500'></td></tr><tr><td><label>Password</label></td><td><input name='pass' length=64 width='500'></td></tr><tr><td><label>Blynk</label></td><td><input name='blynk' length=32 width='500'></td></tr><tr><td></td><td><input type='submit'></tr></tr></table></form>";
            content += "<form method=\"get\" action=\"setting\">";
            content += "<div>Wifi</div>";
            content += "<div><input name=\"ssid\" size=\"40\"></div>";
            content += "<div>Mat Khau</div>";
            content += "<div><input name=\"pass\" size=\"40\"></div>";
            content += "<div>Blynk</div>";
            content += "<div><input name=\"blynk\" size=\"40\"></div>";
            content += "<div><input type='submit'></div>";
 
            content += "<p>";
            content += st;
            content += "</p>";
            content += "</html>";
            server.send(200, "text/html", content);
        });
        server.on("/setting", []()
        {
            String qsid = server.arg("ssid");
            String qpass = server.arg("pass");
            String qblynk = server.arg("blynk");
            if (qsid.length() > 0 && qpass.length() > 0)
            {
                EEPROM.begin(512);
                Serial.println("clearing eeprom");
                for (int i = 0; i < 128; ++i)
                {
                    EEPROM.write(i, 0);
                }
                EEPROM.commit();
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");
                Serial.println(qblynk);
                Serial.println("");
 
                Serial.println("writing eeprom ssid:");
                for (int i = 0; i < qsid.length(); ++i)
                {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                }
 
                Serial.println("writing eeprom pass:");
                for (int i = 0; i < qpass.length(); ++i)
                {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                }
 
                Serial.println("writing eeprom blynk:");
                for (int i = 0; i < qblynk.length(); ++i)
                {
                    EEPROM.write(96 + i, qblynk[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qblynk[i]);
                }
                EEPROM.commit();
                EEPROM.end();
                //Chop den xanh sau khu lam xong
                pinMode(13, OUTPUT);
                digitalWrite(13, LOW);
                digitalWrite(13, HIGH);
                delay(500);
                digitalWrite(13, LOW);
                content = "{\"Success\":\"Luu vao he thong. Khoi dong lai ten wifi moi\"}";
                statusCode = 200;
            }
            else
            {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                Serial.println("Sending 404");
            }
            server.send(statusCode, "application/json", content);
        });
    }
    else if (webtype == 0)
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.localIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
        });
        server.on("/cleareeprom", []()
        {
            content = "<!DOCTYPE HTML>\r\n<html>";
            content += "<h2>XSwitch</h2><p>Clearing the EEPROM</p></html>";
            server.send(200, "text/html", content);
            Serial.println("clearing eeprom");
            for (int i = 0; i < 128; ++i)
            {
                EEPROM.write(i, 0);
            }
            //Chop den xanh sau khu lam xong
            pinMode(13, OUTPUT);
            digitalWrite(13, LOW);
            digitalWrite(13, HIGH);
            delay(500);
            digitalWrite(13, LOW);
            EEPROM.commit();
        });
    }
}
 
void loop()
{
    server.handleClient();
    Blynk.run();
}
