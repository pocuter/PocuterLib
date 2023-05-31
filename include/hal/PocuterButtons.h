#ifndef POCUTERBUTTONS_H
#define POCUTERBUTTONS_H


/**
* @brief options to access the Pocuter buttons
*  
*/

class PocuterButtons {
    public:
        enum PBUTTON {
            BUTTON_NUM_P1=0,
            BUTTON_NUM_P2,
            BUTTON_NUM_P3,
            BUTTON_NUM_UP,    /*!< d-pad up */
            BUTTON_NUM_DOWN,  /*!< d-pad down */
            BUTTON_NUM_LEFT,  /*!< d-pad left */
            BUTTON_NUM_RIGHT, /*!< d-pad right */
            BUTTON_NUM_A,     /*!< A button */
            BUTTON_NUM_B,     /*!< B button */
            BUTTON_NUM_HOME   /*!< HOME button */
           
        };
        enum PBUTTONS {
            BUTTON_1=0x01,  /*!< Pocuter Button 1 */
            BUTTON_2=0x02,  /*!< Pocuter Button 2 */
            BUTTON_3=0x04,  /*!< Pocuter Button 3 */
            BUTTON_UP=0x08,    /*!< d-pad up */
            BUTTON_DOWN=0x10,  /*!< d-pad down */
            BUTTON_LEFT=0x20,  /*!< d-pad left */
            BUTTON_RIGHT=0x40, /*!< d-pad right */
            BUTTON_A=0x80,     /*!< A button */
            BUTTON_B=0x100,     /*!< B button */
            BUTTON_HOME=0x200,     /*!< Home button */
                    
        };
        enum PBACTION {
            BUTTON_PRESSED=0x1,     /*!< button is pressed (fires once at first frame) */
            BUTTON_RELEASED=0x2,    /*!< button is released (fires once at first frame) */
            BUTTON_HELD=0x4,        /*!< button is held (fires continuously when pressed down) */
            BUTTON_ACTIVATED=0x8,   /*!< button got activated (fires repeatedly in regular intervals when pressed down) */
        };
        
        
       
        virtual PBUTTONS getButtonState() = 0;
        
        virtual void updateButtons();
        virtual PBACTION getButtonStatus(PBUTTON button);
        virtual bool isButtonPressedDown(PBUTTON button);
       
    private:
        
};

#endif /* POCUTERBUTTONS_H */

