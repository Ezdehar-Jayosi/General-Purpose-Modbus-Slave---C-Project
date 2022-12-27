#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>


class regTables {

public:
	std::unordered_map<std::byte, std::byte> reg_table;

	regTables(){

	}
	virtual std::byte readReg(std::byte key);
	virtual void writeToReg(std::byte key, std::byte value);
	virtual ~regTables();

};

class DiscreteInputs: public regTables{

public:
	std::byte readReg(std::byte key) {
		return this->reg_table[key];
	}
	void writeToReg(std::byte key, std::byte value) {
		this->reg_table[key] = value;
	}
};


class coilsTable : public regTables{

public:
	std::byte readReg(std::byte key) {
		return this->reg_table[key];
	}
	void writeToReg(std::byte key, std::byte value) {
		this->reg_table[key] = value;
	}
};


class InputRegister: public regTables{

public:
	std::byte readReg(std::byte key) {
		return this->reg_table[key];
	}
	void writeToReg(std::byte key, std::byte value) {
		this->reg_table[key] = value;
	}
};


class HoldingTable : public regTables{

public:
	std::byte readReg(std::byte key) {
		return this->reg_table[key];
	}
	void writeToReg(std::byte key, std::byte value) {
		this->reg_table[key] = value;
	}
};

