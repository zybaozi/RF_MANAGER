#define DebugBegin(message) Serial.begin(message)
#define DebugPrint(message) Serial.print(message)
#define DebugPrintln(message) Serial.println(message)

#include "global.h"

#include "Gr_DataSave.h"
#include "display.h"
#include "switch.h"
#include "website.h"
#include "Key.h"




void setup(void) {
  DebugBegin(115200);
  DateBegin();                   //开启存储
  keyBegin();                    //键盘监听
  u8g2begin();                   //屏幕绘制
  drawMainMenu(menu.indexMain);  //绘制首页
  SwitchBegin();                 //无线接收初始化
  Enumerate_Events();            //初始化

//初始化webserver  
  httpServer();

  //wifibegin();



}

void loop(void) {
  keyloop();
  SwitchAvailable();
}

void Enumerate_Events() {
  Dir dir = LittleFS.openDir("/");
  List1("[返回上级]", "");
  String filename;
  while (dir.next()) {
    filename = dir.fileName();
    if (filename != "System"/* && filename != "web"*/) {
      List1(filename, Base10_1(filename));
    }
  }
  List1("[删除全部数据]", "");
}