
#ifndef POCUTERSDCARD_H
#define POCUTERSDCARD_H

class PocuterSDCard {
    public:
        /**
        * @brief Cheks if a SDCard is in the Slot
        *
        *
        * @return
        *     - true if a card is detected
        *     - false if no card is detected
        */
        virtual bool cardInSlot() = 0;
        
        /**
        * @brief Cheks if a SDCard is mounted
        * 
        * @note The Pocuter automatically mounts FAT32 formatted cars to getMountPoint()
        *
        * @return
        *     - true if a card is detected
        *     - false if no card is detected
        */
        virtual bool cardIsMounted() = 0;
        
        
        /**
        * @brief get the mountpoint of the SDCard as zero terminated c string
        * 
        * @note you will get the standard mountpoint even if no card is mounted. 
        *
        * @return
        *     - zero terminated c string
        *     
        */
        virtual const char* getMountPoint() = 0;
    private:

};

#endif /* POCUTERSDCARD_H */

