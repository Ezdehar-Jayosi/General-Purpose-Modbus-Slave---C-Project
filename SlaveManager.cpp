
#include <stdio.h>
#include<string>
#include <iostream>
#include "regTables.cpp"


class SlaveManager{
	// Registers Tables
	regTables* regT;
	HoldingTable* holdT;
	coilsTable* coilsT;
	DiscreteInputs* discrtT;

public:
	SlaveManager(){}
	void handleMSG(std::string msg){  //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
									  // TODO: change argument type, argument is of type ByteArray
		std::cout<<"SLAVEMANAGER PRINTING: ";
		std::cout<< msg << std::endl;
	}

//	void setRegister(int regAdd, int regVal, int func_code){
//
//	}
//
//	void setCallBackFunction(int regAdd, void (*regFunc)(int regAdd,int regVal) ){
//
//	}
};
