#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>
#include "../Utilities/ModbusTypeDefs.h"
typedef void (*CallBack)(uint16_t, uint16_t);

template<typename T> class regTables {
	std::unordered_map<uint16_t, T> reg_table;
	std::unordered_map<uint16_t, CallBack> cbf_map_table;
	uint16_t start_add;
	uint16_t last_add;
public:
	regTables(uint16_t start_add, uint16_t last_add) {
		this->start_add = start_add;
		this->last_add = last_add;

	}
	bool addInRange(uint16_t add) {
		if (add < start_add || add > last_add) {
			return false;
		}
		return true;
	}
	T readReg(uint16_t key) {
		//check if key exits in map
		auto it = this->reg_table.find(key);
		if (it != reg_table.end()) {
			return it->second;
		}
		return 0x0;
	}
//	void writeToReg(uint8_t key, T value, CallBackf callBackFunction, FunctionCode funCode) {
//		//check if key exits in map
//		auto it = this->reg_table.find(key);
//		if (it != reg_table.end()) {
//			this->reg_table.erase(it);
//		}
//
//		//add <key,value> to map
//		this->reg_table.insert(std::pair<uint8_t, T>(key, value));
//		callBackFunction(funCode, key, value);
//	}
	void writeToReg(uint16_t key, T value) {
		//check if key exits in map
		auto it = this->reg_table.find(key);
		if (it != reg_table.end()) {
			this->reg_table.erase(it);
		}

		//add <key,value> to map
		this->reg_table.insert(std::pair<uint8_t, T>(key, value));

		auto cbf_it = this->cbf_map_table.find(key);
		if (cbf_it != cbf_map_table.end()) {
			this->cbf_map_table[key](key, value);
		}

	}
	void setCallBackFunc(CallBack handler, uint16_t regadd) {
		this->cbf_map_table.insert(
				std::pair<uint16_t, CallBack>(regadd, handler));
	}
	~regTables();

};

