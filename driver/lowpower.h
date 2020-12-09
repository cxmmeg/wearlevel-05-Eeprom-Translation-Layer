#ifndef LOWPOWER_H
#define LOWPOWER_H

void BoardPowerOn();
void BoardPowerOff();
void CalcNextWakeupTime(char* nextHour, char* nextMin);
void SleepIfWakeupAcceidently();
void SetSleepInterval(int interval, int interval_type);
void gotoSleepLPM3();

#endif