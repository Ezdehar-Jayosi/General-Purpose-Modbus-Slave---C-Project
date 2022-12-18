
#include <stdio.h>
#include<string>
#include <iostream>

class SlaveManager{

public:
	SlaveManager(){}
	void handleMSG(std::string msg){  //TODO: RETURN TYPE IS RES, WILL DEFINE LATER
		std::cout<< msg << std::endl;
	}

	void setRegister(int regAdd, int regVal, int func_code){

	}

	void setCallBackFunction(int regAdd, void (*regFunc)(int regAdd,int regVal) ){

	}
};
