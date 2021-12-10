/* main.cpp
 * Jonas
 */
#ifndef VALVE_HPP
#define VALVE_HPP
    
typedef enum VALVE_STATE{CLOSED = 0, OPEN = 1, MOVING = 2, CLOSED_BLOCKED = 3, OPEN_BLOCKED = 4} VALVE_STATE;

static const char *VALVE_STATE_STRING[] = {"closed", "open", "moving", "closed_blocked", "open_blocked"};

class gauge;

/* -------------
 * Generic valve
 * -------------
 */
class valve
{
    public:
        void connect(int enable, int encoder_closed, int encoder_open);
        void open();
        void close();
        VALVE_STATE state;
        //gauge& left;
        //gauge& right;
        
    private:

    protected:
        int _encoder_closed_pin;
        int _encoder_open_pin;
        int _open_pin;
        
};

/* ---------------
 * blockable valve
 * ---------------
 */
class blockable_valve : public valve
{
    public:
        blockable_valve(gauge& left_section, gauge& right_section);
        void update_state();
        
    private:
        gauge& left;
        gauge& right;

    protected:
};

/* ------------------
 * Obstructable valve
 * ------------------
 */
class obstructable_valve : public valve
{
    public:
        obstructable_valve(gauge& left_section, gauge& right_section);
        void connect(int enable, int encoder_closed, int encoder_open, int lock);
        void update_state();
        
    private:
        gauge& left;
        gauge& right;
        
    protected:
        int _lock_pin;
        
};

/* ------------------------------------------------
 * angle valves used for venting and depressurizing
 * ------------------------------------------------
 */
class angle_valve : public valve
{
    public:
        angle_valve(valve& first_valve, valve& second_valve);
        void connect_third_valve(valve *third_valve);
        void update_state();
        
    private:
        valve& first;       // Gate Valve 1
        valve& second;      // Gate Valve 2
        valve *third;       // Other angle_valve

    protected:
        
};

#endif
