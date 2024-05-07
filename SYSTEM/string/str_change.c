/*
 * ���ļ���Ҫ����C���Խ���csv��ʽ/������ʽ���ַ����� ��ȡ��ֵ��֡ͷ֡βУ��
 

*/


#include "crstrlib.h"
#include "string.h"
/* �ж��Ƿ�Ϊ��ֵ�ַ� '0' - '9'*/

Boolean CRStrLib_isNum(char c){
    if (c >= '0' && c <= '9'){
        return TRUE;
    }
    return FALSE;
}

/* ���ַ�ת��Ϊ��ֵ '0' -> 0 */
int CRStrLib_char2Int(char c){
    return (int)(c - '0');
}

/* �ж��Ƿ�Ϊ������ */
Boolean CRStrLib_isSymbol(char c){
    if( c == '+' || c == '-'){
        return TRUE;
    }
    return FALSE;
}

int CRStrLib_strLen(char *str)//�����ַ����ĳ���
	{
    int i;

    /* Ϊ�˷�ֹ�ڴ���� �޶�string�ĳ��� */
    for(i = 0; i < MAX_STR_NUM; i++){
        if(str[i] == '\0'){
            return i;
        }
    }

    return MAX_STR_NUM;

}
/**
�������ܣ�Ѱ����һ������
���������char *str ��������ַ���
			    int strLen���ַ����ĳ���
					int *strIdx�������ĵ�ַ
					int *result���ҵ�������
 **/
Status CRStrLib_findNextInt(char *str, int strLen, int *strIdx, int *result){
    int symbol = 1; // ��ֵ����
    int baseValue = 0;

    /* Ѱ���ײ�Ϊ��ֵ�����ַ� */
    while(CRStrLib_isNum(str[*strIdx]) == FALSE){
        (*strIdx) += 1;
        if (*strIdx >= strLen){
            /* ���û������ */
            return STATUS_NOT_FOUND;
        }
    }
    /* ���Ƿ��� */
    if(*strIdx > 0 && CRStrLib_isSymbol(str[*strIdx - 1]) == TRUE){
        if (str[*strIdx - 1] == '-'){
            symbol = -1;
        }
    }

    /* Ѱ�����������������ַ� */
    while(*strIdx < strLen && CRStrLib_isNum(str[*strIdx])){
        baseValue *= 10;
        // printf("add num : %c %d\n", str[*strIdx], (int)str[*strIdx]);
        baseValue += CRStrLib_char2Int(str[*strIdx]);
        *strIdx += 1;
    }

    *result = symbol * baseValue;

    return STATUS_SUCCESS;
}

/* Ѱ����һ�������� */
Status CRStrLib_findNextFloat(char *str, int strLen, int *strIdx, float *result)
	{

    int intPart = 0; // ��������
    float decimalPart = 0.0; // С������
    int status;
		 float base = 0.1;
    int symbol = 1;

    /* Ѱ���ײ�Ϊ��ֵ�����ַ� */
    while(CRStrLib_isNum(str[*strIdx]) == FALSE){
        (*strIdx) += 1;
        if (*strIdx >= strLen){
            /* ���û������ */
            return STATUS_NOT_FOUND;
        }
    }

    /* ���Ƿ��� */
    if(*strIdx > 0 && CRStrLib_isSymbol(str[*strIdx - 1]) == TRUE){
        if (str[*strIdx - 1] == '-'){
            symbol = -1;
        }
    }

    status = CRStrLib_findNextInt(str, strLen, strIdx, &intPart);
    if (status != STATUS_SUCCESS){
        /* �������ֻ�ȡʧ�� */
        return status;
    }

    /* ֻ��Ҫ��ȡ����ֵ */
    if(intPart < 0){
        intPart *= -1;
    }

    /* �鿴��һλ�ǲ���С����λ */
    if (str[*strIdx] == '.' && CRStrLib_isNum(str[(*strIdx) + 1])){
        // int pointIdx = strIdx; // ��¼С�����λ��
        *strIdx += 1;
       

        // ��Ҫ���ǵ�С������0�����
        // CRStrLib_findNextInt(str, strLen, strIdx, &decimalPart);
        while(*strIdx < strLen && CRStrLib_isNum(str[*strIdx]) == TRUE){
            decimalPart += base * (float)CRStrLib_char2Int(str[*strIdx]);
            base *= 0.1;
            *strIdx += 1;
        }

    }

    // ���������
    *result = symbol * ((float)intPart + decimalPart);

    return STATUS_SUCCESS;
}

/* �ж��Ƿ�Ϊ��Ч���� */
Boolean CRStrLib_isValidData(char *str, char *head, char *tail){
    /* У��֡ͷ֡β*/
    if ((CRStrLib_startWith(str, head) == TRUE )&&( CRStrLib_endWith(str, tail) == TRUE)){
        return TRUE;
    }

    return FALSE;
}

/* �ж�����֡ͷ�Ƿ�ƥ�� */
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

/* �ж�֡β�Ƿ�ƥ�� */
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


