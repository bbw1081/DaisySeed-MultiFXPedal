#include "ff.h"

/* Minimal unicode support for FatFs - ASCII only, sufficient for short filenames */

WCHAR ff_convert(WCHAR wch, UINT dir)
{
    if(wch < 0x80)
    {
        /* ASCII - no conversion needed */
        return wch;
    }
    return 0; /* unsupported character */
}

WCHAR ff_wtoupper(WCHAR wch)
{
    if(wch >= 'a' && wch <= 'z')
        return wch - 0x20;
    return wch;
}