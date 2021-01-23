#include "driverlib.h"
#include <vector>

int		  txlen		  = -1;
uint8_t		  transmitCounter = 0;
vector< uint8_t > transmitData;
bool		  NeedStop = false;

void InitIICForEEPROM() {

	// Stop WDT
	WDT_A_hold(WDT_A_BASE);

	// Assign I2C pins to USCI_B0
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN1 + GPIO_PIN2);

	// Initialize Master
	USCI_B_I2C_initMasterParam param = { 0 };
	param.selectClockSource		 = USCI_B_I2C_CLOCKSOURCE_SMCLK;
	param.i2cClk			 = UCS_getSMCLK();
	param.dataRate			 = USCI_B_I2C_SET_DATA_RATE_400KBPS;
	USCI_B_I2C_initMaster(USCI_B0_BASE, &param);
}

int IICForEEPROMWriteBytes(unsigned long long addr, const char* src, int length) {

	NeedStop = true;
	transmitData.clear();

	uint8_t hi = (addr >> 8) & 0xFF;
	uint8_t lo = addr & 0xFF;
	transmitData.push_back(hi);
	transmitData.push_back(lo);
	for (int i = 0; i < length; i++)
		transmitData.push_back(src[ i ]);
	txlen = 2 + length;

	// Specify slave address
	unsigned char ctrl_cmd = 0xA0 + ((addr / 0x10000) << 1);  //写命令
	USCI_B_I2C_setSlaveAddress(USCI_B0_BASE, ctrl_cmd);

	// Set Transmit mode
	USCI_B_I2C_setMode(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_MODE);

	// Enable I2C Module to start operations
	USCI_B_I2C_enable(USCI_B0_BASE);

	// Enable transmit Interrupt
	USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
	USCI_B_I2C_enableInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);

	// Delay between each transaction
	__delay_cycles(50);

	// Load TX byte counter
	transmitCounter = 1;

	// Initiate start and send first character
	USCI_B_I2C_masterSendMultiByteStart(USCI_B0_BASE, transmitData[ 0 ]);

	__no_operation();

	// Delay until transmission completes
	while (USCI_B_I2C_isBusBusy(USCI_B0_BASE))
		;
}

static void IICWriteReadAddr(unsigned long long addr) {

	NeedStop = false;
	transmitData.clear();

	uint8_t hi = (addr >> 8) & 0xFF;
	uint8_t lo = addr & 0xFF;
	transmitData.push_back(hi);
	transmitData.push_back(lo);
	txlen = 2;

	// Specify slave address
	unsigned char writecmd = 0xA0 + ((addr / 0x10000) << 1);  //写命令
	USCI_B_I2C_setSlaveAddress(USCI_B0_BASE, writecmd);

	// Set Transmit mode
	USCI_B_I2C_setMode(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_MODE);

	// Enable I2C Module to start operations
	USCI_B_I2C_enable(USCI_B0_BASE);

	// Enable transmit Interrupt
	USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
	USCI_B_I2C_enableInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);

	// Delay between each transaction
	__delay_cycles(50);

	// Load TX byte counter
	transmitCounter = 1;

	// Initiate start and send first character
	USCI_B_I2C_masterSendMultiByteStart(USCI_B0_BASE, transmitData[ 0 ]);

	__no_operation();

	// Delay until transmission completes
	while (USCI_B_I2C_isBusBusy(USCI_B0_BASE))
		;
}

unsigned char* receiveBufferPointer;
unsigned char  receiveCount = 0;

int IICForEEPROMReadBytes(unsigned long long addr, char* dest, int length) {

	IICWriteReadAddr(addr);
	unsigned char readcmd = 0xA1 + ((addr / 0x10000) << 1);	 // read cmd

	/* read data */

	// Specify slave address
	USCI_B_I2C_setSlaveAddress(USCI_B0_BASE, readcmd);

	// Set receive mode
	USCI_B_I2C_setMode(USCI_B0_BASE, USCI_B_I2C_RECEIVE_MODE);

	// Enable I2C Module to start operations
	USCI_B_I2C_enable(USCI_B0_BASE);

	// Enable master Receive interrupt
	USCI_B_I2C_enableInterrupt(USCI_B0_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);

	// wait for bus to be free
	while (USCI_B_I2C_isBusBusy(USCI_B0_BASE))
		;

	receiveBufferPointer = ( unsigned char* )dest;
	receiveCount	     = length;

	// Initialize multi reception
	USCI_B_I2C_masterReceiveMultiByteStart(USCI_B0_BASE);

	__no_operation();
}

//******************************************************************************
//
// This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
	void
	USCI_B0_ISR(void) {
	switch (__even_in_range(UCB0IV, 12)) {
	case USCI_I2C_UCTXIFG: {
		// Check TX byte counter
		if (transmitCounter < txlen) {
			// Initiate send of character from Master to Slave
			USCI_B_I2C_masterSendMultiByteNext(USCI_B0_BASE, transmitData[ transmitCounter ]);

			// Increment TX byte counter
			transmitCounter++;
		}
		else {
			// Initiate stop only
			if (NeedStop)
				USCI_B_I2C_masterSendMultiByteStop(USCI_B0_BASE);

			// Clear master interrupt status
			USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
		}
		break;
	}
	case USCI_I2C_UCRXIFG: {
		// Decrement RX byte counter
		receiveCount--;

		if (receiveCount) {
			if (receiveCount == 1) {
				// Initiate end of reception -> Receive byte with NAK
				*receiveBufferPointer++ =
					USCI_B_I2C_masterReceiveMultiByteFinish(USCI_B0_BASE);
			}
			else {
				// Keep receiving one byte at a time
				*receiveBufferPointer++ = USCI_B_I2C_masterReceiveMultiByteNext(USCI_B0_BASE);
			}
		}
		else {
			// Receive last byte
			*receiveBufferPointer = USCI_B_I2C_masterReceiveMultiByteNext(USCI_B0_BASE);
			__bic_SR_register_on_exit(LPM0_bits);
		}
		break;
	}
	}
}