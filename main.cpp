#include <iostream>
#include "./Transport Layer/TransportManager.cpp"
#include "./Logic/Logic.h"
int main(int argc, char **argv) {
	TransportManager *transport = new TransportManager();
	Logic * my_logic = new Logic();
	SlaveManager *slave_manager = new SlaveManager();

	transport->init(slave_manager);
	my_logic->init(slave_manager);
	transport->start();

	return 0;
}
