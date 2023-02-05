#include <iostream>
#include "TransportManager.cpp"
#include "Logic.cpp"
int main(int argc, char **argv) {
	TransportManager *transport = new TransportManager();
	Logic * my_logic = new Logic();
	SlaveManager *slave_manager = new SlaveManager();

	my_logic->init(slave_manager);
//	my_logic->start();

	transport->init(slave_manager);
	transport->start();

	return 0;
}
