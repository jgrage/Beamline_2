/* main.cpp
 * Jonas
 */
#ifndef VALVE_HPP
#define VALVE_HPP
    
typedef enum VALVE_STATE{OPEN = 0,CLOSED = 1, MOVING = 2, CLOSED_BLOCKED = 3, OPEN_BLOCKED = 4} VALVE_STATE;

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

#endif
