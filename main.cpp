#include <iostream>
#include "TransportManager.cpp"
#include "Logic.cpp"
int main(int argc, char **argv) {
	TransportManager* transport = new TransportManager();
	SlaveManager* slave_manager = new SlaveManager();
	transport->init(slave_manager);
	transport->start();
	return 0;
}
