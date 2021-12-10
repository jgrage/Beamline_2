/* gauge.hpp
 * Jonas
 */
#ifndef GAUGE_HPP
#define GAUGE_HPP

typedef enum GAUGE_STATE{HIGH_P = 0, LOW_P = 1} GAUGE_STATE;

class gauge{
    public:
        gauge();
        void connect(int pin);
        void update_state();
        
        GAUGE_STATE state;
        
    private:
        int _readback_pin;
        
    protected:
    
};
#endif
