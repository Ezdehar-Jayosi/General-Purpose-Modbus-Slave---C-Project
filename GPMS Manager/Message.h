/*
 * Message.h
 *
 *  Created on: 3 Jan 2023
 *      Author: ezdehar
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "../Utilities/ModbusTypeDefs.h"
#include <string>
#include <iostream>
#include <cstddef>
#include <cstring>
#include <vector>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <bitset>
#include "../Utilities/utilities.h"
class Message {
	uint8_t uid;
	uint8_t function_code;
	uint16_t starting_add;
	uint16_t val;
	uint8_t byte_count;
	std::vector<uint8_t> output_val;
//	std::vector<uint8_t> bytes_vec;

	static void tokenize(std::string const &str, const char delim,
			std::vector<std::string> &out) {
		std::stringstream ss(str);
		std::string s;
		while (std::getline(ss, s, delim)) {
			out.push_back(s);
		}
	}

public:
	Message(std::string msg);
	virtual ~Message();

	std::string create_read_response(std::vector<std::string> registers_vals);
	uint8_t getFuncCode() {
		return this->function_code;
	}
	uint16_t getStartAdd() {
		return this->starting_add;
	}
	uint16_t getVal() {
		return this->val;
	}
	std::vector<uint8_t> getOutPut() {
		return this->output_val;
	}
	uint8_t getBytesCount(){
		return byte_count;
	}
	uint8_t getfirstelementofvector(){
		return output_val[0];
	}

};

#endif /* MESSAGE_H_ */
