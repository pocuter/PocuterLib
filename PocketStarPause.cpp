#include "PocketStarPause.h"
#include "Pocuter.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "include/hal/esp32-c3/SSD1131_Display.h"
#include "include/hal/esp32-c3/esp32_c3_Buttons.h"
using namespace PocuterLib::HAL;

PocketStarPauseMenu::PocketStarPauseMenu(Pocuter *pocuter) {
    m_pocuter = pocuter;
    isPaused = false;
    start();
    xTaskCreate(&menuTask, "menuTask", 4000, this, 10, NULL);
    
}

void PocketStarPauseMenu::start() {
    // load settings
    brightness = 5;
    volume = 10;
    vibrationOn = true;
    systemColor = GFX_GREEN;
    int timeUntilStandby = 60;
    
    PocuterConfig config((const uint8_t *) "settings");
    char strbuf[16];
    if (config.get((const uint8_t*) "GENERAL", (const uint8_t*) "Brightness", (uint8_t*) strbuf, 16))
        brightness = atoi(strbuf);
    if (config.get((const uint8_t*) "GENERAL", (const uint8_t*) "Volume", (uint8_t*) strbuf, 16))
        volume = atoi(strbuf);
    if (config.get((const uint8_t*) "GENERAL", (const uint8_t*) "Vibration", (uint8_t*) strbuf, 16))
        vibrationOn = atoi(strbuf) != 0;
    if (config.get((const uint8_t*) "GENERAL", (const uint8_t*) "TimeUntilStandby", (uint8_t*) strbuf, 16))
        timeUntilStandby = atoi(strbuf);
    if (config.get((const uint8_t*) "GENERAL", (const uint8_t*) "SystemColor", (uint8_t*) strbuf, 16)) {
        uint32_t c = atoi(strbuf);
        uint8_t r = (c >> 16) & 0xFF;
        uint8_t g = (c >>  8) & 0xFF;
        uint8_t b = (c      ) & 0xFF;
        systemColor = m_pocuter->gfx->RGB2Color(r, g, b);
    }
    m_pocuter->Sleep->setSleepMode(PocuterSleep::SLEEP_MODE_LIGHT);
    m_pocuter->Sleep->setWakeUpModes(PocuterSleep::WAKEUP_MODE_ANY_BUTTON);
    m_pocuter->Sleep->setInactivitySleep(timeUntilStandby, PocuterSleep::SLEEPTIMER_INTERRUPT_BY_BUTTON);
    
    m_pocuter->Display->setBrightness(brightness);
    // TODO set other settings
}

void PocketStarPauseMenu::menuTask(void *arg) {
    PocketStarPauseMenu* myself = (PocketStarPauseMenu*)arg;
    while(true) {
        
        myself->checkPause();
        
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}


bool PocketStarPauseMenu::getInput(PocuterButtons::PBUTTON button) {
    uint64_t ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
    esp32_c3_Buttons* buttons = (esp32_c3_Buttons*)m_pocuter->Buttons;
    if ((ms > lastInputMs + 200) && buttons->isButtonPressedDownInternal(button)) {
        lastInputMs = ms;
        return true;
    }
    return false;
}

bool PocketStarPauseMenu::checkPause() {
    esp32_c3_Buttons* buttons = (esp32_c3_Buttons*)m_pocuter->Buttons;
    if (!isPaused) {
        // TODO use HOME button instead of A
        
        if (buttons->isButtonPressedDownInternal(PocuterButtons::BUTTON_NUM_HOME)) {
        //if (false) {
            m_pocuter->Display->continuousScreenUpdate(false);
            SSD1131_Display* display = (SSD1131_Display*)m_pocuter->Display;
            display->stopExternScreenUpdate();
            esp32_c3_Buttons* b = (esp32_c3_Buttons*)m_pocuter->Buttons;
            b->stopForwardingKeys();
            
            isPaused = true;
            enterPause();
            onPause();
            return true;
        }
        return false;
    } else {
        if (getInput(PocuterButtons::BUTTON_NUM_B)) {
            isPaused = false;
            exitPause();
            SSD1131_Display* display = (SSD1131_Display*)m_pocuter->Display;
            display->continueExternScreenUpdate();
            esp32_c3_Buttons* b = (esp32_c3_Buttons*)m_pocuter->Buttons;
            b->continueForwardingKeys();
            onResume();
            return false;
        }
        
        // draw info
        updatePause();
        drawPause();
        
        return true;
    }
}

void PocketStarPauseMenu::enterPause() {
    lastInputMs = xTaskGetTickCount() * portTICK_PERIOD_MS;
    selectedOption = 0;
}

void PocketStarPauseMenu::exitPause() {
    // save settings
    
    PocuterConfig config((const uint8_t *) "settings");
    char strbuf[16];
    config.set((const uint8_t*) "GENERAL", (const uint8_t*) "Brightness", brightness);
    config.set((const uint8_t*) "GENERAL", (const uint8_t*) "Volume", volume);
    config.set((const uint8_t*) "GENERAL", (const uint8_t*) "Vibration", vibrationOn ? 1 : 0);
}

void PocketStarPauseMenu::updatePause() {
    if (selectedOption > 0 && getInput(PocuterButtons::BUTTON_NUM_UP))
        selectedOption -= 1;
    if (selectedOption < 3 && getInput(PocuterButtons::BUTTON_NUM_DOWN))
        selectedOption += 1;
    
    switch (selectedOption) {
        case 0: {
            uint8_t oldBrightness = brightness;
            if (brightness > 0 && getInput(PocuterButtons::BUTTON_NUM_LEFT))
                brightness -= 1;
            if (brightness < 15 && getInput(PocuterButtons::BUTTON_NUM_RIGHT))
                brightness += 1;
            
            if (brightness != oldBrightness) {
                m_pocuter->Display->setBrightness(brightness);
            }
            break;
        }
        case 1: {
            uint8_t oldVolume = volume;
            if (volume > 0 && getInput(PocuterButtons::BUTTON_NUM_LEFT))
                volume -= 1;
            if (volume < 10 && getInput(PocuterButtons::BUTTON_NUM_RIGHT))
                volume += 1;
            
            if (volume != oldVolume) {
                //  TODO apply settings
            }
            break;
        }
        case 2: {
            bool oldVibrationOn = vibrationOn;
            if (vibrationOn && getInput(PocuterButtons::BUTTON_NUM_LEFT))
                vibrationOn = false;
            if (!vibrationOn && getInput(PocuterButtons::BUTTON_NUM_RIGHT))
                vibrationOn = true;
            
            if (vibrationOn != oldVibrationOn) {
                //  TODO apply settings
            }
            break;
        }
        case 3: {
            if (getInput(PocuterButtons::BUTTON_NUM_A)) {
                m_pocuter->OTA->setNextAppID(1);
                m_pocuter->OTA->restart();
            }
            break;
        }
    }
}

void PocketStarPauseMenu::drawPause() {
    char strbuf[32];
    SSD1131_Display* display = (SSD1131_Display*)m_pocuter->Display;
    display->waitAppScreenUdpate();
    
    m_pocuter->gfx->FillRect(12, 10, 72, 44, RGBA(0x00,0x00,0x00,PSCOLOR_TRANSLUCENCY_7_8TH));

    m_pocuter->gfx->DrawRect(12, 10, 72, 44, systemColor);
    
    m_pocuter->gfx->SetFontColors(GFX_TRANSPARENT, GFX_GREEN);
    m_pocuter->gfx->DrawString("PAUSED",       48, 19, ALIGN_CENTER, &psFont7x5);
    
    m_pocuter->gfx->SetFontColors(GFX_TRANSPARENT, (selectedOption == 0) ? GFX_ORANGE : GFX_CYAN);
    m_pocuter->gfx->DrawString("Brightness:",  14, 28, ALIGN_LEFT,  &psFont6x4);
    sprintf(strbuf, "%d%%", 25 + brightness*5);
    m_pocuter->gfx->DrawString(strbuf,         82, 28, ALIGN_RIGHT, &psFont6x4);
    
    m_pocuter->gfx->SetFontColors(GFX_TRANSPARENT, (selectedOption == 1) ? GFX_ORANGE : GFX_CYAN);
    m_pocuter->gfx->DrawString("Sound:",       14, 36, ALIGN_LEFT,  &psFont6x4);
    if (volume == 0) sprintf(strbuf, "off");
    else             sprintf(strbuf, "%d%%", volume*10);
    m_pocuter->gfx->DrawString(strbuf,         82, 36, ALIGN_RIGHT, &psFont6x4);
    
    m_pocuter->gfx->SetFontColors(GFX_TRANSPARENT, (selectedOption == 2) ? GFX_ORANGE : GFX_CYAN);
    m_pocuter->gfx->DrawString("Vibration:",   14, 44, ALIGN_LEFT,  &psFont6x4);
    sprintf(strbuf, "%s", vibrationOn ? "on" : "off");
    m_pocuter->gfx->DrawString(strbuf,         82, 44, ALIGN_RIGHT, &psFont6x4);
    
    m_pocuter->gfx->SetFontColors(GFX_TRANSPARENT, (selectedOption == 3) ? GFX_ORANGE : GFX_CYAN);
    m_pocuter->gfx->DrawString("Back to Menu", 14, 52, ALIGN_LEFT,  &psFont6x4);
    
    
    display->internalScreenUdpate();
    
}

