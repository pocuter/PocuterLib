
#ifndef POCUTERSDCARD_H
#define POCUTERSDCARD_H
/**
* @brief informations about the sd card 
*  
*/
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
        * @note The Pocuter automatically mounts FAT32 formatted cards to getMountPoint()
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

        /**
        * @brief mount a card if there is an unmounted card present in the slot
        *
        * @return
        *     - true card has been mounted or is already mounted
        *     - false no card present
        *     
        */
        virtual bool mount();

        /**
        * @brief unmount card if there is a mounted card
        */
        virtual void unmount();

    private:

};

#endif /* POCUTERSDCARD_H */

