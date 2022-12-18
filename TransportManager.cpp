
#include "TCP.cpp"

class TransportManager{
private:
	TCP* tcp;
	SlaveManager* slave_manager;
public:
	TransportManager(){}
	void init(SlaveManager* slave_manager){
		this->slave_manager=slave_manager;
		this->tcp = new TCP(slave_manager);
		tcp->init(503);
	}
	void start(){
		tcp->connect();
	}
};
