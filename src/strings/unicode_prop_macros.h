#ifndef UNICODE_PROPS_MACROS_H
#define UNICODE_PROPS_MACROS_H

#define MVM_CP_is_White_Space(cp) (((cp) == 9) || ((cp) == 10) || ((cp) == 11) || ((cp) == 12) || ((cp) == 13) || ((cp) == 32) || ((cp) == 133) || ((cp) == 160) || ((cp) == 5760) || ((cp) == 8192) || ((cp) == 8193) || ((cp) == 8194) || ((cp) == 8195) || ((cp) == 8196) || ((cp) == 8197) || ((cp) == 8198) || ((cp) == 8199) || ((cp) == 8200) || ((cp) == 8201) || ((cp) == 8202) || ((cp) == 8232) || ((cp) == 8233) || ((cp) == 8239) || ((cp) == 8287) || ((cp) == 12288))
#define MVM_CP_is_gencat_name_Zl(cp) (((cp) == 8232))
#define MVM_CP_is_gencat_name_Zp(cp) (((cp) == 8233))

#endif