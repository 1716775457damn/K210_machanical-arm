#ifndef _CRSTRLIB_H
#define _CRSTRLIB_H



/* status ×´Ì¬Âë */
typedef int Status;

#define STATUS_SUCCESS 0
#define STATUS_FAIL -1
#define STATUS_NOT_FOUND 404
#define MAX_STR_NUM 1000

/* boolean */
typedef int Boolean;
#define FALSE 0
#define TRUE 1

Boolean CRStrLib_isNum(char c);
int CRStrLib_char2Int(char c);
Boolean CRStrLib_isSymbol(char c);
int CRStrLib_strLen(char *str);
Status CRStrLib_findNextInt(char *str, int strLen, int *strIdx, int *result);
Status CRStrLib_findNextFloat(char *str, int strLen, int *strIdx, float *result);
Boolean CRStrLib_isValidData(char *str, char *head, char *tail);
Boolean CRStrLib_startWith(char *str, char *head);
Boolean CRStrLib_endWith(char *str, char *tail);



#endif



