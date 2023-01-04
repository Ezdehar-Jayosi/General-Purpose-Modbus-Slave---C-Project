#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include "SlaveManager.cpp"

class TCP {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	char buffer[1024] = { 0 };
	int addrlen = sizeof(address);
	std::string IP;
	int port;
	//*********************//
	SlaveManager *slave_manager;

public:

	TCP(SlaveManager *slave_manager, int port, std::string IP) {
		this->slave_manager = slave_manager;
		this->port = port;
		this->IP = IP;
	}
	void init() {

		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0) {
			perror("socket failed");
			exit(1);
		}
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
			perror("setsocket");
			exit(EXIT_FAILURE);
		}

		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = inet_addr(IP.c_str());

		int res = bind(server_fd, (struct sockaddr*) &address, sizeof(address));
		if (res < 0) {
			perror("bind error");
			exit(EXIT_FAILURE);
		}
		printf("Bind to the port number: %d\n", port);
	}

	void connect() {
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		printf("listening...\n");

		if ((new_socket = accept(server_fd, (struct sockaddr*) &address,
				(socklen_t*) &addrlen)) < 0) {
			perror("accept");
			std::cout << "fail to accept connection" << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "before do while" << std::endl;
		do {
			valread = read(new_socket, buffer, 1024);
//			printf("%s\n",buffer);
			std::string msg_str(buffer);
			this->slave_manager->handleMSG(msg_str);
			memset(&buffer[0], 0, sizeof(buffer));

		} while (true);

	}

};
