#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ACROBOTIC_SSD1306.h>
#include <Wire.h>

boolean connectionWasAlive = true;

const char* ssid = "SUNTORY";
const char* password = "1q2w3e4r";

const String url = "http://quan.suning.com/getSysTime.do";//主机IP：端口/路径 或 网址
int httpCode;
String payload;
String inputString = "";  //接收到的数据
bool GetDate=false;
int Year;
int Month;
int Day;
int Hour;
int Minute;
int Second;

void setup() {
  // put your setup code here, to run once:
Wire.begin();
oled.init();
oled.setFont(font5x7);
oled.clearDisplay();
oled.setTextXY(0,0);
WiFi.begin(ssid, password);
Serial.println("");           
Serial.print("Waiting for connecting to ");
Serial.println(ssid);
oled.putString("");
oled.putString("Waiting for connecting to ");
oled.setTextXY(1,0);
oled.putString(ssid);

//wait for wifi connection
while (WiFi.status() != WL_CONNECTED)
{
  /* code */
  delay(500);
  Serial.print(".");
  oled.putString(".");
}
Serial.println("");
Serial.println("Wifi connected!"); 
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
oled.setTextXY(2,0);
oled.putString("");
oled.putString("Wifi connected!"); 
oled.setTextXY(3,0);
oled.putString("IP address: ");
oled.putString(IPAddress2String(WiFi.localIP()));
oled.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
GetTime();
DataHandle();
oled.setFont(font8x8);
  //oled.putString("Data");
  oled.putString(num2Str(Year));
  oled.putString(":");
  oled.putString(num2Str(Month));
  oled.putString(":");
  oled.putString(num2Str(Day));
  oled.setTextXY(2,0);
  oled.putString(num2Str(Hour));
  oled.putString(":");
  oled.putString(num2Str(Minute));
//  oled.putString(":");
//  oled.putString(num2Str(Second));/**/ 
  delay(60000);
  oled.clearDisplay();
  
}

void GetTime(){
if (WiFi.status() == WL_CONNECTED)
{
  /* code */
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "text/plain");  //设置消息类型
  httpCode = http.POST("Message from ESP8266");   //发送请求
  payload = http.getString();                  //获取返回payload
  http.end();
}
else
{
  Serial.println("WiFi Disconnected!!");
  oled.clearDisplay();
  oled.setTextXY(0,0);
  oled.putString("WiFi Disconnected!!");
}
delay(1000);
GetDate = true;
}

void DataHandle()
{
    int jsonBeginAt = payload.indexOf("{");   //判断json数据完整性
    int jsonEndAt = payload.lastIndexOf("}");
    if (jsonBeginAt != -1 && jsonEndAt != -1) 
    {
        if(GetDate==true)
        {
                //净化json数据
            payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//取得一个完整的JSON字符串
            
            const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
            DynamicJsonDocument doc(capacity);
            //const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
            deserializeJson(doc, payload);//解析payload
            
            const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"
            const char* sysTime1 = doc["sysTime1"]; // "20200307191328"
        
            String Date=sysTime1;
            //int nian = sysTime2.lastIndexOf("-");//定位到某处
            Year=(Date.substring(0,4)).toInt();//截取（下标从0开始）[0,4)的数据
            Month=(Date.substring(4,6)).toInt();
            Day=(Date.substring(6,8)).toInt();
            Hour=(Date.substring(8,10)).toInt();
            Minute=(Date.substring(10,12)).toInt();
            Second=(Date.substring(12,14)).toInt();
            //测试用
            Serial.println(Date);
            Serial.println(Year);
            Serial.println(Second);
            
            GetDate=false;
        }
    }
}


String IPAddress2String(IPAddress address)
{
 return String(address[0]) + "." +
        String(address[1]) + "." +
        String(address[2]) + "." +
        String(address[3]);
}

String num2Str(int number)
{
  return String(number);
}
