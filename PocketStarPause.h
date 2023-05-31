#ifndef POCKETSTARPAUSE_H
#define POCKETSTARPAUSE_H


#include <Pocuter.h>


class PocketStarPauseMenu {
private:
    
    bool isPaused;
    uint64_t lastInputMs;
    
    uint8_t selectedOption;
    uint8_t brightness;
    uint8_t volume;
    bool vibrationOn;
    color_t systemColor;
    Pocuter *m_pocuter;
    
    bool getInput(PocuterButtons::PBUTTON button);
    void enterPause();
    void exitPause();
    void updatePause();
    void drawPause();
    
    static void menuTask(void *arg);
    
public:
    PocketStarPauseMenu(Pocuter *pocuter);
    void start();
    bool checkPause();
};






extern void onPause();
extern void onResume();



#endif /* POCKETSTARPAUSE_H */
