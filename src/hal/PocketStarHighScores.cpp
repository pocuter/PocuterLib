#include "include/PocuterLibConfig.h"

#ifndef POCUTER_DISABLE_HIGH_SCORES
#include "include/hal/PocketStarHighScores.h"

#undef local

PocketStarHighScores::PocketStarHighScores(PocuterHMAC* pHMAC, PocuterOTA* pOTA, PocuterHTTP* pHTTP) {
    m_HMAC = pHMAC;
    m_pOTA = pOTA;
    m_pHTTP = pHTTP;
}

PocketStarHighScores::~PocketStarHighScores() {}

bool PocketStarHighScores::loadHighScores(size_t count) {
    m_appId = esp32_c3_OTA::getCurrentAppID();
    
    highScoreLists = (HighScoreList*) malloc(count * sizeof(HighScoreList));
    if (highScoreLists == NULL)
        return false;
    
    char strbuf[16];
    bool success, successTotal = true;
    PocuterConfig *config = new PocuterConfig((const uint8_t*) "HighScores", &m_appId);
    
    for (int list = 0; list < count; list++) {
        sprintf(strbuf, "List%d", list);
        success = config->getBinary((const uint8_t*) "HighScores", (const uint8_t*) strbuf, highScoreLists[list].local, sizeof(highScoreLists[list].local));
        
        if (!success) {
            for (int i = 0; i < NUM_HIGHSCORES; i++) {
                strncpy(highScoreLists[list].local[i].name, "---", MAX_HIGHSCORE_NAME);
                highScoreLists[list].local[i].name[MAX_HIGHSCORE_NAME] = '\0';
                highScoreLists[list].local[i].score = INT_MIN;
            }
        }
        
        successTotal = successTotal && success;
    }
        
    // TODO load global scores and delete the following
    for (int list = 0; list < count; list++) {
        for (int i = 0; i < NUM_HIGHSCORES; i++) {
            strncpy(highScoreLists[list].global[i].name, "---", MAX_HIGHSCORE_NAME);
            highScoreLists[list].global[i].name[MAX_HIGHSCORE_NAME] = '\0';
            highScoreLists[list].global[i].score = INT_MIN;
        }
    }
    
    delete config;
    
    return successTotal;
}

int PocketStarHighScores::getHighScore(uint8_t list, HSLIST type, uint8_t n) {
    if (highScoreLists == NULL)
        return -1;
    
    if (type == HSLIST_GLOBAL)
        return highScoreLists[list].global[n].score;
    else
        return highScoreLists[list].local[n].score;
}

char* PocketStarHighScores::getHighScoreName(uint8_t list, HSLIST type, uint8_t n) {
    if (highScoreLists == NULL)
        return NULL;
    
    if (type == HSLIST_GLOBAL)
        return highScoreLists[list].global[n].name;
    else
        return highScoreLists[list].local[n].name;
}

bool PocketStarHighScores::isNewHighScore(uint8_t list, int score) {
    if (highScoreLists == NULL)
        return false;
    
    return score > highScoreLists[list].local[NUM_HIGHSCORES-1].score;
    //return (score > highScoreLists[list].local[NUM_HIGHSCORES-1].score || score > highScoreLists[list].global[NUM_HIGHSCORES-1].score);
}

void PocketStarHighScores::addHighScore(uint8_t list, int score, char *name) {
    if (highScoreLists == NULL || !isNewHighScore(list, score))
        return;
    
    if (score > highScoreLists[list].local[NUM_HIGHSCORES-1].score) {
        HighScoreList *hs = &highScoreLists[list];
        
        hs->local[NUM_HIGHSCORES-1].score = score;
        strncpy(hs->local[NUM_HIGHSCORES-1].name, name, MAX_HIGHSCORE_NAME);
        
        for (int i = NUM_HIGHSCORES-1; i > 0; i--) {
            if (score > hs->local[i-1].score) {
                hs->local[i].score = hs->local[i-1].score;
                strcpy(hs->local[i].name, hs->local[i-1].name);
                
                hs->local[i-1].score = score;
                strcpy(hs->local[i-1].name, name);
            }
        }
        
        char strbuf[16];
        sprintf(strbuf, "List%d", list);
        PocuterConfig *config = new PocuterConfig((const uint8_t*) "HighScores", &m_appId);
        bool success = config->setBinary((const uint8_t*) "HighScores", (const uint8_t*) strbuf, highScoreLists[list].local, sizeof(highScoreLists[list].local));
        delete config;
    }
    
    if (score > highScoreLists[list].global[NUM_HIGHSCORES-1].score) {
        // TODO try to send score to server
        
        // Update the in-memory list.
        // The online list is only retrieved on startup.
        HighScoreList *hs = &highScoreLists[list];
        
        hs->global[NUM_HIGHSCORES-1].score = score;
        strncpy(hs->global[NUM_HIGHSCORES-1].name, name, MAX_HIGHSCORE_NAME);
        
        for (int i = NUM_HIGHSCORES-1; i > 0; i--) {
            if (score > hs->global[i-1].score) {
                hs->global[i].score = hs->global[i-1].score;
                strcpy(hs->global[i].name, hs->global[i-1].name);
                
                hs->global[i-1].score = score;
                strcpy(hs->global[i-1].name, name);
            }
        }
    }
}

#endif
