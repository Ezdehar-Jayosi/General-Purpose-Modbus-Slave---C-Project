#include "../GPMS Manager/SlaveManager.h"


class Logic {
	SlaveManager *slave_manager;
	int temp;
	std::thread* tmp_thrd;
public:
	Logic();

   void setTempreture(uint16_t add);

	static void onModChange(uint16_t add, uint16_t val);

	static void onPowerChange(uint16_t add, uint16_t val);

	static void onTemperatureChange(uint16_t add, uint16_t val);

	static void onFanSpeedChange(uint16_t add, uint16_t val);
	void init(SlaveManager *slave_manager);
//	void start(){
//
//	}
};
