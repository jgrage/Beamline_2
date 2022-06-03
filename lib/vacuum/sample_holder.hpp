/* sample_holder.hpp
 * Jonas
 */
#ifndef SAMPLE_HOLDER_HPP
#define SAMPLE_HOLDER_HPP

typedef enum SAMPLE_HOLDER_STATE{REMOVED = 0, INSERTED = 1} SAMPLE_HOLDER_STATE;

class sample_holder{
    public:
        sample_holder();
        void connect(int pin);
        void update_state();
        
        SAMPLE_HOLDER_STATE state;
        
    private:
        int _readback_pin;
        
    protected:
    
};
#endif
