/*
  =================================================================================                         
                            | 什么是看谁手速快? |
  ---------------------------------------------------------------------------------                         
  你是不是想过和你的朋友比比谁的反应更快？这个机器会告诉你答案！挑战你的朋友，
  家人，老师……甚至是你家的小狗。这将会是一场世纪对决。
    
  =================================================================================
                                  | 怎么玩|
  ---------------------------------------------------------------------------------
  1. 每个玩家把一个手指放在碰撞传感器上。
  2. 注意: 请准确地按照说明书里的接线图连接。特定的碰撞传感器需要与相应彩灯对应。
  3. 摁住一个按钮开始游戏!
  4. ColorLED将会闪烁并发出滴滴声，红，红，绿。最后一声响完，游戏开始！
  5. 两个ColorLED出现相同颜色时，立即按下各自的碰撞传感器! 先按下的玩家将获得一分。
     两个ColorLED的颜色不同时，千万别按碰撞传感器，否则对方玩家将得一分。
       a. 如果颜色正确，LED灯将会为先按的玩家闪绿光。
       b. 如果颜色不正确，LED灯将会为先按的玩家闪红光。 
       c. 然后得分高的玩家的LED将会闪白光，闪烁次数即是这个玩家领先的分数。
    例如，如果玩家B领先3分，他的LED灯将闪白光3次。 
  6. 领先5分的玩家将会胜出.
  7. 一旦有玩家获胜音乐就会响起!
  
  注意: 游戏结束后，随时摁住一个按钮即可重新开始游戏。 
  =================================================================================
                                    | 开始 |
  ---------------------------------------------------------------------------------
  用传感器创造一个有趣的互动游戏。
  =================================================================================
                           | 自己动手修改代码! |
  ---------------------------------------------------------------------------------
  点开 userDef.h 文件，然后根据你自己的喜好更改下面的参数。 :)
  
  BRIGHT_MAX:   LED彩灯显示的最大亮度。
  LED_INTERVAL: LED彩灯颜色切换的速度。 
  SCORE_MAX:    获胜条件的分差值。
  
  修改完这些值后，记得再把代码上传到mBattery里。
  =================================================================================
                              |头脑风暴|
  ---------------------------------------------------------------------------------                           
  1. 可以通过修改代码来改变这个游戏吗?
     例如, "在10s内谁按的按钮次数最多?"
  2. 是否可以把代码修改成只有当ColorLED颜色不同时才按碰撞开关?
  3. 是否可以做出一个这样的游戏：
     还是使用两个ColorLED，一左一右，两个ColorLED会做出一些闪烁动作。然后有两个碰撞传感器，同样一左一右，去重复ColorLED的闪烁动作！
       
    注意: 以上功能上的变动会涉及到程序上的改动。    
  =================================================================================
                               | 端口连接 |
  ---------------------------------------------------------------------------------
  蜂鸣器:       D10
  ColorLED:     D12
  碰撞传感器 A: D6
  碰撞传感器 B: D8 
  =================================================================================
                                  |问题 |
  ---------------------------------------------------------------------------------                                 
  欢迎访问 http://forum.microduino.cn/，在我们的论坛提交您的问题， 我们会尽快回复您。
*/

#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"

Key keyA(PIN_KEYA, INPUT);
Key keyB(PIN_KEYB, INPUT);

uint32_t ledTimer;
uint8_t scoreA, scoreB;
int8_t score;
uint8_t ledX, ledY;
bool playStatus;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  randomSeed(80);
  soundInit();
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
  if (playStatus)           //游戏是否在进行
  {
    int8_t scoreCache = score;
    score = updateScore();  //计算分数差值。
    if (abs(score) >= SCORE_MAX)  //游戏结束。
    {
      playStatus = false;
      playIndex = 0;
    }
    else if (score != scoreCache)
    {
      ledScore(score);          //闪烁分数差。
    }
    updateLed();//闪烁新颜色。
  }
  else if (!playSound(10))    //伴随音乐玩游戏。
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_GREEN, 15);
    if (playReset())
      playStatus = true;
  }
}

//--------------闪烁新颜色-----------------//
void updateLed()
{
  if (millis() > ledTimer)
  {
    if (ledX == 0 || ledY == 0)
    {
      ledX = random(5,9);
      ledY = random(5,9);
      ledTimer = millis() + random(LED_INTERVAL - 200, LED_INTERVAL);//闪烁一定时间。
    }
    else
    {
      ledX = 0;
      ledY = 0;
      ledTimer = millis() + LED_INTERVAL / 2;   //熄灭一定时间。
    }
    setLed(ledX, 0);
    setLed(ledY, 1);
  }
}

void soundInit()
{
  setAllLed(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_RED);
  tone(PIN_BUZZER, 1000);
  delay(500);
  setAllLed(COLOR_NONE);
  noTone(PIN_BUZZER);
  delay(500);
  setAllLed(COLOR_GREEN);
  tone(PIN_BUZZER, 1500);
  delay(500);
  noTone(PIN_BUZZER);
  setAllLed(COLOR_NONE);
}
//---------------闪烁分数差-----------------//
void ledScore(int8_t _score)
{
  setAllLed(COLOR_NONE);
  if (_score > 0)
    ledBlinkNum(_score, COLOR_COLD, 0, 300);
  else if (_score < 0)
    ledBlinkNum(abs(_score), COLOR_COLD, 1, 300);
}
//---------------计算得分-----------------//
int8_t updateScore()
{
  if (keyA.read() == SHORT_PRESS)
  {
    setLed(COLOR_NONE, 1);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      scoreA++;
      ledBlinkNum(1, COLOR_GREEN, 0, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      scoreB++;
      ledBlinkNum(1, COLOR_RED, 0, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  else if (keyB.read() == SHORT_PRESS)
  {
    setLed(COLOR_NONE, 0);
    if (ledX == ledY && ledX > 0)
    {
      tone(PIN_BUZZER, 500, 300);
      scoreB++;
      ledBlinkNum(1, COLOR_GREEN, 1, 600);
    }
    else
    {
      tone(PIN_BUZZER, 1000, 300);
      scoreA++;
      ledBlinkNum(1, COLOR_RED, 1, 600);
    }
    ledX = 0;
    ledY = 0;
  }
  return (scoreA - scoreB);
}

bool playReset()
{
  if (keyA.read() == LONG_PRESS || keyB.read() == LONG_PRESS)
  {
    scoreA = 0;
    scoreB = 0;
    soundInit();
    return true;
  }
  else
    return false;
}
