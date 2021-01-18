#ifndef IIC_H
#define IIC_H

#include <stdint.h>
#include <vector>

void IICInit();
void IICWriteBytes(uint8_t slaveaddr, vector< uint8_t >& data);

#endif