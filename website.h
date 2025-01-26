#include <ESPAsyncWebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>  // 确保你已经安装了ArduinoJson库
#include "index_html_gz.h"


AsyncWebServer server(80);

void wifibegin() {

  // //调试用
  // WiFi.mode(WIFI_STA);  //首先开启sta模式
  // String ssid = "MikroTik";
  // String password = "123456789";
  // DebugPrint("Auto ...");  //屏幕显示

  //   WiFi.begin(ssid.c_str(), password.c_str());

  //   for (int i = 0; i < 20; i++) {
  //     int wstatus = WiFi.status();
  //     if (wstatus == WL_CONNECTED) {
  //       DebugPrint("Auto ...ok!");
  //       //连接成功返回真
  //       return ;
  //     } else {
  //       DebugPrint("Auto ... " + String(i) + "s");
  //       delay(1000);
  //     }
  //   }


  //正式用
  WiFi.mode(WIFI_AP);
  const char *ssid = "RFmanager";
  const char *password = "12345678";

  // 自定义的IP地址、网关和子网掩码
  IPAddress local_ip(192, 168, 0, 1);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
}

void Close_Wifi() {
  WiFi.mode(WIFI_OFF);
  //webServer.stop();
}


void httpServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });
  //http://192.168.88.60/getall
  server.on("/getall", HTTP_GET, [](AsyncWebServerRequest *request) {  //获取所有数据
    String response = "[";
    for (int i = 0; i < myAnimalList.size(); i++) {
      Animal *animal = myAnimalList.get(i);
      response += "{\"name\":\"" + String(animal->name) + "\", \"Base10\":\"" + String(animal->Base10) + "\"}";
      if (i < myAnimalList.size() - 1) {
        response += ",";
      }
    }
    response += "]";

    // 发送JSON响应
    request->send(200, "application/json", response);
  });
  //http://192.168.88.60/rename?old=433_11905732&new=开门
  server.on("/rename", HTTP_GET, [](AsyncWebServerRequest *request) {  //重命名文件名
    String oldFilename;
    String newFilename;

    // 获取GET参数
    if (request->hasParam("old")) {
      oldFilename = request->getParam("old")->value();
    }
    if (request->hasParam("new")) {
      newFilename = request->getParam("new")->value();
    }

    // 调用重命名函数
    RenameList(oldFilename, newFilename);
    String data = Read(oldFilename);

    File file = LittleFS.open("/" + newFilename, "w");
    bool state = file.print(data);
    if (state) {
      file.close();
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["state"] = state;  // 将布尔值添加到JSON对象

      // 将JSON对象序列化为字符串
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Deledata(oldFilename);
      // 返回JSON响应
      request->send(200, "application/json", jsonString);
    } else {
      Deledata(newFilename);  //保存失败以后删除已经创建的文件夹
      file.close();
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["state"] = state;  // 将布尔值添加到JSON对象

      // 将JSON对象序列化为字符串
      String jsonString;
      serializeJson(jsonDoc, jsonString);

      // 返回JSON响应
      request->send(200, "application/json", jsonString);
    }





  });


  server.on("/getfile", HTTP_GET, [](AsyncWebServerRequest *request) {  //获取所有数据
    String filename;
    if (request->hasParam("filename")) {
      filename = request->getParam("filename")->value();
    }
    String data = Read(filename);
    request->send(200, "text/plain", data);
  });

  //删除文件
server.on("/deltefile", HTTP_GET, [](AsyncWebServerRequest *request) {  //获取所有数据
    String filename;
    if (request->hasParam("filename")) {
      filename = request->getParam("filename")->value();
    }
    Deledata(filename);
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["state"] = true;  // 将布尔值添加到JSON对象

    // 将JSON对象序列化为字符串
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // 返回JSON响应
    request->send(200, "application/json", jsonString);
});

  //写文件
  server.on("/writefile", HTTP_GET, [](AsyncWebServerRequest *request) {  //获取所有数据
    String filename;
    String filecode;
    if (request->hasParam("filename")) {
      filename = request->getParam("filename")->value();
    }
    if (request->hasParam("filecode")) {
      filecode = request->getParam("filecode")->value();
    }
    // DebugPrintln(filename);
    // DebugPrintln(filecode);
    bool state = File_write(filename, filecode);
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["state"] = state;  // 将布尔值添加到JSON对象

    // 将JSON对象序列化为字符串
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // 返回JSON响应
    request->send(200, "application/json", jsonString);






  });

  server.on("/modifyfile", HTTP_GET, [](AsyncWebServerRequest *request) {  //获取所有数据
    String filename;
    String filecode;
    if (request->hasParam("filename")) {
      filename = request->getParam("filename")->value();
    }
    if (request->hasParam("filecode")) {
      filecode = request->getParam("filecode")->value();
    }
    Deledata(filename);
    // DebugPrintln(filename);
    // DebugPrintln(filecode);
    bool state = File_write(filename, filecode);
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["state"] = state;  // 将布尔值添加到JSON对象

    // 将JSON对象序列化为字符串
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // 返回JSON响应
    request->send(200, "application/json", jsonString);






  });







  server.begin();
}
