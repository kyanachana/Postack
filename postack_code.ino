
/*
 * IFTTTを使ってm5stackとlineを連携させるプログラム
 * 参照元：https://thousandiy.wordpress.com/2017/10/17/esp32_ifttt_line/
 */
#define M5STACK_MPU6886 
#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Avatar.h>
#include <faces/DogFace.h>

//アバターの変数
using namespace m5avatar;
Avatar avatar;

//ジャイロの変数
float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;


bool avatar_angry = false;
//アバターが喋る内容
const char* lyrics[] = {"READ ME",     "Answer",     "this question!",
                         "What's",      "the zip code",    
                         "of my house?  ",     "A.111-1111",   "B.222-2222", "C.333-3333",
                         "Press",         "the Button!","A        B        C  "};                      
const int lyricsSize = sizeof(lyrics) / sizeof(char*);
int lyricsIdx = 0;

//wifiのssid,password設定に合わせる
const char* ssid     = "BCW710J-39720-G";
const char* password = "3a5e54dc5383a";

//IFTTTの設定に合わせる
String makerEvent = "push_line"; // Maker Webhooks
String makerKey = "bJ--W24ke4g0dhLX5MAc6EMG91wTQLLVVafFHAL9j_c"; // Maker Webhooks

const char* server = "maker.ifttt.com";  // Server URL
WiFiClient client;

//wifiのcheck
bool checkWifiConnected() {
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  return true;
}

//wifiにつなげる
void send(String value1, String value2, String value3) {
  while (!checkWifiConnected()) {
    Serial.print("Attempting to connect to WiFi");
    WiFi.begin(ssid, password);
  }

  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 80)) {
    Serial.println("Connection failed!");
  } else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    String url = "/trigger/" + makerEvent + "/with/key/" + makerKey;
    url += "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;
    client.println("GET " + url + " HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    Serial.print("Waiting for response "); //WiFiClientSecure uses a non blocking implementation

    int count = 0;
    while (!client.available()) {
      delay(50); //
      Serial.print(".");
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
    }
  }
}


//セットアップ（最初のみ行われる）
void setup() {
  M5.begin();
  M5.Power.begin();

  //画面の初期化
  M5.Lcd.setBrightness(30);
  M5.Lcd.clear();

  //アバターの初期化
  avatar.init();
  avatar.setExpression(Expression::Sleepy);

  //ジャイロの初期化
  M5.IMU.Init();
  //Initialize serial and wait for port to open:

  M5.Speaker.begin();

  //wifi
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  while (!checkWifiConnected()) {
    WiFi.begin(ssid, password);
  }
}


void angry(){
  while(lyricsIdx%lyricsSize!=0){
     
      const char* l = lyrics[lyricsIdx++ % lyricsSize];
      avatar.setSpeechText(l);
      avatar.setMouthOpenRatio(0.7);
      delay(500);
      avatar.setMouthOpenRatio(0);

    
  }

}




//繰り返し実行される
void loop() {  
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  if(pow(gyroX,2)+pow(gyroY,2)+pow(gyroZ,2)>4000&&avatar_angry==false){
    avatar.setExpression(Expression::Angry);

    const char* l = lyrics[lyricsIdx++ % lyricsSize];
    avatar.setSpeechText(l);
    avatar.setMouthOpenRatio(0.7);
    delay(500);
    avatar.setMouthOpenRatio(0);

    angry();

    avatar.setExpression(Expression::Neutral);
    avatar_angry = true;
    
  }

  if(avatar_angry){
    M5.update();

    if (M5.BtnA.wasReleased()) {
      avatar.setExpression(Expression::Angry);
      M5.Speaker.tone(960,200);
      delay(50);
      avatar.setSpeechText("Don't visit ");
      delay(500);
      avatar.setSpeechText("my house again!! ");
      delay(500);
      avatar_angry = false;
      delay(10000);
      M5.Speaker.mute();
    }else if (M5.BtnC.wasReleased()) {
      avatar.setExpression(Expression::Angry);
      M5.Speaker.tone(960,200);
      

      delay(50);
      avatar.setSpeechText("Don't visit ");
      delay(500);
      avatar.setSpeechText("my house again!! ");
      delay(500);
      avatar_angry = false;
      delay(10000);
      M5.Speaker.mute();
    }else if (M5.BtnB.wasReleased()) {
      send("","",""); //任意の文字列3つ
      
      avatar.setSpeechText("Thank you!!!!");
      avatar.setExpression(Expression::Happy);
      avatar_angry = false;
      delay(20000);
      
    }

    if(avatar_angry==false){
      avatar.setSpeechText("");
      avatar.setExpression(Expression::Sleepy);
    }
  } 

  delay(20);
}
