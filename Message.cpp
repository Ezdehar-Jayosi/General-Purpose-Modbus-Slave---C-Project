/*
 * Message.cpp
 *
 *  Created on: 3 Jan 2023
 *      Author: ezdehar
 */

#include "Message.h"

Message::Message(std::string msg) {
	// TODO: deal with the message
	const char delim = ' ';
	std::vector<std::string> out;
	Message::tokenize(msg, delim, out);

	std::vector<uint8_t> bytes_arr;
	for (std::string &o : out) {
		uint8_t idx0 = (uint8_t) o[0];
		uint8_t idx1 = (uint8_t) o[1];
//		uint8_t t = (uint8_t) (hashmap[idx0] << 4);
//		std::cout << "idx0 " << +idx0 << " idx1 " << +idx1 << " (hashmap[idx0] << 4): " <<(hashmap[idx0] << 4) <<
//		std::endl;
		bytes_arr.push_back((uint8_t) (hashmap[idx0] << 4) | hashmap[idx1]);

	}
//	int i = 0;
//	for (auto &a : bytes_arr) {
//		std::cout << " i: " << i << " val: " << +a << std::endl;
//		i++;
//	}
//
//	std::cout << std::endl;

	/// for testing:
	this->uid = bytes_arr[6];
	this->function_code = bytes_arr[7];
	this->starting_add = ((uint16_t) (bytes_arr[8] << 8) | bytes_arr[9]);
	this->quantity_of_regs = ((uint16_t) (bytes_arr[10] << 8) | bytes_arr[11]);

//	std::cout << "hashmap[11] " << +(bytes_arr[11])
//			<< " ((uint16_t)(hashmap[10] << 8) | hashmap[11]) "
//			<< +((uint16_t) (bytes_arr[10] << 8) | bytes_arr[11]) << std::endl;
//
//	std::cout << "uid: " << +(this->uid) << " function_code: "
//			<< +(this->function_code) << " starting_add:"
//			<< +(this->starting_add) << " quantity_of_regs:"
//			<< +(this->quantity_of_regs) << std::endl;
//	assert(this->starting_add == 0x6B);
}

Message::~Message() {
}

