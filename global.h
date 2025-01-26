#ifndef GLOBAL_H
#define GLOBAL_H

// 声明一个全局变量
struct Menu_structure {
  bool isMain;//是否是主菜单
  int indexMain;//主菜单的索引  1.接受  2.发送 3.wifi 4.删除
  // int indexSubMenu;//子菜单的索引  1.接受  2.发送 3.wifi 4.删除
  int sendIndex; //用来控制发射列表的索引
  bool reviceMode; //是否处于接收模式
  int delIndex; //用来控制删除模式的索引
  
} menu = {true,1,0,false,0};
#endif