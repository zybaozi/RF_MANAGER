#include "OneButton.h"

uint32_t keyTime;
uint32_t keyTime1;
// #include "switch.h"
#define UP_KEY 0
#define M_KEY 2
#define DOWN_KEY 14

OneButton button1(UP_KEY, true);
OneButton button2(M_KEY, true);
OneButton button3(DOWN_KEY, true);
void click1();
void click2();
void click3();
void longPressStart1();
void longPressStart2();
void longPressStart3();
void longPressStop1();
void longPressStop2();
void longPressStop3();
void longPress1();
void longPress2();
void longPress3();



//按键初始化函数
void keyBegin() {
  // menu.Rolling = Get_Rolling();
  button1.attachClick(click1);                    //上翻单击事件
  button1.attachLongPressStart(longPressStart1);  //上翻长按开始
  button1.attachLongPressStop(longPressStop1);    //上翻长按结束
  button1.attachDuringLongPress(longPress1);      //上翻长按期间
  button1.setDebounceTicks(60);                   //按键消抖时间

  button2.attachClick(click2);
  button2.attachLongPressStart(longPressStart2);
  button2.attachLongPressStop(longPressStop2);
  button2.attachDuringLongPress(longPress2);
  button2.setDebounceTicks(60);

  button3.attachClick(click3);
  button3.attachLongPressStart(longPressStart3);
  button3.attachLongPressStop(longPressStop3);
  button3.attachDuringLongPress(longPress3);
  button3.setDebounceTicks(60);
}

//按键循环检测函数
void keyloop() {
  uint32_t keyloop;
  keyloop = millis();
  if (keyloop > keyTime & keyloop - keyTime > 1) {
    keyTime = keyloop;
    button1.tick();
    button2.tick();
    button3.tick();
  }
}
int selectedOption = 0;
//上翻单击事件
void click1() {
  DebugPrintln("上翻单击事件");
  //判断是不是首页菜单 是的话进行上下选择
  if (menu.isMain) {
    menu.indexMain = (menu.indexMain + 2) % 4 + 1;
    drawMainMenu(menu.indexMain);
    return;
  } else {                      //不是首页
    if (menu.indexMain == 2) {  //说明在发送页面里
      --menu.sendIndex;
      if (menu.sendIndex < 0) { menu.sendIndex = myAnimalList.size() - 2; }  //防止越界 和循环
      drawSendMenu(menu.sendIndex);
    }
    if (menu.indexMain == 4) {  //说明在删除页面里
      --menu.delIndex;
      if (menu.delIndex < 0) {
        if (myAnimalList.size() - 2 == 0) {
          menu.delIndex = myAnimalList.size() - 2;
        } else {
          menu.delIndex = myAnimalList.size() - 1;
        }
      }
      drawDelMenu(menu.delIndex);
      return;
    }
  }
}

//确认单击事件
void click2() {
  DebugPrintln("确认单击事件");
  //判断是不是首页菜单 是的话准备进入子菜单
  if (menu.isMain) {
    if (menu.indexMain == 1) {
      Switch_433.resetAvailable();  //进入接收菜单前清除数据
      Switch_315.resetAvailable();  //进入接收菜单前清除数据
      drawReviceMenu("", "");
      menu.reviceMode = true;
    }
    if (menu.indexMain == 2) {
      drawSendMenu(menu.sendIndex);
      //   //
      // test();
    }
    if (menu.indexMain == 3) {
      drawWIFIMenu();
      wifibegin();
    }
    if (menu.indexMain == 4) {
      drawDelMenu(menu.delIndex);
    }

    menu.isMain = false;  //表明不在首页
    return;
  } else {                      //不是首页
    if (menu.indexMain == 1) {  //返回首页并停止接收
      menu.reviceMode = false;
      drawMainMenu(menu.indexMain);
      menu.isMain = true;
      return;
    }
    if (menu.indexMain == 2) {  //判断是不是在接收菜单
      DebugPrintln(menu.sendIndex);
      DebugPrintln(myAnimalList.size());
      if (menu.sendIndex == 0) {
        drawMainMenu(menu.indexMain);
        menu.isMain = true;

        return;
      }
      if (menu.sendIndex > 0 & menu.sendIndex < myAnimalList.size() - 1) {
        SwitchSend(retList(menu.sendIndex));
        DebugPrintln(retList(menu.sendIndex));
      }
      return;
    }
    if (menu.indexMain == 3) {
      DebugPrintln(" wifi");
      if (menu.delIndex == 0) {  //索引为0 为返回上级
        drawMainMenu(menu.indexMain);
        menu.isMain = true;
        Close_Wifi();
        return;
      }
    }
    if (menu.indexMain == 4) {
      DebugPrintln("在删除页面");
      if (menu.delIndex == 0) {  //索引为0 为返回上级
        drawMainMenu(menu.indexMain);
        menu.isMain = true;
        return;
      }
      if (menu.delIndex > 0 & menu.delIndex < myAnimalList.size() - 1) {  //索引不为0和最后一个 为删除功能
        String Read_RF = retList(menu.delIndex);

        Deledata(Read_RF);
        DeleList(menu.delIndex);
        drawDelMenu(menu.delIndex);
        if (menu.delIndex == myAnimalList.size() - 1) { --menu.delIndex; }
        return;
      }
      if (menu.delIndex == myAnimalList.size() - 1) {
        // Delete_all(menu.Main_menu_inde4);
        LittleFS.format();
        u8g2.clear();
        myAnimalList.clear();
        List1("[返回上级]", "");
        List1("[删除全部数据]", "");
        menu.delIndex = 0;
        drawDelMenu(menu.delIndex);
        return;
      }
    }

    return;
  }
}





//下翻单击事件
void click3() {
  DebugPrintln("下翻单击事件");
  //判断是不是首页菜单 是的话进行上下选择
  if (menu.isMain) {
    menu.indexMain = (menu.indexMain % 4) + 1;
    drawMainMenu(menu.indexMain);
    return;
  } else {                      //不是首页
    if (menu.indexMain == 2) {  //说明在发送页面里
      ++menu.sendIndex;
      if (menu.sendIndex >= myAnimalList.size() - 1) { menu.sendIndex = 0; }  //防止越界 和循环
      drawSendMenu(menu.sendIndex);
    }
    if (menu.indexMain == 4) {  //说明在删除页面里
      ++menu.delIndex;
      if (myAnimalList.size() - 2 == 0) {
        menu.delIndex = 0;
      }

      if (menu.delIndex == myAnimalList.size()) { menu.delIndex = 0; }
      //防止越界 和循环
      drawDelMenu(menu.delIndex);
    }
  }
}

//上翻长按开始
void longPressStart1() {
  DebugPrintln("上翻长按开始");
}

//上翻长按停止
void longPressStop1() {
  DebugPrintln("上翻长按停止");
}

//确认长按开始
void longPressStart2() {
  DebugPrintln("确认长按开始");
}

//确认长按停止
void longPressStop2() {
  DebugPrintln("确认长按停止");
  // if (!menu.isMain) {
  //   drawMainMenu(menu.indexMain);
  //   menu.isMain = true;
  // }
  // return;
}

//下翻长按开始
void longPressStart3() {
  DebugPrintln("下翻长按开始");
}

//下翻长按停止
void longPressStop3() {
  DebugPrintln("下翻长按停止");
}

//上翻长按期间
void longPress1() {
  DebugPrintln("上翻长按期间");
}

//确认长按期间
void longPress2() {
}
//下翻长按期间
void longPress3() {
}