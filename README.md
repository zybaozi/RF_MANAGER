# RF_MANAGER

## 射频管家代码。

兼容立创广场射频管家1.3的PCB https://oshwhub.com/ys-tao/shuang-pin-ye-pin-guan-jia-v1-3 



思路及 很多核心代码来自于  https://github.com/sprlightning/RF-Master

只有四个界面

- 监听
- 发射
- AP模式
- 删除

改名 查看 编辑 删除均正常。 支持手动新增，方便初始化设备和备份信号用。

所有引脚均为代码中定义，更精简一些。

注：
1.[ESPAsyncWebServer](https://github.com/sprlightning/ESPAsyncWebServer) 库需要手动导入，已经在代码中。感谢sprlightning。
2.如果需要修改网页 请修改index.html。然后使用conver.py生成代码文件。

请勿用于非法用途。


使用方法：
arduino编译。 选择开发板为nodemcu 12E。 其他好像没啥注意事项了。
