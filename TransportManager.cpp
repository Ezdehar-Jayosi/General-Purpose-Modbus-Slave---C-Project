#include "TCP.cpp"
#include "/home/ezdehar/Downloads/rapidjson-master/include/rapidjson/writer.h"
#include "/home/ezdehar/Downloads/rapidjson-master/include/rapidjson/document.h"
#include "/home/ezdehar/Downloads/rapidjson-master/include/rapidjson/stringbuffer.h"
#include <vector>
#include <fstream>      // std::ofstream
#include <iostream>     // std::streambuf, std::cout
#include <sstream>
#include <thread>

using namespace rapidjson;

class TransportManager {
private:
	//TCP *tcp; // TODO: change this to a vector of tcps, and add threads to the implementation
			  // give each thread the start function;
	std::vector<TCP*> tcp_connections;
	std::vector<std::thread*> connections_threads;
	void thread_function(int indx) {
			std::cout << "connecting tcp number " << indx << std::endl;
			this->tcp_connections[indx]->connect();
		}

	void create_threads() {
		int num_of_threads = this->tcp_connections.size();
		for (int i = 0; i < num_of_threads; i++) {
			std::thread *thrd = new std::thread(
					[=] { thread_function(i); });
			this->connections_threads.push_back(thrd);
		}
	}
public:
	TransportManager() {
	}

	void init(SlaveManager *slave_manager) {
		//this->tcp = new TCP(slave_manager);
		//tcp->init(PORT);
		/*
		 * read form setting.conf and create tcp connections
		 */
		std::ifstream file("settings.conf");
		if (file) {
			std::ostringstream tmp;
			tmp << file.rdbuf();
			std::string s = tmp.str();
			Document d;
			d.Parse(s.c_str());
//			Value &s_v = d["connections"];
			for (auto &m : d["connections"].GetObject()) {
				for (auto &v : m.value.GetArray()) {
					std::string ip;
					int port;
					for (auto &t : v.GetObject()) { // read the ip and port
						std::string name = t.name.GetString();
						if (name == "ip") {
							std::cout << t.name.GetString() << " "
									<< t.value.GetString() << std::endl;
							ip = t.value.GetString();

						} else { // name is port
							std::cout << t.name.GetString() << " "
									<< t.value.GetInt() << std::endl;
							port = t.value.GetInt();
						}

					}
					//create and add a new tcp connection to vector<tcp>
					TCP *new_tcp = new TCP(slave_manager, port, ip);
					new_tcp->init();
					this->tcp_connections.push_back(new_tcp);
				}
			}

		}

	}

	void start() {
		std::cout << "TransportManager start." << std::endl;
		create_threads();
		for(auto& th: this->connections_threads){
			th->join();
		}
		//tcp->connect();
	}
};
