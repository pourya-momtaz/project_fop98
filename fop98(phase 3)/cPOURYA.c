#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include "cPOURYA.h"



static char* cPOURYA_SaveString(const char* str)
{
    char* save_string;
    save_string = (char*)malloc(strlen(str) + 1);
    if (!save_string)
        return 0;
    memcpy(save_string,str,strlen(str) + 1);
    return save_string;
}


cPOURYA *cPOURYA_CreateString(const char *string)
{
    cPOURYA *item=(cPOURYA*)malloc(sizeof(cPOURYA));
    if (item)
        memset(item,0,sizeof(cPOURYA));
    if(item)
    {
        item->type=cPOURYA_String;
        item->valuestring=cPOURYA_SaveString(string);
    }
    return item;
}


cPOURYA *cPOURYA_CreateArray()
{
    cPOURYA *item=(cPOURYA*)malloc(sizeof(cPOURYA));
    if (item)
        memset(item,0,sizeof(cPOURYA));
    if(item)
        item->type=cPOURYA_Array;
    return item;
}


cPOURYA *cPOURYA_CreateObject()
{
    cPOURYA *item=(cPOURYA*)malloc(sizeof(cPOURYA));
    if (item)
        memset(item,0,sizeof(cPOURYA));
    if(item)
        item->type=cPOURYA_Object;
    return item;
}



cPOURYA *cPOURYA_CreateStringArray(const char **str,int number)
{
    int i;
    cPOURYA *new_child,*old_child,*fake=cPOURYA_CreateArray();
    for(i=0; fake && i<number; i++)
    {
        new_child=cPOURYA_CreateString(str[i]);
        if(i==0)
            fake->child=new_child;
        else
        {
            old_child->next=new_child;
            new_child->prev=old_child;
        }
        old_child=new_child;
    }
    return fake;
}



void cPOURYA_AddItemToArray(cPOURYA *array, cPOURYA *item)
{
    cPOURYA *fake=array->child;
    if (!item)
        return;
    if (!fake)
    {
        array->child=item;
    }
    else
    {
        while (fake && fake->next)
        {
            fake=fake->next;
        }
        fake->next=item;
        item->prev=fake;
    }
}



void cPOURYA_AddItemToObject(cPOURYA *object,const char *str,cPOURYA *item)
{
    if (!item)
        return;
    if (item->string)
        free(item->string);
    item->string=cPOURYA_SaveString(str);
    cPOURYA_AddItemToArray(object,item);
}


int cPOURYA_GetArraySize(cPOURYA *array)
{
    cPOURYA *fake=array->child;
    int i=0;
    while(fake)
    {
        fake=fake->next;
        i++;
    }
    return i;
}



cPOURYA *cPOURYA_GetArrayItem(cPOURYA *array,int item)
{
    cPOURYA *fake=array->child;
    while (fake && item>0)
    {
        fake=fake->next;
        item--;
    }
    return fake;
}


cPOURYA *cPOURYA_GetObjectItem(cPOURYA *object,const char *string)
{
    cPOURYA *fake=object->child;
    while (fake && strcmp((fake->string),string))
    {
        fake=fake->next;
    }
    return fake;
}


char *cPOURYA_Print(cPOURYA *item)
{
    char *plus;
    char result[10000]="";
    if(item->type==cPOURYA_Array)
    {
        if(item->prev)
            plus=",\"";
        else
            plus="\"";
        strcat(result,plus);
        strcat(result,item->string);
        strcat(result,"\"");
        strcat(result,":[");
    }
    cPOURYA *fake=item->child;
    if(fake)
    {
        if(item->type==cPOURYA_Object)
            strcat(result,"{");
        do
        {

            //strcat(result,"\n");
            plus=cPOURYA_Print(fake);
            strcat(result,plus);
            fake=fake->next;
        }
        while(!((fake==NULL)));
        if(item->type==cPOURYA_Array)
        {
            strcat(result,"]");
        }
        if(item->type==cPOURYA_Object)
        {
            strcat(result,"}");
            if(!(item->next==NULL))
                strcat(result,",");
        }
        char *save=(char*)calloc(10000,sizeof(char));
        save=result;
        return save;
    }
    else
    {
        if(!(item->string))
        {
            char plus2[10000];
            if(!(item->prev))
            {
                plus2[0]='{';
                strcat(plus2,"\"");
            }
            else
            {
                plus2[0]=',';
                strcat(plus2,"{\"");
            }
            plus=item->valuestring;
            strcat(plus2,plus);
            strcat(plus2,"\"}");
            strcat(result,plus2);
            memset(plus2, 0, strlen(plus2));
            char *save=(char*)calloc(10000,sizeof(char));
            save=result;
            return save;
        }
        plus=item->string;
        char plus2[10000];
        if(!(item->prev))
        {
            plus2[0]='\"';
        }
        else
        {
            plus2[0]=',';
            strcat(plus2,"\"");
        }
        strcat(plus2,plus);
        strcat(plus2,"\"");
        strcat(plus2,":\"");
        plus=item->valuestring;
        strcat(plus2,plus);
        strcat(plus2,"\"");
        strcat(result,plus2);
        memset(plus2, 0, strlen(plus2));
        if(item->type==cPOURYA_Array)
        {
            strcat(result,"[");
        }
        char *save=(char*)calloc(10000,sizeof(char));
        save=result;
        return save;
    }
    char *save=(char*)calloc(10000,sizeof(char));
    save=result;
    return save;
}


cPOURYA *cPOURYA_Parse(char *str)
{
    int flag=0,flag2=0,flag3=0,flag4=0,flag5=0;
    cPOURYA *result=cPOURYA_CreateObject();
    char *word=strtok(str,"{:,");
    char *word2=strtok(NULL,"{:,");
    char *word3;
    char *word4;
    while(word2[strlen(word2)-1]!='}')
    {
        if(strcmp(word2,"["))
        {
            memmove(&word[0], &word[1], strlen(word) - 0);
            memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
            memmove(&word2[0], &word2[1], strlen(word2) - 0);
            memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
            cPOURYA_AddItemToObject(result,word,cPOURYA_CreateString(word2));

        }

        else
        {

            char save_word[10000];
            strcpy(save_word,word);
            memmove(&save_word[0], &save_word[1], strlen(save_word) - 0);
            memmove(&save_word[strlen(save_word)-1], &save_word[strlen(save_word)], 1);

            word=strtok(NULL,"{:,");

            if(word[strlen(word)-1]==']'||word[strlen(word)-1]=='}')
            {
                char *name[]= {};
                cPOURYA_AddItemToObject(result,save_word,cPOURYA_CreateStringArray(name,0));
                while(word[strlen(word)-1]!=']')
                {
                    if(word[strlen(word)-1]=='}')
                    {
                        if(word[strlen(word)-2]==']')
                        {
                            flag5=1;
                            break;
                        }
                        memmove(&word[0], &word[1], strlen(word) - 0);
                        memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                        memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                        cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),cPOURYA_CreateString(word));
                    }
                    word=strtok(NULL,"{:,");
                }
                if(flag5==1)
                {
                    memmove(&word[0], &word[1], strlen(word) - 0);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),cPOURYA_CreateString(word));
                }
                else
                {
                    memmove(&word[0], &word[1], strlen(word) - 0);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),cPOURYA_CreateString(word));
                }
            }

            else
            {
                cPOURYA_AddItemToObject(result,save_word,cPOURYA_CreateArray());
                word2=strtok(NULL,"{:,");
                while(word2[strlen(word2)-1]!=']')
                {
                    if(word2[strlen(word2)-1]=='}')
                    {
                        if(word2[strlen(word2)-2]==']')
                        {
                            flag2=1;
                            break;

                        }
                        cPOURYA *grandchild=cPOURYA_CreateObject();
                        memmove(&word[0], &word[1], strlen(word) - 0);
                        memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                        memmove(&word2[0], &word2[1], strlen(word2) - 0);
                        memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                        memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                        cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                        cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                    }
                    else
                    {
                        cPOURYA *grandchild=cPOURYA_CreateObject();
                        memmove(&word[0], &word[1], strlen(word) - 0);
                        memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                        memmove(&word2[0], &word2[1], strlen(word2) - 0);
                        memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                        cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                        word3=strtok(NULL,"{:,");
                        word4=strtok(NULL,"{:,");
                        if(word4[strlen(word4)-1]!=']')
                        {
                            flag=1;
                            break;
                        }
                        if(word4[strlen(word4)-1]!='}')
                        {
                            if(word4[strlen(word4)-1]!=']')
                            {
                                flag3=1;
                                break;
                            }
                        }
                        memmove(&word3[0], &word3[1], strlen(word3) - 0);
                        memmove(&word3[strlen(word3)-1], &word3[strlen(word3)], 1);
                        memmove(&word4[0], &word4[1], strlen(word4) - 0);
                        memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                        memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                        cPOURYA_AddItemToObject(grandchild,word3,cPOURYA_CreateString(word4));
                        cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                    }
                    word=strtok(NULL,"{:,");
                    word2=strtok(NULL,"{:,");
                }

                if(flag==1)
                {
                    flag=0;
                    cPOURYA *grandchild=cPOURYA_CreateObject();
                    cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                    memmove(&word3[0], &word3[1], strlen(word3) - 0);
                    memmove(&word3[strlen(word3)-1], &word3[strlen(word3)], 1);
                    memmove(&word4[0], &word4[1], strlen(word4) - 0);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    cPOURYA_AddItemToObject(grandchild,word3,cPOURYA_CreateString(word4));
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                }
                else if(flag2==1)
                {
                    cPOURYA *grandchild=cPOURYA_CreateObject();
                    memmove(&word[0], &word[1], strlen(word) - 0);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word2[0], &word2[1], strlen(word2) - 0);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                }
                else if(flag3==1)
                {
                    cPOURYA *grandchild=cPOURYA_CreateObject();
                    cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                    memmove(&word3[0], &word3[1], strlen(word3) - 0);
                    memmove(&word3[strlen(word3)-1], &word3[strlen(word3)], 1);
                    memmove(&word4[0], &word4[1], strlen(word4) - 0);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    memmove(&word4[strlen(word4)-1], &word4[strlen(word4)], 1);
                    cPOURYA_AddItemToObject(grandchild,word3,cPOURYA_CreateString(word4));
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                }
                else
                {
                    cPOURYA *grandchild=cPOURYA_CreateObject();
                    memmove(&word[0], &word[1], strlen(word) - 0);
                    memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
                    memmove(&word2[0], &word2[1], strlen(word2) - 0);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
                    cPOURYA_AddItemToObject(grandchild,word,cPOURYA_CreateString(word2));
                    cPOURYA_AddItemToArray(cPOURYA_GetObjectItem(result,save_word),grandchild);
                }
            }
        }
        if(flag2==1||flag3==1)
        {
            flag4=1;
            break;
        }
        word=strtok(NULL,"{:,");
        word2=strtok(NULL,"{:,");

    }
    if(flag4!=1)
    {
        memmove(&word[0], &word[1], strlen(word) - 0);
        memmove(&word[strlen(word)-1], &word[strlen(word)], 1);
        memmove(&word2[0], &word2[1], strlen(word2) - 0);
        memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
        memmove(&word2[strlen(word2)-1], &word2[strlen(word2)], 1);
        cPOURYA_AddItemToObject(result,word,cPOURYA_CreateString(word2));
    }
    return result;
}


void cPOURYA_Delete(cPOURYA *item)
{
    cPOURYA *fake;
    while (item)
    {
        fake=item->next;
        if (!(item->type&cPOURYA_IsReference) && item->child)
            cPOURYA_Delete(item->child);
        if (!(item->type&cPOURYA_IsReference) && item->valuestring)
            free(item->valuestring);
        if (!(item->type&cPOURYA_StringIsConst) && item->string)
            free(item->string);
        free(item);
        item=fake;
    }
}


cPOURYA *cPOURYA_DetachItemFromArray(cPOURYA *array,int item)
{
    cPOURYA *fake=array->child;
    while (fake && item>0)
    {
        fake=fake->next;
        item--;
    }
    if (!fake)
        return 0;
    if (fake->prev)
        fake->prev->next=fake->next;
    if (fake->next)
        fake->next->prev=fake->prev;
    if (fake==array->child)
        array->child=fake->next;
    fake->prev=fake->next=0;
    return fake;
}



void cPOURYA_DeleteItemFromArray(cPOURYA *array,int item)
{
    cPOURYA_Delete(cPOURYA_DetachItemFromArray(array,item));
}



cPOURYA *cPOURYA_DetachItemFromObject(cPOURYA *object,const char *string)
{
    int i=0;
    cPOURYA *fake=object->child;
    while (fake && strcasecmp(fake->string,string))
    {
        i++;
        fake=fake->next;
    }
    if (fake)
        return cPOURYA_DetachItemFromArray(object,i);
    return 0;
}


void cPOURYA_DeleteItemFromObject(cPOURYA *object,const char *string)
{
    cPOURYA_Delete(cPOURYA_DetachItemFromObject(object,string));
}


