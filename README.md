# 基于STM32F107的互联空调主控
基于STM32F107的互联空调主控实现、触屏调节、手机APP控制、温度报警等功能。
[手机APP地址](https://github.com/xsj321/Connect-air-conditioner-to-master-Android-APP/tree/master)

----------

  ## 基于神州STM32F107开发板开发实现了一下主要功能:


1. 显示环境温度与空调温度信息于LCD屏幕；
  
2. 通过触摸实现对温度的调节；
  
3. 对环境有异常发出报警（温度湿度错误）；
  
4. 将数据发送到内网内装有APP的手机；
  
5. 将手机APP中的设定进行同步。

---------
  
## 使用到的传感器和设备：
1. DHT11；

2. 蜂鸣器；

3. LCD触屏；

4. Wifi透传模块USR-C215[相关资料](http://www.usr.cn/Product/159.html)
  
## 与安卓APP数据交互方式:
 
- 使用Socket编程的方式
