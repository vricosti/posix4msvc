#if 0
wchar_t *
char_to_wchar(const char *text)
{
   wchar_t *wtext;
   int      wsize;

   if (!text)
     return NULL;

   wsize = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text) + 1, NULL, 0);
   if ((wsize == 0) ||
       (wsize > (int)(ULONG_MAX / sizeof(wchar_t))))
     {
        if (wsize == 0)
          _evil_last_error_display(__FUNCTION__);
        return NULL;
     }

   wtext = malloc(wsize * sizeof(wchar_t));
   if (wtext)
     if (!MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text) + 1, wtext, wsize))
     {
        _evil_last_error_display(__FUNCTION__);
        return NULL;
     }

   return wtext;
}

char *
wchar_to_char(const wchar_t *text)
{
   char  *atext;
   int    asize;

   if (!text)
     return NULL;

   asize = WideCharToMultiByte(CP_ACP, 0, text, -1, NULL, 0, NULL, NULL);
   if (asize == 0)
     {
        _evil_last_error_display(__FUNCTION__);
        return NULL;
     }

   atext = (char*)malloc(asize * sizeof(char));
   if (!atext)
     return NULL;

   asize = WideCharToMultiByte(CP_ACP, 0, text, -1, atext, asize, NULL, NULL);
   if (asize == 0)
     {
        _evil_last_error_display(__FUNCTION__);
        return NULL;
     }

   return atext;
}
#endif