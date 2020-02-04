#ifndef cPOURYA__h
#define cPOURYA__h

#ifdef __cplusplus
extern "C"
{
#endif

#define POURYA_False 0
#define POURYA_True 1
#define cPOURYA_NULL 2
#define cPOURYA_Number 3
#define cPOURYA_String 4
#define cPOURYA_Array 5
#define cPOURYA_Object 6

#define cPOURYA_IsReference 256
#define cPOURYA_StringIsConst 512


typedef struct cPOURYA
{
    struct cPOURYA *next;
    struct cPOURYA *prev;
    struct cPOURYA *child;
    int type;
    char *valuestring;
    int valueint;
    double valuedouble;
    char *string;
} cPOURYA;

static char* cPOURYA_SaveString(const char* str);
extern cPOURYA *cPOURYA_CreateString(const char *string);
extern cPOURYA *cPOURYA_CreateString(const char *string);
extern cPOURYA *cPOURYA_CreateArray();
extern cPOURYA *cPOURYA_CreateObject();
extern void cPOURYA_AddItemToArray(cPOURYA *array, cPOURYA *item);
extern void cPOURYA_AddItemToObject(cPOURYA *object,const char *str,cPOURYA *item);
extern int cPOURYA_GetArraySize(cPOURYA *array);
extern cPOURYA *cPOURYA_GetArrayItem(cPOURYA *array,int item);
extern cPOURYA *cPOURYA_GetObjectItem(cPOURYA *object,const char *string);
extern char *cPOURYA_Print(cPOURYA *item);
extern cPOURYA *cPOURYA_Parse(char *str);
extern void cPOURYA_Delete(cPOURYA *item);
extern cPOURYA *cPOURYA_DetachItemFromArray(cPOURYA *array,int item);
extern void cPOURYA_DeleteItemFromArray(cPOURYA *array,int item);
extern cPOURYA *cPOURYA_DetachItemFromObject(cPOURYA *object,const char *string);
extern void cPOURYA_DeleteItemFromObject(cPOURYA *object,const char *string);


#ifdef __cplusplus
}
#endif
#endif
