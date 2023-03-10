#include "ModbusError.h"
#include <cmath>
ModbusError check_multi_coils_req(Message *msg) {
	uint8_t rem = msg->getVal() % 8;
	uint8_t byte_c =
			(rem > 0) ? (msg->getVal() >> 3) + 1 : (msg->getVal() >> 3);
	if ((msg->getVal() < 0x0001 || msg->getVal() > 0x07B0)
			|| (msg->getBytesCount() != byte_c)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if ((msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF)
			|| ((msg->getStartAdd() + msg->getVal()) < 0x0000
					|| (msg->getStartAdd() + msg->getVal()) > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_write_coil_req(Message *msg) {
	if (msg->getVal() != 0x0000 && msg->getVal() != 0xFF00) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if (msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF) {

		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_write_hold_req(Message *msg) {
	if (msg->getVal() < 0x0000 || msg->getVal() > 0xFFFF) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if (msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF) {

		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_write_multiReg_req(Message *msg) {
	uint8_t rem = msg->getVal() % 2;
	uint8_t byte_c =
			(rem > 0) ? (msg->getVal() >> 3) + 1 : (msg->getVal() >> 3);
	if ((msg->getVal() < 0x0001 || msg->getVal() > 0x07B0)
			|| (msg->getBytesCount() != byte_c)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if ((msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF)
			|| ((msg->getStartAdd() + (0x10 * msg->getVal())) < 0x0000
					|| (msg->getStartAdd() + (0x10 * msg->getVal())) > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_mask_writeReg_req(Message *msg, uint16_t or_mask) {
	if (msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	} else if ((msg->getVal() < 0x0000 || msg->getVal() > 0xFFFF)
			|| (or_mask < 0x0000 || or_mask > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_read_coil_req(Message *msg) {
	if ((msg->getVal() < 0x0001 || msg->getVal() > 0x07B0)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if ((msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF)
			|| ((msg->getStartAdd() + msg->getVal()) < 0x0000
					|| (msg->getStartAdd() + msg->getVal()) > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
	}
	return ModbusError(Modbus::Error::SUCCESS);

}

ModbusError check_read_discrt_req(Message *msg) {
	if ((msg->getVal() < 0x0001 || msg->getVal() > 0x07D0)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if ((msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF)
			|| ((msg->getStartAdd() + msg->getVal()) < 0x0000
					|| (msg->getStartAdd() + msg->getVal()) > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);

	}
	return ModbusError(Modbus::Error::SUCCESS);
}

ModbusError check_read_hold_req(Message *msg) {
	if ((msg->getVal() < 0x0001 || msg->getVal() > 0x007D)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_VALUE);
	} else if ((msg->getStartAdd() < 0x0000 || msg->getStartAdd() > 0xFFFF)
			|| ((msg->getStartAdd() + msg->getVal()) < 0x0000
					|| (msg->getStartAdd() + msg->getVal()) > 0xFFFF)) {
		return ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);

	}
	return ModbusError(Modbus::Error::SUCCESS);

}

const auto &check_read_inputReg_req = check_read_hold_req;

