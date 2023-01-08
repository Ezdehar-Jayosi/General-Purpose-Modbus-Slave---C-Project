#include <stdio.h>
#include<string>
#include <iostream>
#include "regTables.cpp"
#include "Message.h"
class SlaveManager {
	// Registers Tables
	regTables<uint16_t> *regT;
	regTables<uint16_t> *holdT;
	regTables<bool> *coilsT;
	regTables<bool> *discrtT;

	// helper messages
	void writeBits(Message *msg,bool tFlag){
		std::cout<< "Writebits func" << std::endl;
		std::vector<bool> bits;
		// convert byte to bits
		for(uint8_t o : msg->getOutPut()){
			//convert uint8_t to binary
			uint8_t tmp_v = o;
			while(tmp_v!=0x0){
				if((tmp_v&0x1) == 1 ){
					bits.push_back(true);
				}else{
					bits.push_back(false);
				}
				tmp_v = tmp_v >>1;
			}
		}
		bits.push_back(false);

		//write to registers
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add= msg->getStartAdd();
		regTables<bool>* table = (tFlag == true) ? coilsT : discrtT;
		while(i<(int) quantity_of_reg){
			table->writeToReg(add, bits[i]);
			i+=1;
			add += 0x1;
//			std::cout  << +quantity_of_reg << ' ' << +add << std::endl;
		}

	}

	void writeBytes(Message *msg, bool tFlag){
		//write to registers
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add= msg->getStartAdd();
		regTables<uint16_t>* table = (tFlag == true) ? regT : holdT;
		while(i<(int) quantity_of_reg){
			table->writeToReg(add, msg->getOutPut()[i]);
			i+=1;
			add += 0x10;
			//std::cout  << +quantity_of_reg << ' ' << +add << std::endl;
		}

	}

public:
	SlaveManager() {
		this->coilsT = new regTables<bool>;
		this->discrtT = new regTables<bool>;
		this->holdT = new regTables<uint16_t>;
		this->regT = new regTables<uint16_t>;

	}

	const char* handleMSG(std::string msg) { //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
									  // TODO: change argument type, argument is of type ByteArray
		std::cout << "SLAVEMANAGER PRINTING: ";
		std::cout << msg << std::endl;

		Message *msgB = new Message(msg);

		uint8_t funCode = msgB->getFuncCode();
		std::pair<void, bool> res;
		switch(funCode){
		case Modbus::FunctionCode::READ_COIL:
			std::cout << "READ COIL" << std::endl;
			res = this->coilsT->readReg(msgB->getStartAdd());
			break;
		case Modbus::FunctionCode::READ_DISCR_INPUT:
			std::cout << "READ_DISCR_INPUT" << std::endl;
			res = this->discrtT->readReg(msgB->getStartAdd());
			break;
		case Modbus::FunctionCode::READ_HOLD_REGISTER:
			std::cout << "READ_HOLD_REGISTER" << std::endl;
			res = this->holdT->readReg(msgB->getStartAdd());
			break;
		case Modbus::FunctionCode::READ_INPUT_REGISTER:
			std::cout << "READ_INPUT_REGISTER" << std::endl;
			res = this->regT->readReg(msgB->getStartAdd());
			break;
		case Modbus::FunctionCode::WRITE_COIL:  // 0x05
			std::cout << "Writing coil" << std::endl;
			this->coilsT->writeToReg(msgB->getStartAdd(),msgB->getVal());
			break;
		case Modbus::FunctionCode::WRITE_HOLD_REGISTER:  // 0x06
			std::cout << "Write hold" << std::endl;
			this->holdT->writeToReg(msgB->getStartAdd(),msgB->getVal());
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
			std::cout << "Write Mult Coils" << std::endl;
			this->writeBits(msgB,true);
			break;
		case Modbus::FunctionCode::WRITE_MULT_REGISTERS: //0x10
			std::cout << "WRITE_MULT_REGISTERS" << std::endl;
			this->writeBytes(msgB,true);
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
//		default:
//			return ModbusError::getText(Modbus::Error::ILLEGAL_FUNCTION);
		}
//		return ModbusError::getText(Modbus::Error::SUCCESS);


	}

//	void setRegister(int regAdd, int regVal, int func_code){
//
//	}
//
//	void setCallBackFunction(int regAdd, void (*regFunc)(int regAdd,int regVal) ){
//
//	}
};
