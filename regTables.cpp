#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>


template <typename T> class regTables {

public:
	std::unordered_map<uint16_t, T> reg_table;

	regTables(){

	}
	std::pair<T,bool> readReg(uint8_t key){
	//check if key exits in map
		auto it = this->reg_table.find(key);
		if(it != reg_table.end()){
			return std::pair<T,bool>(it->second, true);
		}
		return std::pair<T,bool>(-1,false);
	}
	void writeToReg(uint8_t key, T value){
		//check if key exits in map
		auto it = this->reg_table.find(key);
		if(it != reg_table.end()){
			this->reg_table.erase(it);
		}

		//add <key,value> to map
		this->reg_table.insert(std::pair<uint8_t,T>(key,value));

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

