#include "../GPMS Manager/SlaveManager.h"
#include "../GPMS Manager/handleError.h"

#include <unistd.h>

SlaveManager::SlaveManager() {
	this->coilT_lock = new rw_lock();
	this->discrt_lock = new rw_lock();
	this->holdT_lock = new rw_lock();
	this->regT_lock = new rw_lock();
}
void SlaveManager::init(
		std::unordered_map<FunctionCode, std::vector<uint16_t>> tables_range) {
	for (auto &trange : tables_range) {
		switch (trange.first) {
		case FunctionCode::READ_COIL:
			this->coilsT = new regTables<bool>(trange.second[0],
					trange.second[1]);
			break;
		case FunctionCode::READ_DISCR_INPUT:
			this->discrtT = new regTables<bool>(trange.second[0],
					trange.second[1]);
			break;
		case FunctionCode::READ_INPUT_REGISTER:
			this->regT = new regTables<uint16_t>(trange.second[0],
					trange.second[1]);
			break;
		case FunctionCode::READ_HOLD_REGISTER:
			this->holdT = new regTables<uint16_t>(trange.second[0],
					trange.second[1]);
			break;
		}
	}

}

void SlaveManager::setCallBackFunc(FunctionCode table_type, CallBack handler,
		uint16_t regadd) {
	if (table_type == FunctionCode::READ_HOLD_REGISTER) {
		this->holdT->setCallBackFunc(handler, regadd);

	} else if (table_type == FunctionCode::READ_INPUT_REGISTER) {
		this->regT->setCallBackFunc(handler, regadd);
	} else if (table_type == FunctionCode::READ_COIL) {
		this->coilsT->setCallBackFunc(handler, regadd);
	}
}

std::string SlaveManager::handleMSG(std::string msg) {

	Message *msgB = new Message(msg);
	uint8_t funCode = msgB->getFuncCode();

	std::vector<std::string> bits_read;
	std::vector<std::string> bytes_read;

	bool coil_val;
	std::string response;
	std::string error;

	ModbusError err;
	ModbusError _table_range_err = ModbusError(Modbus::Error::SUCCESS);

	switch (funCode) {
	case Modbus::FunctionCode::READ_COIL:
		std::cout << "READ COIL" << std::endl;

		//checking request
		err = check_read_coil_req(msgB);
		if (!(this->coilsT->addInRange(msgB->getStartAdd()))
				|| !(this->coilsT->addInRange(
						msgB->getStartAdd() + msgB->getVal()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//reading from table
		this->coilT_lock->r_lock();
		this->readbits(msgB, &bits_read, true);
		this->coilT_lock->r_unlock();
		response = msgB->create_read_response(bits_read);
		break;
	case Modbus::FunctionCode::READ_DISCR_INPUT:
		std::cout << "READ_DISCR_INPUT" << std::endl;

		//checking request
		err = check_read_discrt_req(msgB);
		if (!(this->discrtT->addInRange(msgB->getStartAdd()))
				|| !(this->discrtT->addInRange(
						msgB->getStartAdd() + msgB->getVal()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//read from table
		this->discrt_lock->r_lock();
		this->readbits(msgB, &bits_read, false);
		this->discrt_lock->r_unlock();
		response = msgB->create_read_response(bits_read);
		break;

	case Modbus::FunctionCode::READ_HOLD_REGISTER:
		std::cout << "READ_HOLD_REGISTER" << std::endl;

		//check request
		err = check_read_hold_req(msgB);
		if (!(this->holdT->addInRange(msgB->getStartAdd()))
				|| !(this->holdT->addInRange(
						msgB->getStartAdd() + msgB->getVal()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//read from table
		this->holdT_lock->r_lock();
		this->readbytes(msgB, &bytes_read, false);
		this->holdT_lock->r_unlock();
		response = msgB->create_read_response(bytes_read);
		break;

	case Modbus::FunctionCode::READ_INPUT_REGISTER:
		std::cout << "READ_INPUT_REGISTER" << std::endl;

		//check request
		err = check_read_inputReg_req(msgB);
		if (!(this->regT->addInRange(msgB->getStartAdd()))
				|| !(this->regT->addInRange(
						msgB->getStartAdd() + msgB->getVal()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//read from table
		this->regT_lock->r_lock();
		this->readbytes(msgB, &bytes_read, true);
		this->regT_lock->r_unlock();
		response = msgB->create_read_response(bytes_read);
		break;

	case Modbus::FunctionCode::WRITE_COIL:  // 0x05

		std::cout << "WRITE_COIL" << std::endl;

		//checking request
		err = check_write_coil_req(msgB);
		if (!(this->coilsT->addInRange(msgB->getStartAdd()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//writing to table
		this->coilT_lock->w_lock();
		coil_val = (msgB->getVal()== 0xFF00)? true : false;
		this->coilsT->writeToReg(msgB->getStartAdd(), coil_val);
		this->coilT_lock->w_unlock();
		response = msg;
		break;

	case Modbus::FunctionCode::WRITE_HOLD_REGISTER:  // 0x06
		std::cout << "WRITE_HOLD_REGISTER" << std::endl;
		err = check_write_hold_req(msgB);
		if (!(this->holdT->addInRange(msgB->getStartAdd()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;
		this->holdT_lock->w_lock();
		this->holdT->writeToReg(msgB->getStartAdd(), msgB->getVal());
		this->holdT_lock->w_unlock();
		response = msg;
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
		std::cout << "WRITE_MULT_COILS" << std::endl;

		//checking request
		err = check_multi_coils_req(msgB);
		if (!(this->coilsT->addInRange(msgB->getStartAdd()))
				|| !(this->coilsT->addInRange(
						msgB->getStartAdd() + msgB->getVal()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//writing to table
		this->coilT_lock->w_lock();
//		sleep(30);
		this->writeBits(msgB, true);
		this->coilT_lock->w_unlock();
		response = msg;
		break;

	case Modbus::FunctionCode::WRITE_MULT_REGISTERS: //0x10
		std::cout << "WRITE_MULT_REGISTERS" << std::endl;

		//checking request
		err = check_write_multiReg_req(msgB);
		if (!(this->regT->addInRange(msgB->getStartAdd()))
				|| !(this->regT->addInRange(
						msgB->getStartAdd() + (0x10 * msgB->getVal())))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;
		//writing to table
		this->regT_lock->w_lock();
		this->writeBytes(msgB, true);
		this->regT_lock->w_unlock();
		response= msg;
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

		//checking request
		err = check_mask_writeReg_req(msgB,
				((uint16_t) (msgB->getBytesCount() << 8)
						| msgB->getfirstelementofvector()));

		if (!(this->holdT->addInRange(msgB->getStartAdd()))) {
			_table_range_err = ModbusError(Modbus::Error::ILLEGAL_DATA_ADDRESS);
		}
		if ((err != ModbusError(Modbus::Error::SUCCESS)
				|| (_table_range_err != ModbusError(Modbus::Error::SUCCESS))))
			break;

		//writing to table
		this->holdT_lock->w_lock();
		this->holdT->writeToReg(msgB->getStartAdd(), this->maskReg(msgB));
		this->holdT_lock->w_unlock();
		response=msg;
		break;

	case Modbus::FunctionCode::R_W_MULT_REGISTERS:
		std::cout << "TBD: R_W_MULT_REGISTERS" << std::endl;
		break;
	case Modbus::FunctionCode::READ_FIFO_QUEUE:
		std::cout << "TBD: READ_FIFO_QUEUE" << std::endl;
		break;

	default:
		err = ModbusError(Modbus::Error::ILLEGAL_FUNCTION);
		error.append(std::to_string(funCode+0x80) + ' ' + std::to_string(err));
		return error;
		//const char* str = (const char*)err;

	}
	if (static_cast<int>(err) > static_cast<int>(_table_range_err)){
		error.append(std::to_string(funCode+0x80) + ' ' + std::to_string(err));
		return error;
	}
	if(_table_range_err==ModbusError(Modbus::Error::SUCCESS)) return response;
	error.append(std::to_string(funCode+0x80) + ' ' + std::to_string(_table_range_err));
	return error;

}

void SlaveManager::setTempreture(uint16_t reg, uint16_t tmp) {
	this->regT_lock->w_lock();
	this->regT->writeToReg(reg, tmp);
	this->regT_lock->w_unlock();
}

