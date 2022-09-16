
#ifndef POCUTERPORTS_H
#define POCUTERPORTS_H

class PocuterPorts {
    public:
       enum PORT_NUMBER {
           PORT_0,
           PORT_1,
           PORT_2,
           PORT_3,
           PORT_4,
           PORT_5
       };
        
       enum PORT_DIRECTION {
          PORT_DIRECTION_IN,
          PORT_DIRECTION_OUT
          
       };
       enum PORT_PULLUP {
          PORT_PULLUP_OFF,
          PORT_PULLUP_ON
          
       };
       
       enum PORT_MODE {
           PORT_MODE_BINARY,
           PORT_MODE_ADC
       };
       enum PORTSERROR {
            PORTSERROR_OK,
            PORTSERROR_NOT_SUPPORTED,
            PORTSERROR_NOT_INITIALIZED,
            PORTSERROR_UNKNOWN
       };
        
        
       /**
        * @brief init an exposed port
        * 
        * @note not all ports have the same possibilities
        *       Port 0: BIN or ADC, IN/OUT BIN, PULLUP
        *       Port 1: BIN, IN/OUT
        *       Port 2: BIN, IN/OUT
        *       Port 3: BIN, IN/OUT
        *       Port 4: BIN, IN/OUT
        *       Port 5: BIN or ADC, IN/OUT BIN, PULLUP
        * 
        *       in ADC mode you cannot register an event handler
        * 
        * @param n PORT_0 to PORT_5
        * @param d IN or OUT direction
        * @param m binary or ADC mode
        * @param p internal pullup on/off
        * 
        * @return 
        *     - PORTSERROR_OK everything works well
        *     - PORTSERROR_NOT_SUPPORTED this combination of options is not possible
        */
       
       virtual PORTSERROR initPort(PORT_NUMBER n, PORT_DIRECTION d, PORT_MODE m, PORT_PULLUP p) = 0;
       
       /**
        * @brief deinit an exposed port
        * 
        * @note this also cancels the event registration
        * 
        * @param n PORT_0 to PORT_5
        * 
        * @return 
        *     - PORTSERROR_OK everything works well
        *     - PORTSERROR_NOT_INITIALIZED the port wasn't initialized
        */
       
       virtual PORTSERROR deinitPort(PORT_NUMBER n) = 0;
       
       
       
       
       /**
        * @brief get a value from an ADC port 
        * 
        * @param n PORT_0 to PORT_5
        * @param value a pointer to a memory where the value should be saved
        * 
        * @return 
        *     - PORTSERROR_OK everything works well
        *     - PORTSERROR_NOT_INITIALIZED the port wasn't initialized
        */
       virtual PORTSERROR getValue(PORT_NUMBER n, uint16_t* value);
       
       /**
        * @brief get a value from a binary port 
        * 
        * @param n PORT_0 to PORT_5
        * @param value a pointer to a memory where the value should be saved
        * 
        * @return 
        *     - PORTSERROR_OK everything works well
        *     - PORTSERROR_NOT_INITIALIZED the port wasn't initialized
        */
       virtual PORTSERROR getValue(PORT_NUMBER n, bool* value);
       
        /**
        * @brief set a binary port 
        * 
        * @param n PORT_0 to PORT_5
        * @param value set or unset the port
        * 
        * @return 
        *     - PORTSERROR_OK everything works well
        *     - PORTSERROR_NOT_INITIALIZED the port wasn't initialized
        */
               
       virtual PORTSERROR setValue(PORT_NUMBER n, bool value); 
       
       typedef void (portEventHandler)(PORT_NUMBER n, bool portValue, void*); /*!< type definition of the event callback function */
       
       
       /**
        * @brief register an event callback
        * 
        * @note this is olny available for binary ports 
        * @note the event handler is called every time the value of a binary port changed. 
        * 
        * @param n the port number
        * @param h a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        * 
        */
       virtual PORTSERROR registerEventHandler(PORT_NUMBER n, portEventHandler* h, void* u) = 0;
       
       

    private:

};

#endif /* POCUTERPORTS_H */

