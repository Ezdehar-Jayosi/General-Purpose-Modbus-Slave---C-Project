#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>


template <typename T> class regTables {
	std::unordered_map<uint16_t, T> reg_table;
public:
	regTables(){

	}
	std::pair<T,bool> readReg(uint8_t key){
	//check if key exits in map
		auto it = this->reg_table.find(key);
		if(it != reg_table.end()){
			return std::pair<T,bool>(it->second, true); // TODO: check if we should return 0x0 if a reg is not found in the map
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

