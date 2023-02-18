#include "SlaveManager.h"

//TODO: thread that changes temprature every 5 seconds (+-1)

class Logic {
	SlaveManager *slave_manager;
	int temp;
public:
	Logic() {
		temp=20;
	}

	static void onModChange(uint16_t add, uint16_t val) {
		if (val == 0x0) {
			printf("Cold Mode ON \n");
		} else if (val == 0x1) {
			printf("Warm Mode ON \n");
		} else if (val == 0x2) {
			printf("Auto Mode ON \n");
		}

	}

	static void onPowerChange(uint16_t add, uint16_t val) {
		if (val == 0x0) {
			printf("Power OFF \n");
		} else if (val == 0x1) {
			printf("Power ON \n");
		}
	}

	static void onTemperatureChange(uint16_t add, uint16_t val) {
		printf("Temperature is set to: %d\n", val);
	}



	static void onFanSpeedChange(uint16_t add, uint16_t val) {
		if (val == 0x0) {
			printf("#Fans is set to 1\n");
		} else if (val == 0x1) {
			printf("#Fans is set to 2\n");
		} else if (val == 0x2) {
			printf("#Fans is set to 3\n");
		}else if (val == 0x3) {
			printf("#Fans is set to Auto\n");
		}
	}
	void init(SlaveManager *slave_manager) {
		this->slave_manager = slave_manager;
		this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
				onModChange, 0x07);
		this->slave_manager->setCallBackFunc(FunctionCode::READ_COIL,
				onPowerChange, 0x30);
		this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
				onTemperatureChange, 0x09);
		this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
				onFanSpeedChange, 0x08);

	}
//	void start(){
//
//	}
};
