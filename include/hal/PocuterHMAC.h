/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   PocuterHMAC.h
 * Author: Test
 *
 * Created on 5. Februar 2022, 15:22
 */

#ifndef POCUTERHMAC_H
#define POCUTERHMAC_H

class PocuterHMAC {
    public:
        virtual bool setEfuseKey(const uint8_t* masterKey, uint8_t keyLength) = 0;
        virtual uint8_t* getChipID() = 0;
        virtual void calculateChallengeReply(const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength) = 0;
        virtual void calculateChallengeReplySW(const uint8_t* masterKey, uint8_t keyLength, const uint8_t* challenge, uint8_t challengeLength, uint8_t* reply, uint8_t& replyLength) = 0;
    private:

};


#endif /* POCUTERHMAC_H */

