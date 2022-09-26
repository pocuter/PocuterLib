#ifndef POCUTERCONFIG_H
#define POCUTERCONFIG_H
#include <stdint.h>
#include <stddef.h>

/**
* @brief save configuration on the SD Card or get app specific configuration data
*  
*/
class PocuterConfig {
public:
    /**
    * @brief constructor for a new config instance
    * 
    * @note the appIF is optional. If you do not give an appID the ID of the currently running app will be used.
    * 
    * @param configName a name for this configuration
    * @param appID (optional)the appID
    * 
    * 
    */
    PocuterConfig(const uint8_t* configName, uint64_t* appID = NULL);
    
    
    /**
    * @brief constructor for a new config instance for retrieving the app metadata 
    * 
    * @note the app meta data is readonly. Meta data could be the name or the creator for example.
    * 
    * @param appID the appID
    * 
    * 
    */
    PocuterConfig(uint64_t appID);
    
    /**
    * @brief get string data
    * 
    * @note If the data is encrypted, only the pocuter where the data was stored can decrypt it.
    * 
    * @param section the section designation
    * @param name the name
    * @param result the resulting string. The memory has to be allocated before calling this function.
    * @param maxLength the size of the buffer
    * @param decrypt (optional) decrypt - set it to true if the data is encrypted
    *
    * @return bool - true if it was successfully read  
    */
    bool get(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength, bool decrypt = false);
    /**
    * @brief set string data
    * 
    * @note If the data is encrypted, only the pocuter where the data was stored can decrypt it. If the section and the name exists already it will be overwritten
    * 
    * @param section the section designation
    * @param name the name
    * @param value the zero terminated string value
    * @param encrypt (optional) decrypt - set it to true if the data is encrypted
    * 
    * @return bool - true if it was successfully set 
    */
    bool set(const uint8_t* section, const uint8_t* name, const uint8_t* value, bool encrypt = false);
   
    
    /**
    * @brief delete a value
    * 
    * 
    * @param section the section designation
    * @param name the name
    * 
    * @return bool - true if it was successfully deleted 
    */
    bool del(const uint8_t* section, const uint8_t* name);


    
    /**
    * @brief get a 32bit unsigned number value
    * 
    * @note if you need to know, if this value exists please save the value as a string or binary instead
    * 
    * @param section the section designation
    * @param name the name
    * 
    * @return the value. It will returns a 0, if the value does not exist.
    */
    uint32_t get(const uint8_t* section, const uint8_t* name);
    
    
    /**
    * @brief set a 32bit unsigned number value
    * 
    * 
    * @param section the section designation
    * @param name the name
    * @param value the the uin32_t value
    * 
    * @return bool - true if it was successfully deleted 
    */
    bool set(const uint8_t* section, const uint8_t* name, uint32_t value);
    
    
    /**
    * @brief get a binary data blob
    * 
    * @note If the data is encrypted, only the pocuter where the data was stored can decrypt it.
    *  
    * @param section the section designation
    * @param name the name
    * @param result the resulting data blob. The memory has to be allocated before calling this function.
    * @param maxLength size of the buffer
    * @param decrypt (optional)does it need to be decrypted?
    * 
    * @return bool - true if it was successfully loaded 
    */
    bool getBinary(const uint8_t* section, const uint8_t* name, void* result, size_t maxLength, bool decrypt = false);
    
    /**
    * @brief set a binary data blob
    * 
    * @note If the data is encrypted, only the pocuter where the data was stored can decrypt it.
    *  
    * @param section the section designation
    * @param name the name
    * @param value the data blob
    * @param valueLength the size of the data blob
    * @param encrypt (optional)should it be encrypted?
    * 
    * @return bool - true if it was successfully saved 
    */
    bool setBinary(const uint8_t* section, const uint8_t* name, const void* value, size_t valueLength, bool encrypt = false);
    
    virtual ~PocuterConfig();
    
    /**
    * @brief the destructor. 
    * 
    */
private:
    bool getEncrypted(const uint8_t* section, const uint8_t* name, uint8_t* result, size_t maxLength);
    bool setEncrypted(const uint8_t* section, const uint8_t* name, const uint8_t* value);

    uint8_t* m_configFile;
    bool m_readony;
};

#endif /* POCUTERCONFIG_H */

