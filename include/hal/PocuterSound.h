
#ifndef POCUTERSOUND_H
#define POCUTERSOUND_H

class PocuterSound {
    public:
        enum SOUNDERROR {
            SOUNDERROR_OK = 0,
            SOUNDERROR_FAILED
        };
        enum SAMPLE_RATE {
            SAMPLE_RATE_8000 = 8000,
            SAMPLE_RATE_11025 = 11025,
            SAMPLE_RATE_22050 = 22050,
            SAMPLE_RATE_32000 = 32000,
            SAMPLE_RATE_44100 = 44100,
            SAMPLE_RATE_48000 = 48000
        };
        
        enum SOUND_CHANNELS {
            SOUND_CHANNELS_MONO = 1,
            SOUND_CHANNELS_STEREO = 2
        };
        enum SOUND_BIT_RATE {
            SOUND_BIT_RATE_8 = 8,
            SOUND_BIT_RATE_16 = 16,
            
        };
        virtual SOUNDERROR init(SAMPLE_RATE sr, SOUND_CHANNELS c, SOUND_BIT_RATE b) = 0;
        virtual SOUNDERROR start() = 0;
        virtual SOUNDERROR stop() = 0;
        virtual SOUNDERROR write(uint8_t *inbuf, size_t inbuf_len, size_t *bytes_written) = 0;
        virtual SOUNDERROR setVolume(int8_t volume) = 0;
        virtual SOUNDERROR getVolume(int8_t *volume) = 0;
};



#endif /* POCUTERSOUND_H */

