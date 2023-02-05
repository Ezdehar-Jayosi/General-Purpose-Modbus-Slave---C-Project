#include "SlaveManager.h"

#include "handleError.h"

SlaveManager::SlaveManager() {
	this->coilsT = new regTables<bool>;
	this->discrtT = new regTables<bool>;
	this->holdT = new regTables<uint16_t>;
	this->regT = new regTables<uint16_t>;

}
void SlaveManager::setCallBackFunc(FunctionCode table_type, CallBack handler, uint16_t regadd) {
	if (table_type == FunctionCode::READ_HOLD_REGISTER) {
		this->cbf_map_holdT.insert(
				std::pair<uint16_t, CallBack>(regadd, handler));

	} else if (table_type == FunctionCode::READ_INPUT_REGISTER) {
		this->cbf_map_regT.insert(
				std::pair<uint16_t, CallBack>(regadd, handler));
	} else if (table_type == FunctionCode::READ_COIL) {
		this->cbf_map_coil.insert(
				std::pair<uint16_t, CallBack>(regadd, handler));

	} else if (table_type == FunctionCode::READ_DISCR_INPUT) {
		this->cbf_map_discrt.insert(
				std::pair<uint16_t, CallBack>(regadd, handler));
	}
}

ModbusError SlaveManager::handleMSG(std::string msg) { //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
	// TODO: change argument type, argument is of type ByteArray

	Message *msgB = new Message(msg);

	uint8_t funCode = msgB->getFuncCode();
	std::pair<uint, bool> res;
	std::vector<bool> bits_read;
	std::vector<uint16_t> bytes_read;

	ModbusError err;

	switch (funCode) {
	case Modbus::FunctionCode::READ_COIL:
		std::cout << "READ COIL" << std::endl;
		err = check_read_coil_req(msgB);
		this->readbits(msgB, &bits_read, true);
		break;
	case Modbus::FunctionCode::READ_DISCR_INPUT:
		std::cout << "READ_DISCR_INPUT" << std::endl;
		err = check_read_discrt_req(msgB);
		this->readbits(msgB, &bits_read, false);
		break;
	case Modbus::FunctionCode::READ_HOLD_REGISTER:
		std::cout << "READ_HOLD_REGISTER" << std::endl;
		err = check_read_hold_req(msgB);
		this->readbytes(msgB, &bytes_read, false);
		break;
	case Modbus::FunctionCode::READ_INPUT_REGISTER:
		std::cout << "READ_INPUT_REGISTER" << std::endl;
		err = check_read_inputReg_req(msgB);
		this->readbytes(msgB, &bytes_read, true);
		break;
	case Modbus::FunctionCode::WRITE_COIL:  // 0x05
		std::cout << "Writing coil" << std::endl;
		check_write_coil_req(msgB);
		this->coilsT->writeToReg(msgB->getStartAdd(), msgB->getVal());
		break;
	case Modbus::FunctionCode::WRITE_HOLD_REGISTER:  // 0x06
		std::cout << "Write hold" << std::endl;
		check_write_hold_req(msgB);
		this->holdT->writeToReg(msgB->getStartAdd(), msgB->getVal());
		break;
	case Modbus::FunctionCode::READ_EXCEPTION_SERIAL:
		std::cout << "TBD: READ_EXCEPTION_SERIAL" << std::endl;
		break;
	case Modbus::FunctionCode::DIAGNOSTICS_SERIAL:
		std::cout << "TBD: DIAGNOSTICS_SERIAL" << std::endl;
		break;
	case Modbus::FunctionCode::READ_COMM_CNT_SERIAL:
		std::cout << "TBD: READ_COMM_CNT_SERIAL" << std::endl;
		break;
	case Modbus::FunctionCode::READ_COMM_LOG_SERIAL:
		std::cout << "TBD: READ_COMM_LOG_SERIAL" << std::endl;
		break;
	case Modbus::FunctionCode::WRITE_MULT_COILS: //0x0F
		std::cout << "Write Mult Coils" << std::endl;
		err = check_multi_coils_req(msgB);
		this->writeBits(msgB, true);
		break;
	case Modbus::FunctionCode::WRITE_MULT_REGISTERS: //0x10
		std::cout << "WRITE_MULT_REGISTERS" << std::endl;
		check_write_multiReg_req(msgB);
		this->writeBytes(msgB, true);
		break;
	case Modbus::FunctionCode::REPORT_SERVER_ID_SERIAL:
		std::cout << "TBD: REPORT_SERVER_ID_SERIAL" << std::endl;
		break;
	case Modbus::FunctionCode::READ_FILE_RECORD:
		std::cout << "TBD: READ_FILE_RECORD" << std::endl;
		break;
	case Modbus::FunctionCode::WRITE_FILE_RECORD:
		std::cout << "TBD: WRITE_FILE_RECORD" << std::endl;
		break;
	case Modbus::FunctionCode::MASK_WRITE_REGISTER: //0x16
		std::cout << "MASK_WRITE_REGISTER" << std::endl;
//			uint16_t  or_mask = ((uint16_t) (msgB->getBytesCount() << 8)
//					| msgB->getfirstelementofvector());
		check_mask_writeReg_req(msgB, ((uint16_t) (msgB->getBytesCount() << 8)
				| msgB->getfirstelementofvector()));
		this->holdT->writeToReg(msgB->getStartAdd(), this->maskReg(msgB));
		break;
	case Modbus::FunctionCode::R_W_MULT_REGISTERS:
		std::cout << "TBD: R_W_MULT_REGISTERS" << std::endl;
		break;
	case Modbus::FunctionCode::READ_FIFO_QUEUE:
		std::cout << "TBD: READ_FIFO_QUEUE" << std::endl;
		break;

	default:
		err = ModbusError(Modbus::Error::ILLEGAL_FUNCTION);
		return err;
		//const char* str = (const char*)err;

	}

	return err;

}


