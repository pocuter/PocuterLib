#include "include/minIni/minGlue.h"

char *toLower(const char *str, size_t len)
{
    char *str_l = (char*)calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

FILE * pocrfopen ( const char * filename, const char * mode ) {
    char * fn = toLower(filename, strlen(filename));
    bool isAPP = false;
    if (strstr(fn, "esp32c3.app")) isAPP = true;
    free(fn);
    if (! isAPP) return fopen(filename, mode);
    FILE* f = fopen(filename, "r");
    if (f) {
        // this seems to be an app file, go to the position where ini starts
        PocuterLib::HAL::esp32_c3_OTA::fheader header;
        fread(&header, sizeof(header), 1, f);
        fpos_t position = header.startMetaFile;
        if (position == 0){
            fclose(f);
            return NULL;
        }
        fsetpos(f, &position);
        
    }
    return f;
   
}
FILE * pocwfopen ( const char * filename, const char * mode ) {
    char * fn = toLower(filename, strlen(filename));
    bool isAPP = false;
    if (strstr(fn, "esp32c3.app")) isAPP = true;
    free(fn);
    if (isAPP) {
        // wirte only ini files
        return NULL;
    }
    return fopen(filename, mode);
}