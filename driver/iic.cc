#include "iic.h"
#include "driverlib.h"

uint8_t		  transmitCounter = 0;
int		  TXLENGTH	  = 0;
vector< uint8_t > transmitData;

void IICInit() {

	// Assign I2C pins to USCI_B0
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN1 + GPIO_PIN2);

	// Initialize Master
	USCI_B_I2C_initMasterParam param = { 0 };
	param.selectClockSource		 = USCI_B_I2C_CLOCKSOURCE_SMCLK;
	param.i2cClk			 = UCS_getSMCLK();
	param.dataRate			 = USCI_B_I2C_SET_DATA_RATE_400KBPS;
	USCI_B_I2C_initMaster(USCI_B0_BASE, &param);
}

void IICWriteBytes(uint8_t slaveaddr, vector< uint8_t >& data) {

	transmitData = data;
	TXLENGTH     = data.size();

	// Specify slave address
	USCI_B_I2C_setSlaveAddress(USCI_B0_BASE, slaveaddr);

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

	// Delay until transmission completes
	while (USCI_B_I2C_isBusBusy(USCI_B0_BASE))
		;
}

vector< uint8_t > IICReadBytes(uint8_t slaveaddr) {
	return vector< uint8_t >();
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
		if (transmitCounter < TXLENGTH) {
			// Initiate send of character from Master to Slave
			USCI_B_I2C_masterSendMultiByteNext(USCI_B0_BASE, transmitData[ transmitCounter ]);

			// Increment TX byte counter
			transmitCounter++;
		}
		else {
			// Initiate stop only
			USCI_B_I2C_masterSendMultiByteStop(USCI_B0_BASE);

			// Clear master interrupt status
			USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
		}
		break;
	}
	}
}