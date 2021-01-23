#ifndef HARDIIC_H
#define HARDIIC_H

void InitIICForEEPROM();
int  IICForEEPROMWriteBytes(unsigned long long addr, const char* src, int length);
int  IICForEEPROMReadBytes(unsigned long long addr, char* dest, int length);

#endif