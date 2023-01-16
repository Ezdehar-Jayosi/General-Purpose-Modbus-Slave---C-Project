#include <stdio.h>
#include<string>
#include <iostream>
#include "regTables.cpp"
#include "utilities.h"
#include "handleError.cpp"
class SlaveManager {
	// Registers Tables

	regTables<uint16_t> *regT;
	regTables<uint16_t> *holdT;
	regTables<bool> *coilsT;
	regTables<bool> *discrtT;

	//call back functions maps
    std::unordered_map<uint16_t, bool (*)(uint16_t)> cbf_map_coil;
    std::unordered_map<uint16_t, bool (*)(uint16_t)> cbf_map_discrt;
    std::unordered_map<uint16_t, uint16_t (*)(uint16_t)> cbf_map_regT;
    std::unordered_map<uint16_t, uint16_t (*)(uint16_t)> cbf_map_holdT;


	// helper messages
	void writeBits(Message *msg,bool tFlag){
		std::cout<< "Writebits func" << std::endl;
		std::vector<bool> bits;
		// convert byte to bits
		for(uint8_t o : msg->getOutPut()){
			//convert uint8_t to binary
			std::cout << +o << " ";
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
		int i = 0, j=0;
		uint16_t add= msg->getStartAdd();
		regTables<uint16_t>* table = (tFlag == true) ? regT : holdT;
		while(i<(int) quantity_of_reg){
			uint16_t val_to_write = ((uint16_t) (msg->getOutPut()[j] << 8) | msg->getOutPut()[j+1]);
			table->writeToReg(add, val_to_write );
			i+=1;
			j+=2;
			add += 0x10; // TODO: check if we should add 0x01 or 0x10
			//std::cout  << +quantity_of_reg << ' ' << +add << std::endl;
		}

	}
	uint16_t maskReg(Message *msg){
		std::cout << "mask reg" << std::endl;
		uint16_t calc_res;
		uint16_t Not_And_Mask= msg->getVal() ^ 0xFF;
//		std::cout << "msg->getVal() = " << +msg->getVal() << std::endl;
//		std::cout << "Not_And_Mask" << +Not_And_Mask<< std::endl;

		uint16_t content_and_mask;
		uint16_t Or_Mask = ((uint16_t) (msg->getBytesCount() << 8) | msg->getfirstelementofvector());
//		std::cout << "Or_Mask"  << +Or_Mask<< std::endl;
//		std::cout << "msg->getStartAdd() " << +msg->getStartAdd() << std::endl;
		content_and_mask = msg->getStartAdd() & msg->getVal();

		calc_res = ((content_and_mask & msg->getVal()) | (Or_Mask & Not_And_Mask));
//		std::cout << "calc_res = " << +calc_res <<std::endl;
		return calc_res;
	}

	void readbits(Message* msg, std::vector<bool>* reg_vals, bool tFlag){
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add= msg->getStartAdd();
		regTables<bool>* table = (tFlag == true) ? coilsT : discrtT;
		while(i<(int) quantity_of_reg){
			reg_vals->push_back(table->readReg(add).first);
			i+=1;
			add +=0x1;
		}
	}

	void readbytes(Message* msg, std::vector<uint16_t>* reg_vals ,bool tFlag){
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add= msg->getStartAdd();
		regTables<uint16_t>* table = (tFlag == true) ? regT : holdT;
		while(i<(int) quantity_of_reg){
			reg_vals->push_back(table->readReg(add).first);
			i+=1;
			add +=0x1; // TODO: check if we should add 0x01 or 0x10
		}
	}


public:
	SlaveManager() {
		this->coilsT = new regTables<bool>;
		this->discrtT = new regTables<bool>;
		this->holdT = new regTables<uint16_t>;
		this->regT = new regTables<uint16_t>;

	}

	// set call back functions
	void setCallBackFunc_16(tables table_type,uint16_t (*CBF)(uint16_t), uint16_t regadd){
		if(table_type== tables::Holding){
			this->cbf_map_holdT.insert(std::pair<uint16_t,uint16_t (*)(uint16_t)>(regadd,CBF));

		}else if(table_type == tables::Register){
			this->cbf_map_regT.insert(std::pair<uint16_t,uint16_t (*)(uint16_t)>(regadd,CBF));
		}
	}
	void setCallBackFunc_b(tables table_type,bool (*CBF)(uint16_t), uint16_t regadd){
		if(table_type== tables::Coil){
			this->cbf_map_coil.insert(std::pair<uint16_t,bool (*)(uint16_t)>(regadd,CBF));

		}else if(table_type == tables::Discrete){
			this->cbf_map_discrt.insert(std::pair<uint16_t,bool (*)(uint16_t)>(regadd,CBF));
		}
	}



	ModbusError handleMSG(std::string msg) { //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
									  // TODO: change argument type, argument is of type ByteArray

		Message *msgB = new Message(msg);

		uint8_t funCode = msgB->getFuncCode();
		std::pair<uint, bool> res;
		std::vector<bool> bits_read;
		std::vector<uint16_t> bytes_read;

		ModbusError err;

		switch(funCode){
		case Modbus::FunctionCode::READ_COIL:
			std::cout << "READ COIL" << std::endl;
			err = check_read_coil_req(msgB);
			this->readbits(msgB,&bits_read,true);
			break;
		case Modbus::FunctionCode::READ_DISCR_INPUT:
			std::cout << "READ_DISCR_INPUT" << std::endl;
			err =check_read_discrt_req(msgB);
			this->readbits(msgB,&bits_read,false);
			break;
		case Modbus::FunctionCode::READ_HOLD_REGISTER:
			std::cout << "READ_HOLD_REGISTER" << std::endl;
			err = check_read_hold_req(msgB);
			this->readbytes(msgB,&bytes_read,false);
			break;
		case Modbus::FunctionCode::READ_INPUT_REGISTER:
			std::cout << "READ_INPUT_REGISTER" << std::endl;
			err = check_read_inputReg_req(msgB);
			this->readbytes(msgB,&bytes_read,true);
			break;
		case Modbus::FunctionCode::WRITE_COIL:  // 0x05
			std::cout << "Writing coil" << std::endl;
			check_write_coil_req(msgB);
			this->coilsT->writeToReg(msgB->getStartAdd(),msgB->getVal());
			break;
		case Modbus::FunctionCode::WRITE_HOLD_REGISTER:  // 0x06
			std::cout << "Write hold" << std::endl;
			check_write_hold_req(msgB);
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
			err = check_multi_coils_req(msgB);
			this->writeBits(msgB,true);
			break;
		case Modbus::FunctionCode::WRITE_MULT_REGISTERS: //0x10
			std::cout << "WRITE_MULT_REGISTERS" << std::endl;
			check_write_multiReg_req(msgB);
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
		case Modbus::FunctionCode::MASK_WRITE_REGISTER: //0x16
			std::cout << "MASK_WRITE_REGISTER" << std::endl;
			uint16_t or_mask = ((uint16_t) (msgB->getBytesCount() << 8) | msgB->getfirstelementofvector());
			check_mask_writeReg_req(msgB,or_mask);
			this->holdT->writeToReg(msgB->getStartAdd(),this->maskReg(msgB));
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

//	void setRegister(int regAdd, int regVal, int func_code){
//
//	}
//
//	void setCallBackFunction(int regAdd, void (*regFunc)(int regAdd,int regVal) ){
//
//	}
};
