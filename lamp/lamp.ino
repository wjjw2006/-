//引入ESP8266.h头文件，建议使用教程中修改后的文件
#include "ESP8266.h"
//#include "dht11.h"
#include "SoftwareSerial.h"

//配置ESP8266WIFI设置
#define SSID "HONOR V20"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "7479680bin"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "657298466"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "lAm78PucdgHsv8nuGWcJIke2Iqg="; //与设备绑定的APIKey

#define INTERVAL_SENSOR 1000 //定义传感器采样及发送时间间隔

//创建dht11示例

//dht11 DHT11;

//定义DHT11接入Arduino的管脚

//定义ESP8266所连接的软串口
/*********************
 * 该实验需要使用软串口
 * Arduino上的软串口RX定义为D3,
 * 接ESP8266上的TX口,
 * Arduino上的软串口TX定义为D2,
 * 接ESP8266上的RX口.
 * D3和D2可以自定义,
 * 但接ESP8266时必须恰好相反
 *********************/
SoftwareSerial mySerial(3, 2);
ESP8266 wifi(mySerial);

void setup()
{
  pinMode(9,OUTPUT);pinMode(10,OUTPUT);pinMode(11,OUTPUT);
  mySerial.begin(115200); //初始化软串口
  Serial.begin(9600);     //初始化串口
  Serial.print("setup begin\r\n");

  //以下为ESP8266初始化的代码
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  //ESP8266接入WIFI
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("setup end\r\n");
}

unsigned long net_time1 = millis(); //数据上传服务器时间
void Light(int Status,int light){//手动模式
  if(Status==0){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
  }
  else{
    analogWrite(9,255/5*light);
    analogWrite(10,255/5*light);
    analogWrite(11,255/5*light);
  }
}
void Ilight(int Status){//智能模式
  //Serial.print(1);
  if(Status==0){
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
  }
  else{
    int n=analogRead(A3)-150;
    int light=min(n,255);
    light=max(0,light);
    Serial.print(light);
    analogWrite(9,light);
    analogWrite(10,light);
    analogWrite(11,light);
  }
}
void loop(){
  int Status=0,light=0,pattern=-1;
  if (net_time1 > millis())
    net_time1 = millis();

  if (millis() - net_time1 > INTERVAL_SENSOR) //发送数据时间间隔
  {
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
      Serial.print("create tcp ok\r\n");
      //拼接GET请求字符串
      String getString = "GET /devices/";
      getString += DEVICE_ID;
      getString += "/datastreams?datastream_ids=status,light,pattern HTTP/1.1";
      getString += "\r\n";
      getString += "api-key:";
      getString += APIKey;
      getString += "\r\n";
      getString += "Host:api.heclouds.com\r\n";
      getString += "Connection:close\r\n";
      getString += "\r\n";

      const char *getArray = getString.c_str(); //将str转化为char数组

      Serial.println(getArray);
      wifi.send((const uint8_t *)getArray, strlen(getArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
      Serial.println("send success");
      uint8_t buffer[512]={0};
      uint32_t len=wifi.recv(buffer,sizeof(buffer),20000);
      Serial.println(len);
      Serial.println("recv success");
      /*char ch[512];
      for(uint32_t i=0;i<len;i++){
        ch[i]=(char)buffer[i];
        Serial.print(ch[i]);
      }*/ 
      //提取数据
      for(int i=0;i<int(len);i++){
        if((char)buffer[i]=='s' && (char)buffer[i+1]=='t' && (char)buffer[i+2]=='a' && (char)buffer[i+3]=='t' && (char)buffer[i+4]=='u' && (char)buffer[i+5]=='s'){
          Status=(char)buffer[i+60]-'0';
        }
        if((char)buffer[i]=='l' && (char)buffer[i+1]=='i' && (char)buffer[i+2]=='g' && (char)buffer[i+3]=='h' && (char)buffer[i+4]=='t'){
          light=(char)buffer[i+59]-'0';
        }
        if((char)buffer[i]=='p' && (char)buffer[i+1]=='a' && (char)buffer[i+2]=='t' && (char)buffer[i+3]=='t' && (char)buffer[i+4]=='e' && (char)buffer[i+5]=='r' && (char)buffer[i+6]=='n'){
          pattern=(char)buffer[i+61]-'0';
        }
      }
      Serial.print(Status);
      Serial.print(light);
      Serial.print(pattern);
      
      if (wifi.releaseTCP()) { //释放TCP连接
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      getArray = NULL; //清空数组，等待下次传输数据
     
     
      if(pattern==1)Light(Status,light);
      if(pattern==0)Ilight(Status);
    } else {
      Serial.print("create tcp err\r\n");
    }

    Serial.println("");
    
    net_time1 = millis();
    
   
  }
}
