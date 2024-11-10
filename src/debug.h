#ifndef INCL_DEBUG
#define INCL_DEBUG

#ifdef DEBUG
    // #include "debugmalloc.h"
#endif


//Define debug print macros
#ifndef DEBUG
#define PRINTDEBUG_MALLOCNULL() ;;
#define PRINTDEBUG_FILEERR();;
#define PRINTDEBUG_CORRUPTEDFILE() ;;
#define PRINTDEBUG_CUSTOM(str, ...) ;;
#endif
#ifdef DEBUG
#define PRINTDEBUG_MALLOCNULL() printf("[DEBUG] Malloc returned with null in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_FILEERR() printf("[DEBUG] Error flag was set while doing file operations in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_CORRUPTEDFILE() printf("[DEBUG] Invalid or corrupted file found in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_CUSTOM(str, ...) printf("[DEBUG] " #str "[file %s, line: %d, function: %s]\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);
#endif



#endif