#include "settings.h"
#include "system.h"

long lastFrame;

void setup() {
    pocuter = new Pocuter();
    pocuter->begin(PocuterDisplay::BUFFER_MODE_DOUBLE_BUFFER);
    pocuter->Display->continuousScreenUpdate(false);
    
    pocuterSettings.brightness = getSetting("GENERAL", "Brightness", 5);
    pocuter->Display->setBrightness(pocuterSettings.brightness);
    pocuterSettings.systemColor = getSetting("GENERAL", "SystemColor", C_LIME);
    pocuterSettings.timeUntilStandby = getSetting("GENERAL", "TimeUntilStandby", 30);

    pocuter->Sleep->setSleepMode(PocuterSleep::SLEEP_MODE_LIGHT);
    pocuter->Sleep->setWakeUpModes(PocuterSleep::WAKEUP_MODE_ANY_BUTTON);
    pocuter->Sleep->setInactivitySleep(pocuterSettings.timeUntilStandby, PocuterSleep::SLEEPTIMER_INTERRUPT_BY_BUTTON);

    // enable or disable double click (disabling can achieve faster reaction to single clicks)
    enableDoubleClick(BUTTON_A);
    enableDoubleClick(BUTTON_B);
    enableDoubleClick(BUTTON_C);

    // setup your app here
    
    lastFrame = micros();
}

void loop() {
    dt = (micros() - lastFrame) / 1000.0 / 1000.0;
    lastFrame = micros();
    updateInput();

    if (ACTION_BACK_TO_MENU) {
        pocuter->OTA->setNextAppID(1);
        pocuter->OTA->restart();
    }

    // update your app here
    // dt contains the amount of time that has passed since the last update, in seconds
}

void onPause() {
    // this function gets called on PocketStars when the user presses the HOME button.
    // you could save the game state to SD here, but for most games, this can go empty.
}

void onResume() {
    // this gets called on PocketStars when the pause menu is closed.
    // can also leave this empty for most games.
}
