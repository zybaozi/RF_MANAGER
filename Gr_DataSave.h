#include <LittleFS.h>       //文件存储库
#include "Gr_LinkedList.h"  //链表 修改版
#include <ArduinoJson.h>
FSInfo fs_info;



class Animal {
public:
  char *name;
  char *Base10;
  bool isMammal;
};

Linked<Animal *> myAnimalList = Linked<Animal *>();
String Read(String filename);
bool Get_Exis(String filename);

//文本分割
String fenge(String str, String fen, int index) {
  int weizhi;
  String temps[str.length()];
  int i = 0;
  do {
    weizhi = str.indexOf(fen);
    if (weizhi != -1) {
      temps[i] = str.substring(0, weizhi);
      str = str.substring(weizhi + fen.length(), str.length());
      i++;
    } else {
      if (str.length() > 0)
        temps[i] = str;
    }
  } while (weizhi >= 0);

  if (index > i) return "-1";
  return temps[index];
}
//-------初始化存储---------
bool DateBegin() {
  bool a = LittleFS.begin();
  LittleFS.info(fs_info);
  return a;
}







//-------文件读取---------
String Read(String filename) {
  File file = LittleFS.open("/" + filename, "r");
  if (file) {
    String str;
    while (file.available()) {
      str += (char)file.read();
    }
    file.close();
    return str;
  } else {
    return "";
  }
}

//--------文件是否存在--------
bool Get_Exis(String filename) {
  File f = LittleFS.open(filename, "r");
  return f;
}

//添加名字到链表
void List(String Str) {
  char *str;
  const int len = Str.length();
  str = new char[len + 1];

  strcpy(str, Str.c_str());
  Animal *cat = new Animal();

  cat->name = str;
  cat->isMammal = true;
  myAnimalList.add(cat);
}
//添加名字到链表
void List1(String Str, String Str1) {
  char *str;
  char *str1;
  const int len = Str.length();
  const int len1 = Str1.length();
  str = new char[len + 1];
  str1 = new char[len1 + 1];

  strcpy(str, Str.c_str());
  strcpy(str1, Str1.c_str());
  Animal *cat = new Animal();

  cat->name = str;
  cat->Base10 = str1;
  cat->isMammal = true;
  myAnimalList.add(cat);
}
//删除链表数据
void DeleList(int i) {

  myAnimalList.remove(i);
}
//根据索引取文件名字
String retList(int i) {
  char *str;
  Animal *animal;
  animal = myAnimalList.get(i);
  if (animal->isMammal) {
    str = animal->name;
    return str;
  }
  return "";//返回空字符，不然会编译失败
}
//根据索引取10进制数据
String Get_ListBase10(int i) {
  char *str;
  Animal *animal;
  animal = myAnimalList.get(i);
  if (animal->isMammal) {
    str = animal->Base10;
    return str;
  }
  return "";//返回空字符，不然会编译失败
}
//链表根据名字删除数据
void DeleString(String Str) {

  for (int i = 0; i < myAnimalList.size(); i++) {
    if (retList(i) == Str) { DeleList(i); }
  }
}
//链表根据名字修改数据 文件名 10进制数据
void RenameList(String Str, String Str1) {

  for (int i = 0; i < myAnimalList.size(); i++) {
    if (retList(i) == Str) {
      char *str;
      const int len = Str1.length();
      str = new char[len + 1];

      strcpy(str, Str1.c_str());
      Animal *cat = new Animal();
      
      cat->name = str;
      String myString = Get_ListBase10(i);
      char charArray[myString.length() + 1]; // +1 是为了存储结尾的null字符
      myString.toCharArray(charArray, sizeof(charArray)); // 将内容复制到charArray
      cat->Base10=charArray;
      cat->isMammal = true;
      myAnimalList.set(i, cat);
    }
  }
}
//删除文件
void Deledata(String filename) {
  LittleFS.remove(filename);
}

//10转2进制
static char *dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64];
  unsigned int i = 0;

  while (Dec > 0) {
    bin[32 + i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j < bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[31 + i - (j - (bitLength - i))];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';

  return bin;
}
//自动补齐
String Complement(String data , int frequency) {
  String text;
  for (int i = 0; i < frequency - data.length(); i++) {
    text = text + "0";
  }
  return text + data;
}
//文件保存
bool File_save(String filename, String pattern , String Base10 , int Position , int Pulse , int Protocol) {
  String output;
  output = pattern + Complement(String(Base10.length()) , 2) + Base10 +  Complement(String(Position) , 3)  + Complement(String(Pulse) , 5) + Complement(String(Protocol) , 2);
  DebugPrintln(output);

  File file = LittleFS.open("/" + filename, "w");
  bool state = file.print(output);
  if (state) {
    file.close();
    return true;
  } else {
    Deledata(filename);  //保存失败以后删除已经创建的文件夹
    file.close();
    return false;
  }
  return true;
}

bool File_write(String filename,String output){
  File file = LittleFS.open("/" + filename, "w");
  bool state = file.print(output);
  if (state) {
    file.close();
    return true;
  } else {
    Deledata(filename);  //保存失败以后删除已经创建的文件夹
    file.close();
    return false;
  }
  return true;
}

//获取链表文件是否存在
bool Get_ListExis(String filename) {
  for (int i = 0; i < myAnimalList.size(); i++) {
    if (retList(i) == filename) {
      return true;
    }
  }
  return false;
}

//获取链表10进制是否存在
bool Get_ListExisBase10(String filename) {
  for (int i = 0; i < myAnimalList.size(); i++) {
    if (Get_ListBase10(i) == filename) {
      return true;
    }
  }
  return false;
}

//获取链表10进制=》文件名
String Get_ListBase10_Name(String filename) {
  for (int i = 0; i < myAnimalList.size(); i++) {
    if (Get_ListBase10(i) == filename) {
      return retList(i);
    }
  }
  return "";
}
//读取链表10进制数据
String Base10_1(String filename){
  String data = Read(filename); 
  String Base10_length = data.substring(2,4);
  String Base10 = data.substring(4 , Base10_length.toInt() + 4);
  return Base10;
}
