#include "dencode.h"

/* Base-64 decoding.  This represents binary data as printable ASCII
** characters.  Three 8-bit binary bytes are turned into four 6-bit
** values, like so:
**
**   [11111111]  [22222222]  [33333333]
**
**   [111111] [112222] [222233] [333333]
**
** Then the 6-bit values are represented using the characters "A-Za-z0-9+/".
*/

static prog_char base64dtab[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/*
 * Do base-64 decoding on a string. Ignore any non-base64 bytes.
 * Return the actual number of bytes generated. The decoded size will
 * be at most 3/4 the size of the encoded, and may be smaller if there
 * are padding characters (blanks, newlines).
 */
char *NutDecodeBase64(u_char * str)
{
    int code;
    char *sp;
    char *tp;
    int last = -1;
    int step = 0;

    for (tp = sp = str; *sp; ++sp) {
        if ((code = PRG_RDB(&base64dtab[(int) *sp])) == -1)
            continue;
        switch (step++) {
        case 1:
            *tp++ = ((last << 2) | ((code & 0x30) >> 4));
            break;
        case 2:
            *tp++ = (((last & 0xf) << 4) | ((code & 0x3c) >> 2));
            break;
        case 3:
            *tp++ = (((last & 0x03) << 6) | code);
            step = 0;
            break;
        }
        last = code;
    }
    *tp = 0;
    return str;
}

int NutDecodeHex(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

char *NutDecodePath(char *path)
{
    int x1;
    int x2;
    char *src = path;
    char *dst = path;
    char last = *path;

    if (last != '/')
        return 0;
    while (*++src) {
        if (*src == '%' &&
            (x1 = NutDecodeHex(*(src + 1))) >= 0 &&
            (x2 = NutDecodeHex(*(src + 2))) >= 0) {
            src += 2;
            if ((*src = x1 * 16 + x2) == 0)
                break;
        }
        if (*src == '\\')
            *src = '/';
        if ((last != '.' && last != '/') || (*src != '.' && *src != '/'))
            *dst++ = last = *src;

    }
    *dst = 0;

    return path;
}
