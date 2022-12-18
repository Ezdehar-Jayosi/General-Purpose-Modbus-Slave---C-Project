
#include "TCP.cpp"
#define PORT 8888
class TransportManager{
private:
	TCP* tcp; // TODO: change this to a vector of tcps, and add threads to the implementation
			  // give each thread the start function;
public:
	TransportManager(){}

	void init(SlaveManager* slave_manager){
		this->tcp = new TCP(slave_manager);
		tcp->init(PORT);
	}
	void start(){
		std::cout<< "transportmanager start."<<std::endl;
		tcp->connect();
	}
};
