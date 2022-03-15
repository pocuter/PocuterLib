/*  Glue functions for the minIni library, based on the C/C++ stdio library
 *
 *  Or better said: this file contains macros that maps the function interface
 *  used by minIni to the standard C/C++ file I/O functions.
 *
 *  By CompuPhase, 2008-2012
 *  This "glue file" is in the public domain. It is distributed without
 *  warranties or conditions of any kind, either express or implied.
 */

/* map required file I/O types and functions to the standard C library */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ff.h>
#include "include/hal/esp32-c3/esp32_c3_OTA.h"
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
    FILE* f = fopen(filename, mode);
    if (f && strstr(fn, "esp32c3.app")) {
        // this seems to be an app file, go to the position where ini starts
        PocuterLib::HAL::esp32_c3_OTA::fheader header;
        fread(&header, sizeof(header), 1, f);
        fpos_t position = header.startMetaFile;
        fsetpos(f, &position);
    }
    free(fn);
    return f;
}
FILE * pocwfopen ( const char * filename, const char * mode ) {
    char * fn = toLower(filename, strlen(filename));
    if (! strstr(fn, ".ini")) {
        // wirte only ini files
        return NULL;
    }
    return fopen(filename, mode);
}
#define INI_FILETYPE                  FILE*
#define ini_openread(filename,file)   ((*(file) = pocrfopen((filename),"r")) != NULL)
#define ini_openwrite(filename,file)  ((*(file) = pocwfopen((filename),"w")) != NULL)
#define ini_close(file)               (fclose(*(file)) == 0)
#define ini_read(buffer,size,file)    (fgets((buffer),(size),*(file)) != NULL)
#define ini_write(buffer,file)        (fputs((buffer),*(file)) >= 0)
#define ini_rename(source,dest)       (rename((source), (dest)) == 0)
#define ini_remove(filename)          (remove(filename) == 0)

#define INI_FILEPOS                   fpos_t
#define ini_tell(file,pos)            (fgetpos(*(file), (pos)) == 0)
#define ini_seek(file,pos)            (fsetpos(*(file), (pos)) == 0)

/* for floating-point support, define additional types and functions */
#define INI_REAL                      float
#define ini_ftoa(string,value)        sprintf((string),"%f",(value))
#define ini_atof(string)              (INI_REAL)strtod((string),NULL)
