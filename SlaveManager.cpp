#include <stdio.h>
#include<string>
#include <iostream>
#include "regTables.cpp"
#include "Message.h"
class SlaveManager {
	// Registers Tables
	regTables<uint> *regT;
	regTables<uint> *holdT;
	regTables<bool> *coilsT;
	regTables<bool> *discrtT;

public:
	SlaveManager() {
		this->coilsT = new regTables<bool>;
		this->discrtT = new regTables<bool>;
		this->holdT = new regTables<uint>;
		this->regT = new regTables<uint>;

	}

	void handleMSG(std::string msg) { //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
									  // TODO: change argument type, argument is of type ByteArray
		std::cout << "SLAVEMANAGER PRINTING: ";
		std::cout << msg << std::endl;

		Message *msgB = new Message(msg);

		uint8_t funCode = msgB->getFuncCode();
		switch(funCode){
		case Modbus::FunctionCode::READ_COIL:
			std::cout << "TBD: READ COIL" << std::endl;
			break;
		case Modbus::FunctionCode::READ_DISCR_INPUT:
			std::cout << "TBD: READ_DISCR_INPUT" << std::endl;
			break;
		case Modbus::FunctionCode::READ_HOLD_REGISTER:
			std::cout << "TBD: READ_HOLD_REGISTER" << std::endl;
			break;
		case Modbus::FunctionCode::READ_INPUT_REGISTER:
			std::cout << "TBD: READ_INPUT_REGISTER" << std::endl;
			break;
		case Modbus::FunctionCode::WRITE_COIL:  // 0x05
			std::cout << "TBD: WRITE_COIL" << std::endl;
			break;
		case Modbus::FunctionCode::WRITE_HOLD_REGISTER:  // 0x06
			std::cout << "TBD: WRITE_HOLD_REGISTER" << std::endl;
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
		case Modbus::FunctionCode::WRITE_MULT_COILS : //0x0F
			std::cout << "TBD: WRITE_MULT_COILS" << std::endl;
			break;
		case Modbus::FunctionCode::WRITE_MULT_REGISTERS: //0x10
			std::cout << "TBD: WRITE_MULT_REGISTERS" << std::endl;
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
		case Modbus::FunctionCode::MASK_WRITE_REGISTER:
			std::cout << "TBD: MASK_WRITE_REGISTER" << std::endl;
			break;
		case Modbus::FunctionCode::R_W_MULT_REGISTERS:
			std::cout << "TBD: R_W_MULT_REGISTERS" << std::endl;
			break;
		case Modbus::FunctionCode::READ_FIFO_QUEUE:
			std::cout << "TBD: READ_FIFO_QUEUE" << std::endl;
			break;
		}

	}

//	void setRegister(int regAdd, int regVal, int func_code){
//
//	}
//
//	void setCallBackFunction(int regAdd, void (*regFunc)(int regAdd,int regVal) ){
//
//	}
};
