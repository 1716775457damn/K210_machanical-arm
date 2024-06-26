/*
 * 此文件主要用于C语言解析csv格式/其他格式的字符串， 提取数值，帧头帧尾校验
 

*/


#include "crstrlib.h"
#include "string.h"
/* 判断是否为数值字符 '0' - '9'*/

Boolean CRStrLib_isNum(char c){
    if (c >= '0' && c <= '9'){
        return TRUE;
    }
    return FALSE;
}

/* 将字符转换为数值 '0' -> 0 */
int CRStrLib_char2Int(char c){
    return (int)(c - '0');
}

/* 判断是否为正负号 */
Boolean CRStrLib_isSymbol(char c){
    if( c == '+' || c == '-'){
        return TRUE;
    }
    return FALSE;
}

int CRStrLib_strLen(char *str)//计算字符串的长度
	{
    int i;

    /* 为了防止内存溢出 限定string的长度 */
    for(i = 0; i < MAX_STR_NUM; i++){
        if(str[i] == '\0'){
            return i;
        }
    }

    return MAX_STR_NUM;

}
/**
函数功能：寻找下一个整数
传入参数：char *str ：传入的字符串
			    int strLen：字符串的长度
					int *strIdx：整数的地址
					int *result：找到的整数
 **/
Status CRStrLib_findNextInt(char *str, int strLen, int *strIdx, int *result){
    int symbol = 1; // 数值正负
    int baseValue = 0;

    /* 寻找首部为数值的子字符 */
    while(CRStrLib_isNum(str[*strIdx]) == FALSE){
        (*strIdx) += 1;
        if (*strIdx >= strLen){
            /* 如果没有整数 */
            return STATUS_NOT_FOUND;
        }
    }
    /* 考虑符号 */
    if(*strIdx > 0 && CRStrLib_isSymbol(str[*strIdx - 1]) == TRUE){
        if (str[*strIdx - 1] == '-'){
            symbol = -1;
        }
    }

    /* 寻找相连的所有数字字符 */
    while(*strIdx < strLen && CRStrLib_isNum(str[*strIdx])){
        baseValue *= 10;
        // printf("add num : %c %d\n", str[*strIdx], (int)str[*strIdx]);
        baseValue += CRStrLib_char2Int(str[*strIdx]);
        *strIdx += 1;
    }

    *result = symbol * baseValue;

    return STATUS_SUCCESS;
}

/* 寻找下一个浮点数 */
Status CRStrLib_findNextFloat(char *str, int strLen, int *strIdx, float *result)
	{

    int intPart = 0; // 整数部分
    float decimalPart = 0.0; // 小数部分
    int status;
		 float base = 0.1;
    int symbol = 1;

    /* 寻找首部为数值的子字符 */
    while(CRStrLib_isNum(str[*strIdx]) == FALSE){
        (*strIdx) += 1;
        if (*strIdx >= strLen){
            /* 如果没有整数 */
            return STATUS_NOT_FOUND;
        }
    }

    /* 考虑符号 */
    if(*strIdx > 0 && CRStrLib_isSymbol(str[*strIdx - 1]) == TRUE){
        if (str[*strIdx - 1] == '-'){
            symbol = -1;
        }
    }

    status = CRStrLib_findNextInt(str, strLen, strIdx, &intPart);
    if (status != STATUS_SUCCESS){
        /* 整数部分获取失败 */
        return status;
    }

    /* 只需要获取绝对值 */
    if(intPart < 0){
        intPart *= -1;
    }

    /* 查看下一位是不是小数点位 */
    if (str[*strIdx] == '.' && CRStrLib_isNum(str[(*strIdx) + 1])){
        // int pointIdx = strIdx; // 记录小数点的位置
        *strIdx += 1;
       

        // 需要考虑到小数部分0的情况
        // CRStrLib_findNextInt(str, strLen, strIdx, &decimalPart);
        while(*strIdx < strLen && CRStrLib_isNum(str[*strIdx]) == TRUE){
            decimalPart += base * (float)CRStrLib_char2Int(str[*strIdx]);
            base *= 0.1;
            *strIdx += 1;
        }

    }

    // 添加正负号
    *result = symbol * ((float)intPart + decimalPart);

    return STATUS_SUCCESS;
}

/* 判断是否为有效数据 */
Boolean CRStrLib_isValidData(char *str, char *head, char *tail){
    /* 校验帧头帧尾*/
    if ((CRStrLib_startWith(str, head) == TRUE )&&( CRStrLib_endWith(str, tail) == TRUE)){
        return TRUE;
    }

    return FALSE;
}

/* 判断数据帧头是否匹配 */
Boolean CRStrLib_startWith(char *str, char *head){
    int strLen = strlen(str);
    int headLen = strlen(head);
    int i;
    if (strLen < headLen){
			  return FALSE;
    }

    for(i = 0; i < headLen; i += 1){
        if (str[i] != head[i]){
            return FALSE;
        }
    }
    return TRUE;
}

/* 判断帧尾是否匹配 */
Boolean CRStrLib_endWith(char *str, char *tail)
{
    int strLen = strlen(str);
    int tailLen = strlen(tail);
    int i;
    if (strLen < tailLen){
        return FALSE;
    }

    for(i = 0; i < tailLen; i += 1){
        if (str[strLen - i - 1] != tail[tailLen - i - 1]){
            return FALSE;
        }
    }
    return TRUE;
}


