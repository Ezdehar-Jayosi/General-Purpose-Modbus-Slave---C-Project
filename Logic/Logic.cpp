#include "Logic.h"

Logic::Logic() {
	temp = 20;
}

void Logic::setTempreture(uint16_t add) {

	while (true) {
		int sign = rand() % 2 + 1;
		if (sign == 1) {
			this->temp -= 1;
		} else {
			this->temp += 1;
		}
		//std::cout << " sign = " << sign << " temp = " << this->temp << std::endl;
		slave_manager->setTempreture(add, this->temp);
		sleep(5);
	}

}
void Logic::onModChange(uint16_t add, uint16_t val) {
	if (val == 0x0) {
		printf("Cold Mode ON \n");
	} else if (val == 0x1) {
		printf("Warm Mode ON \n");
	} else if (val == 0x2) {
		printf("Auto Mode ON \n");
	}

}

void Logic::onPowerChange(uint16_t add, uint16_t val) {
	if (val == 0x0) {
		printf("Power OFF \n");
	} else if (val == 0x1) {
		printf("Power ON \n");
	}
}

void Logic::onTemperatureChange(uint16_t add, uint16_t val) {
	printf("Temperature is set to: %d\n", val);
}

void Logic::onFanSpeedChange(uint16_t add, uint16_t val) {
	if (val == 0x0) {
		printf("#Fans is set to 1\n");
	} else if (val == 0x1) {
		printf("#Fans is set to 2\n");
	} else if (val == 0x2) {
		printf("#Fans is set to 3\n");
	} else if (val == 0x3) {
		printf("#Fans is set to Auto\n");
	}
}
void Logic::init(SlaveManager *slave_manager) {
	this->slave_manager = slave_manager;
	this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
			onModChange, 0x07);
	this->slave_manager->setCallBackFunc(FunctionCode::READ_COIL, onPowerChange,
			0x30);
	this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
			onTemperatureChange, 0x09);
	this->slave_manager->setCallBackFunc(FunctionCode::READ_HOLD_REGISTER,
			onFanSpeedChange, 0x08);
	tmp_thrd = new std::thread([=] {
		setTempreture(0x09);
	});

}

