#ifndef POCUTERBUTTONS_H
#define POCUTERBUTTONS_H

class PocuterButtons {
    public:
        enum PBUTTONS {
            BUTTON_1=0x01,
            BUTTON_2=0x02,
            BUTTON_3=0x04
        };
        typedef void (buttonEventHandler)(PBUTTONS, void*);
        
        virtual void registerEventHandler(buttonEventHandler*, void*) = 0;
        virtual void unregisterEventHandler() = 0;
        virtual PBUTTONS getButtonState() = 0;
       
    private:
        
};

#endif /* POCUTERBUTTONS_H */

