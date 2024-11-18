#ifndef INCL_DEBUG
#define INCL_DEBUG

/// @brief Megnézi, hogy ptr NULL-e. Ha igen, akkor kilép a programból 1es
/// értékkel, különben nem változtatja meg a futást
#define CHECKMALLOCNULL(ptr) if(ptr == NULL) { PRINTDEBUG_MALLOCNULL(); exit(1); }

#ifndef DEBUG
/// @brief Kiírja hogy egy memóriafoglalás sikertelen volt.
#define PRINTDEBUG_MALLOCNULL() ;;
/// @brief Kiírja, hogy a fájlművelet sikertelen volt.
#define PRINTDEBUG_FILEERR();;
/// @brief Kiírja, hogy dekódolás közben nem várt karakterrel találkoztnk.
#define PRINTDEBUG_CORRUPTEDFILE() ;;
/// @brief Általános hibakeresésre használható, konzolra való kiírásra alkalmas.
#define PRINTDEBUG_CUSTOM(str, ...) ;;
#endif


#ifdef DEBUG
#include "./debug/debugmalloc.h"

#define PRINTDEBUG_MALLOCNULL() printf("[DEBUG] Malloc returned with null in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_FILEERR() printf("[DEBUG] Error flag was set while doing file operations in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_CORRUPTEDFILE() printf("[DEBUG] Invalid or corrupted file found in function '%s' (file %s, line %d)\n", __FUNCTION__, __FILE__, __LINE__);
#define PRINTDEBUG_CUSTOM(str, ...) printf("[DEBUG] " #str "[file %s, line: %d, function: %s]\n", __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);
#endif



#endif