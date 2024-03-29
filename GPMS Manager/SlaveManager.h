/*
 * SlaveManager.h
 *
 *  Created on: 5 Feb 2023
 *      Author: ezdehar
 */

#ifndef SLAVEMANAGER_H_
#define SLAVEMANAGER_H_

#include <stdio.h>
#include<string>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "../Utilities/ModbusError.h"
#include <mutex>
#include "../GPMS Manager/Message.h"
#include "../GPMS Manager/regTables.cpp"
#include "../GPMS Manager/rw_lock.cpp"

class SlaveManager {

	// Registers Tables
	regTables<uint16_t> *regT;
	regTables<uint16_t> *holdT;
	regTables<bool> *coilsT;
	regTables<bool> *discrtT;

//// mutexes
	rw_lock *regT_lock, *holdT_lock, *coilT_lock, *discrt_lock;

	// helper messages

	void writeBits(Message *msg, bool tFlag) {
		//std::cout << "Writebits func" << std::endl;
		std::vector<bool> bits;
		// convert byte to bits
		for (uint8_t o : msg->getOutPut()) {
			//convert uint8_t to binary
			//std::cout << +o << " ";
			uint8_t tmp_v = o;
			while (tmp_v != 0x0) {
				if ((tmp_v & 0x1) == 1) {

					bits.push_back(true);
				} else {
					bits.push_back(false);
				}
				tmp_v = tmp_v >> 1;
			}
		}
		bits.push_back(false);

		//write to registers
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;

		uint16_t add = msg->getStartAdd();
		regTables<bool> *table = (tFlag == true) ? coilsT : discrtT;
		while (i < (int) quantity_of_reg) {
			table->writeToReg(add, bits[i]);
			i += 1;
			add += 0x1;
//			std::cout  << +quantity_of_reg << ' ' << +add << std::endl;
		}

	}

	void writeBytes(Message *msg, bool tFlag) {
		//write to registers
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0, j = 0;
		uint16_t add = msg->getStartAdd();
		regTables<uint16_t> *table = (tFlag == true) ? regT : holdT;
		while (i < (int) quantity_of_reg) {
			uint16_t val_to_write = ((uint16_t) (msg->getOutPut()[j] << 8)
					| msg->getOutPut()[j + 1]);
			table->writeToReg(add, val_to_write);
			i += 1;
			j += 2;
			add += 0x1;
			//std::cout  << +quantity_of_reg << ' ' << +add << std::endl;
		}

	}
	uint16_t maskReg(Message *msg) {
		//std::cout << "mask reg" << std::endl;
		uint16_t calc_res;
		uint16_t Not_And_Mask = msg->getVal() ^ 0xFF;
//		std::cout << "msg->getVal() = " << +msg->getVal() << std::endl;
//		std::cout << "Not_And_Mask" << +Not_And_Mask<< std::endl;

		uint16_t content_and_mask;
		uint16_t Or_Mask = ((uint16_t) (msg->getBytesCount() << 8)
				| msg->getfirstelementofvector());
//		std::cout << "Or_Mask"  << +Or_Mask<< std::endl;
//		std::cout << "msg->getStartAdd() " << +msg->getStartAdd() << std::endl;
		content_and_mask = msg->getStartAdd() & msg->getVal();

		calc_res = ((content_and_mask & msg->getVal())
				| (Or_Mask & Not_And_Mask));
//		std::cout << "calc_res = " << +calc_res <<std::endl;
		return calc_res;
	}

	void readbits(Message *msg, std::vector<std::string> *reg_vals, bool tFlag) {
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add = msg->getStartAdd();
		regTables<bool> *table = (tFlag == true) ? coilsT : discrtT;
		uint8_t n = 0x00;
		int j=0;
		while (i < (int) quantity_of_reg) {
			//reg_vals->push_back(table->readReg(add));
			if(j==8){
				reg_vals->push_back(std::to_string(n));
				//std::cout << " bits things : " << std::to_string(n)<< std::endl;
				j=0;
				n = 0x00;
			}
			if(table->readReg(add)==true){
				n = n>>1;
				n = n | 0x80;
			}else{
				n = n>>1;
			}

			j++;
			i += 1;
			add += 0x1;
		}
		if(j>0){
			reg_vals->push_back(std::to_string(n>>(8-j)));
			//std::cout << " bits things : " << std::to_string((n>>(8-j)))<< std::endl;
		}
	}

	void readbytes(Message *msg, std::vector<std::string> *reg_vals, bool tFlag) {
		uint8_t quantity_of_reg = msg->getVal();
		int i = 0;
		uint16_t add = msg->getStartAdd();
		regTables<uint16_t> *table = (tFlag == true) ? regT : holdT;
		while (i < (int) quantity_of_reg) {
			reg_vals->push_back(std::to_string(table->readReg(add)));
			i += 1;
			add += 0x1;
		}
	}


public:
	SlaveManager();
	void init(std::unordered_map<FunctionCode, std::vector<uint16_t>> tables_range);
	// set call back functions
	void setCallBackFunc(FunctionCode table_type, CallBack handler,
			uint16_t regadd);

	std::string handleMSG(std::string msg);

	void setTempreture(uint16_t reg,uint16_t tmp);

};

#endif /* SLAVEMANAGER_H_ */
