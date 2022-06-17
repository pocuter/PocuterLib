#ifndef POCUTERCONFIG_H
#define POCUTERCONFIG_H
#include <stdint.h>
#include <stddef.h>
class PocuterConfig {
public:
    PocuterConfig(const uint8_t* configName, uint64_t* pappID = NULL);
    PocuterConfig(uint64_t appID);
    
    bool get(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength);
    bool set(const uint8_t* section, const uint8_t* name, const uint8_t* value);
    bool del(const uint8_t* section, const uint8_t* name);
#ifndef POCUTER_NO_CONFIG_ENCRYPTION    
    bool getEncrypted(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength);
    bool setEncrypted(const uint8_t* section, const uint8_t* name, const uint8_t* value);
#endif
    
    
    uint32_t get(const uint8_t* section, const uint8_t* name);
    bool set(const uint8_t* section, const uint8_t* name, uint32_t value);
    
    
    bool getBinary(const uint8_t* section, const uint8_t* name, void* result, size_t maxLength);
    bool setBinary(const uint8_t* section, const uint8_t* name, const void* value, size_t valueLength);
    
    virtual ~PocuterConfig();
private:
    uint8_t* m_configFile;
    bool m_readony;
};

#endif /* POCUTERCONFIG_H */

