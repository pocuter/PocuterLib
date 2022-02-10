
#ifndef POCUTERSDCARD_H
#define POCUTERSDCARD_H

class PocuterSDCard {
    public:
        virtual bool cardInSlot() = 0;
        virtual bool cardIsMounted() = 0;
        virtual const char* getMountPoint() = 0;
    private:

};

#endif /* POCUTERSDCARD_H */

