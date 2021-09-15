#ifndef _SENSOR_H_
#define _SENSOR_H_


class SENSOR { 
    public:
        SENSOR(int pin_, bool missingSignalIsOut_);
        void setup();
        bool IsIn();
        bool IsOut();
        bool IsOutOfBounds();
        void handleInterrupt();
        

    private:
        static int inside_code[];
        static int outside_code[];

        int pin;
        bool missingSignalIsOut;
        unsigned long lastInTime;
        unsigned long lastOutTime;
        unsigned long last_pulse;
        int signel_status;
        int pulse_count_inside;
        int pulse_count_outside;

};

#endif