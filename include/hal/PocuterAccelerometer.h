
#ifndef POCUTERACCELEROMETER_H
#define POCUTERACCELEROMETER_H


class PocuterAccelerometer {
    public:
        enum ACCERROR {
           ACCERROR_OK = 0,                     /*!< everything went well */
           ACCERROR_OFFLINE,                    /*!< no accelerometer found */
           ACCERROR_COMMUNICATION_FAILED,       /*!< communication problems with the accelerometer */
           ACCERROR_UNKNOWN                     /*!< unknown error */
        };
        enum ACCSCALE {
            G2 = 0,     /*!< Accelerometer scale factor 0 */
            G4 = 1,     /*!< Accelerometer scale factor 1 */
            G8 = 2      /*!< Accelerometer scale factor 2 */
        };
        struct State {
            int16_t x; /*!< Accelerometer position in space, x axis*/
            int16_t y; /*!< Accelerometer position in space, y axis*/
            int16_t z; /*!< Accelerometer position in space, z axis*/
        };
        enum ACCEVENT {
            ACC_ORIENTATION,    /*!< Accelerometer event, orientation has changed */
            ACC_SHAKE           /*!< Accelerometer event, the pocuter was shaked */
        };   
        typedef void (accEventHandler)(ACCEVENT, void*); /*!< type definition of the event callback function */
       
         /**
        * @brief register an event callback
        * 
        * @note the event handler is called every time an ACCEVENT occurred.
        * 
        * @param e a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        * @return 
        *     - ACCERROR_OK everything works well
        *     - ACCERROR_OFFLINE can not add an event handler now
        */
       virtual ACCERROR registerEventHandler(accEventHandler* e, void* u) = 0;
       
       /**
        * @brief unregister the event handler
        * 
        */
       
       virtual void unregisterEventHandler() = 0; 
        /**
        * @brief checks if the accelerometer is online
        * 
        * @return true / false
        */
       virtual ACCERROR isOnline() = 0;
       /**
        * @brief get the current position of the accelerometer in space.
        * 
        * @param state the state struct
        *
        * @return   ACCERROR_OK everything works well
        *           ACCERROR_OFFLINE no accelerometer found
        *           ACCERROR_COMMUNICATION_FAILED an communication error the the accelerometer occurred
        */
       virtual ACCERROR getState(State* state) = 0;

       /**
        * @brief set the accelerometer scale.
        * 
        * 
        * @return   ACCERROR_OK everything works well
        *           ACCERROR_OFFLINE no accelerometer found
        *           ACCERROR_COMMUNICATION_FAILED an communication error the the accelerometer occurred
        */
       virtual ACCERROR setScale(ACCSCALE) = 0;
       
       /**
        * @brief get the current sensor temperature
        * 
        * 
        * @return   ACCERROR_OK everything works well
        *           ACCERROR_OFFLINE no accelerometer found
        *           ACCERROR_COMMUNICATION_FAILED an communication error the the accelerometer occurred
        */
       
       virtual ACCERROR getTemperature(int8_t*) = 0;
       
    private:
        
};

#endif /* POCUTERACCELEROMETER_H */

