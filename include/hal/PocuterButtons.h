#ifndef POCUTERBUTTONS_H
#define POCUTERBUTTONS_H


/**
* @brief options to access the Pocuter buttons
*  
*/

class PocuterButtons {
    public:
        enum PBUTTONS {
            BUTTON_1=0x01,  /*!< the lower right button */
            BUTTON_2=0x02,  /*!< the upper right button */
            BUTTON_3=0x04   /*!< the lower left button */
        };
        
        typedef void (buttonEventHandler)(PBUTTONS, void*);  /*!< type definition of the event callback function */
        
        /**
        * @brief register an event callback
        * 
        * @note the event handler is called every time an button was pressed or released. the PBUTTONS value is a bit matrix with all buttons. so to check if a button was pressed you can do a "if(getButtonState() & PocuterButtons::BUTTON_1) {...}" for example
        * 
        * @param e a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        * 
        */
        virtual void registerEventHandler(buttonEventHandler* e, void* u) = 0;
        
        /**
        * @brief unregister the event handler
        * 
        */
        
        virtual void unregisterEventHandler() = 0;
        /**
        * @brief get the current state  of the buttons
        * 
        * @note the return value is a bit matrix with all buttons. so to check if a button was pressed you can do a "if(getButtonState() & PocuterButtons::BUTTON_1) {...}" for example
        *  
        * @return PBUTTONS
        */
        virtual PBUTTONS getButtonState() = 0;
       
    private:
        
};

#endif /* POCUTERBUTTONS_H */

