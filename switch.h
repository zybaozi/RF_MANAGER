#include <string>
#include "RCSwitch.h"
#include "TCSwitch.h"


RCSwitch Switch_433 = RCSwitch();
TCSwitch Switch_315 = TCSwitch();
uint32_t SwitchTime;
void SwitchBegin() {

  Switch_433.enableReceive(12);   // 接收器
  Switch_433.enableTransmit(16);  // 发送器
  Switch_315.enableReceive(13);   // 接收器
  Switch_315.enableTransmit(15);  // 发送器
}

void enabled(bool a) {
  if (a) {
    Switch_433.enableTransmit(16);
  } else {
    Switch_315.enableTransmit(15);
  }
}

void disable() {
  Switch_433.disableTransmit();
  Switch_315.disableTransmit();
}
void test() {
  Switch_433.setProtocol(1);
  Switch_433.setPulseLength(258);
  Switch_433.send(8127816, 23);
}
void SwitchAvailable() {
  uint32_t Switch;
  String Base10;

  Switch = millis();
  if (menu.reviceMode) {
    if (!menu.isMain && menu.indexMain == 1) {
      if (Switch > SwitchTime & Switch - SwitchTime > 10) {
        //延迟10毫秒
        SwitchTime = Switch;
        if (Switch_433.available()) {
          // DebugPrintln(Switch_433.getReceivedValue());
          // DebugPrintln(Switch_433.getReceivedBitlength());
          // DebugPrintln(Switch_433.getReceivedDelay());
          // //  DebugPrintln( Switch_433.getReceivedRawdata());
          // DebugPrintln(Switch_433.getReceivedProtocol());


          Base10 = String(Switch_433.getReceivedValue());
          if (Base10.length() > 3) {
            bool List_Base10 = Get_ListExisBase10(Base10);
            if (!List_Base10) {
              bool state = File_save("433_" + Base10, "F4", Base10, Switch_433.getReceivedBitlength(), Switch_433.getReceivedDelay(), Switch_433.getReceivedProtocol());
              if (state) {
                DeleString("[删除全部数据]");
                List1("433_" + Base10, Base10);  //将文件名添加到链表
                List1("[删除全部数据]", "");
                drawReviceMenu("433_" + Base10, "文件保存成功");
              } else {
                drawReviceMenu("433_" + Base10, "文件保存失败");
              }

            } else {

              drawReviceMenu(Get_ListBase10_Name(Base10), "文件已存在");
            }
          }
          Switch_433.resetAvailable();
        }
        if (Switch_315.available()) {
          Base10 = String(Switch_315.getReceivedValue());
          if (Base10.length() > 3) {
            bool List_Base10 = Get_ListExisBase10(Base10);
            if (!List_Base10) {
              bool state = File_save("315_" + Base10, "F3", Base10, Switch_315.getReceivedBitlength(), Switch_315.getReceivedDelay(), Switch_315.getReceivedProtocol());
              if (state) {
                DeleString("[删除全部数据]");
                List1("315_" + Base10, Base10);  //将文件名添加到链表
                List1("[删除全部数据]", "");
                drawReviceMenu("315_" + Base10, "文件保存成功");
              } else {
                drawReviceMenu("315_" + Base10, "文件保存失败");
              }

            } else {

              drawReviceMenu(Get_ListBase10_Name(Base10), "文件已存在");
            }
          }

          Switch_315.resetAvailable();
        }
      }
    }
  }
}

void SwitchSend(String filename) {
  String data = Read(filename);
  String pattern = data.substring(0, 2);
  DebugPrintln(filename);
  DebugPrintln(data);
  String Base10_length = data.substring(2, 4);
  String Base10 = data.substring(4, Base10_length.toInt() + 4);
  String Position = data.substring(Base10_length.toInt() + 4, Base10_length.toInt() + 7);
  String Pulse = data.substring(Base10_length.toInt() + 7, Base10_length.toInt() + 12);
  String Protocol = data.substring(Base10_length.toInt() + 12, Base10_length.toInt() + 14);
  DebugPrintln(filename);
  DebugPrintln(data);
  DebugPrintln(pattern);
  DebugPrintln(Base10_length);
  DebugPrintln(Base10);
  DebugPrintln(Position);
  DebugPrintln(Pulse);
  DebugPrintln(Protocol);

  if (pattern == "F4") {
    Switch_433.setProtocol(Protocol.toInt());
    Switch_433.setPulseLength(Pulse.toInt());
    Switch_433.send(atoi(Base10.c_str()), Position.toInt());
  }

  if (pattern == "F3") {
    Switch_315.setProtocol(Protocol.toInt());
    Switch_315.setPulseLength(Pulse.toInt());
    Switch_315.send(atoi(Base10.c_str()), Position.toInt());
  }
}