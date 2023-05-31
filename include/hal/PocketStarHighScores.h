#ifndef POCKETSTARHIGHSCORES_H
#define POCKETSTARHIGHSCORES_H

#include "include/hal/PocuterHMAC.h"
#include "include/hal/PocuterOTA.h"
#include "include/hal/PocuterHTTP.h"

#define MAX_HIGHSCORE_NAME 16
#define NUM_HIGHSCORES 5

class PocketStarHighScores {
private:
    struct HighScore {
        int score;
        char name[MAX_HIGHSCORE_NAME+1];
    };
    struct HighScoreList {
        HighScore local[NUM_HIGHSCORES];
        HighScore global[NUM_HIGHSCORES];
    };
    HighScoreList *highScoreLists = NULL;
    uint64_t m_appId;
    
public:
    enum HSLIST {
        HSLIST_LOCAL=0,
        HSLIST_GLOBAL,
    };
    
    PocketStarHighScores(PocuterHMAC*, PocuterOTA*, PocuterHTTP*);
    virtual ~PocketStarHighScores();
    
    bool loadHighScores(size_t count);
    int getHighScore(uint8_t list, HSLIST type, uint8_t n);
    char* getHighScoreName(uint8_t list, HSLIST type, uint8_t n);
    bool isNewHighScore(uint8_t list, int score);
    void addHighScore(uint8_t list, int score, char *name);
    
private:
    PocuterHMAC* m_HMAC;
    PocuterOTA* m_pOTA;
    PocuterHTTP* m_pHTTP;
};

#endif /* POCKETSTARHIGHSCORES_H */