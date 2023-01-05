#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>


template <typename T> class regTables {

public:
	std::unordered_map<uint8_t, T> reg_table;

	regTables(){

	}
	T readReg(uint8_t key);
	void writeToReg(uint8_t key, T value){
		std::cout << "writing to reg" << std::endl;

	}
	~regTables();

};

//class DiscreteInputs: public <bool> regTables{
//
//public:
//	std::byte readReg(uint8_t key) {
//		return this->reg_table[key];
//	}
//	void writeToReg(uint8_t key, std::byte value) {
//		this->reg_table[key] = value;
//	}
//};
//
//
//class coilsTable : public regTables{
//
//public:
//	std::byte readReg(uint8_t key) {
//		return this->reg_table[key];
//	}
//	void writeToReg(uint8_t key, std::byte value) {
//		this->reg_table[key] = value;
//	}
//};
//
//
//class InputRegister: public regTables{
//
//public:
//	std::byte readReg(uint8_t key) {
//		return this->reg_table[key];
//	}
//	void writeToReg(uint8_t key, std::byte value) {
//		this->reg_table[key] = value;
//	}
//};
//
//
//class HoldingTable : public regTables{
//
//public:
//	std::byte readReg(std::byte key) {
//		return this->reg_table[key];
//	}
//	void writeToReg(std::byte key, std::byte value) {
//		this->reg_table[key] = value;
//	}
//};

