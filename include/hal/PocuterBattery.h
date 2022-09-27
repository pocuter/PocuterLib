
#ifndef POCUTERBATTERY_H
#define POCUTERBATTERY_H

class PocuterBattery {
    public:
	   
	   virtual uint16_t getBatteryLevel() = 0;
	   virtual bool isCharging() = 0;
        

    private:

};

#endif /* POCUTERBATTERY_H */

