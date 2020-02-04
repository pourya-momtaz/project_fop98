//In the name of God
//pourya momtaz <std id>:98106061
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <unistd.h>
#include <windows.h>
#include <dirent.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 100
#define PORT 12345
#define SA struct sockaddr


cJSON *tokens;
cJSON *channels;
char token[30];
int server_socket, client_socket;
int socket_making(int server_socket,struct sockaddr_in server);
void getting_packet();
void connect_client(struct sockaddr_in client);
void register_client(char *packet);
void login_client(char *packet);
void create_channel(char *packet);
void join_channel(char *packet);
void send_message(char *packet);
void refresh(char *packet);
void leave_channel(char *packet);
void logout(char *packet);
void channel_members(char *packet);
void search_members(char *packet);

int main()
{
    struct sockaddr_in server,client;
    server_socket=socket_making(server_socket,server);
    channels=cJSON_CreateObject();
    tokens=cJSON_CreateObject();
    do
    {
        connect_client(client);
        getting_packet();
    }
    while(1);
    return 0;
}

int socket_making(int server_socket,struct sockaddr_in server)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {

        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }


    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");


    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);


    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");
    return server_socket;
}

void connect_client(struct sockaddr_in client)
{

    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");


    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");
}

void getting_packet()
{
    char packet[2000],save_packet[2000];
    memset(packet, 0, sizeof(packet));

    recv(client_socket, packet, sizeof(packet), 0);
    strcpy(save_packet,packet);
    char *request = strtok (packet,", ");
    if(strcmp(request,"register")==0)
    {
        register_client(save_packet);
    }
    else if(strcmp(request,"login")==0)
    {
        login_client(save_packet);
    }
    else if(strcmp(request,"create")==0)
    {
        request = strtok (NULL, ", ");
        if(strcmp(request,"channel")==0)
        {
            create_channel(save_packet);
        }
    }
    else if(strcmp(request,"join")==0)
    {
        request = strtok (NULL, ", ");
        if(strcmp(request,"channel")==0)
        {
            join_channel(save_packet);
        }
    }
    else if(strcmp(request,"send")==0)
    {
        send_message(save_packet);
    }
    else if(strcmp(request,"refresh")==0)
    {
        refresh(save_packet);
    }
    else if(strcmp(request,"channel")==0)
    {
        request = strtok (NULL, ", ");
        if(strcmp(request,"members")==0)
        {
            channel_members(save_packet);
        }
    }
    else if(strcmp(request,"leave")==0)
    {
        leave_channel(save_packet);
    }
    else if(strcmp(request,"logout")==0)
    {
        logout(save_packet);
    }
    else if(strcmp(request,"search")==0)
    {
        request = strtok (NULL, ", ");
        if(strcmp(request,"members")==0)
        {
            search_members(save_packet);
        }
    }
}

void register_client(char *packet)
{
    char *request = strtok (packet,", \n");
    char *username= strtok (NULL, ", \n");
    char *password = strtok (NULL, ", \n");
    cJSON *info=cJSON_CreateObject();
    cJSON_AddItemToObject(info,"username",cJSON_CreateString(username));
    cJSON_AddItemToObject(info,"password",cJSON_CreateString(password));
    char *print=cJSON_Print(info);
    FILE *user;
    char address[1500]="./users/";
    char plus[10]=".json";
    strcat(username,plus);
    strcat(address,username);
    DIR* dir = opendir("users");
    if (dir)
    {
        // Directory exists.
        closedir(dir);
    }
    else
    {
        int check;
        char* dirname = "users";
        check = mkdir(dirname);
        // check if directory is created or not
        if (check)
        {
            printf("Unable to create directory\n");
            exit(1);
        }
    }
    user = fopen(address,"r");
    if(user==NULL)
    {
        user = fopen(address,"w");
        fprintf(user,"%s",print);
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
        print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    else
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("This username has been used before"));
        print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    fclose(user);
}

void login_client(char *packet)
{
    FILE *user;
    char *request = strtok (packet,", \n");
    char *username= strtok (NULL, ", \n");
    char *password = strtok (NULL, ", \n");
    char pass[1010]="\"";
    char save_username[1000];
    strcpy(save_username,username);
    printf("%s\n",username);
    strcat(pass,password);
    strcat(pass,"\"");
    char address[1500]="./users/";
    char plus[10]=".json";
    strcat(username,plus);
    strcat(address,username);
    user=fopen(address,"r");
    if(user==NULL)
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Username doesn't exist"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    else
    {
        int w=0;
        char c;
        char word[1000];
        while ((c = fgetc(user)) != EOF)       //countig no.of words
        {
            ungetc(c, user);
            fscanf(user, "%s", word);
            if (word[0] != ' ' || word[0] != '\n')
                w++;
            if(w==5)
                break;
        }
        if(strcmp(pass,word))
        {
            cJSON *response=cJSON_CreateObject();
            cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
            cJSON_AddItemToObject(response,"content",cJSON_CreateString("Wrong password"));
            char *print=cJSON_Print(response);
            printf("%s\n",print);
            send(client_socket, print, 1024, 0);
        }
        else
        {
            long long r;
            char charset[]= {"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ*-+=_&^%$#@!~"};
            int t=time(NULL);
            srand(t);
            for(int i=0; i<30; i++)
            {
                r=rand();
                token[i]=charset[(r%(sizeof(charset)))];
            }

            cJSON_AddItemToObject(tokens,token,cJSON_CreateString(save_username));
            cJSON *response=cJSON_CreateObject();
            cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response,"content",cJSON_CreateString(token));
            char *print=cJSON_Print(response);
            printf("%s\n",print);
            send(client_socket, print, 1024, 0);
        }
    }
    fclose(user);
}

void create_channel(char *packet)
{
    //ممکنه اسم کانل چند کلمه ای باشه
    FILE *channel;
    char *request = strtok (packet,", \n");
    request = strtok (NULL,", \n");
    char *channel_name= strtok (NULL, ",\n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    char save_channel_name[1000];
    strcpy(save_channel_name,channel_name);
    //printf("%s\n",request);
    //باید آرایه بسازیم
    char address[1500]="./channels/";
    char plus[10]=".json";
    strcat(channel_name,plus);
    strcat(address,channel_name);
    DIR* dir = opendir("channels");
    if (dir)
    {
        // Directory exists.
        closedir(dir);
    }
    else
    {
        int check;
        char* dirname = "channels";
        check = mkdir(dirname);
        // check if directory is created or not
        if (check)
        {
            printf("Unable to create directory\n");
            exit(1);
        }
    }
    channel = fopen(address,"r");
    if(channel==NULL)
    {
        channel = fopen(address,"w");

        char *save_username=cJSON_GetObjectItem(tokens,token)->valuestring;
        char *content=save_username;

        strcat(content," created ");
        strcat(content,save_channel_name);
        strcat(content," .");
        cJSON *channel_info=cJSON_CreateObject();
        cJSON *message=cJSON_CreateObject();
        cJSON_AddItemToObject(message,"sender",cJSON_CreateString("server"));
        cJSON_AddItemToObject(message,"content",cJSON_CreateString(content));
        cJSON_AddItemToObject(channel_info,"messages",cJSON_CreateArray());
        cJSON_AddItemToObject(channel_info,"name",cJSON_CreateString(save_channel_name));
        cJSON_AddItemToArray(cJSON_GetObjectItem(channel_info,"messages"),message);
        char *print=cJSON_Print(channel_info);
        fprintf(channel,"%s",print);
        save_username=strtok(save_username," ");

        cJSON_AddItemToObject(tokens,cJSON_GetObjectItem(tokens,token)->valuestring,cJSON_CreateString(save_channel_name));
        printf("%s\n",cJSON_GetObjectItem(tokens,token)->valuestring);
        printf("%s\n",save_channel_name);
        char *names[]= {cJSON_GetObjectItem(tokens,token)->valuestring};
        cJSON_AddItemToObject(channels,save_channel_name,cJSON_CreateStringArray(names,1));



        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
        print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    else
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("This channel name has been used before"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    fclose(channel);
}

void join_channel(char *packet)
{
    FILE *channel;
    char *request = strtok (packet,", \n");
    request = strtok (NULL,", \n");
    char *channel_name= strtok (NULL, ",\n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    char address[1500]="./channels/";
    char plus[10]=".json";
    strcat(channel_name,plus);
    strcat(address,channel_name);
    channel = fopen(address,"r");
    if(channel==NULL)
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Channel with this name doesn't exist"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    else
    {
        char *save_channel_name=strtok(channel_name,".");
        char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
        char save_username[1000];
        strcpy(save_username,username);
        printf("%s %s\n",save_channel_name,save_username);
        cJSON_AddItemToObject(tokens,save_username,cJSON_CreateString(save_channel_name));

        char *client_message=save_username;
        strcat(client_message," joined .");
        cJSON *message=cJSON_CreateObject();
        printf("%s\n",client_message);
        cJSON_AddItemToObject(message,"sender",cJSON_CreateString("server"));
        cJSON_AddItemToObject(message,"content",cJSON_CreateString(client_message));
        char channel_info[10000];
        fscanf(channel,"%[^EOF]",channel_info);
        fclose(channel);
        cJSON *content=cJSON_Parse(channel_info);
        cJSON_AddItemToArray(cJSON_GetObjectItem(content,"messages"),message);
        client_message=cJSON_Print(content);
        fopen(address,"w");
        fprintf(channel,"%s",client_message);
        fclose(channel);


        if(!(cJSON_GetObjectItem(channels,save_channel_name)))
        {
            char *names[]= {cJSON_GetObjectItem(tokens,token)->valuestring};
            cJSON_AddItemToObject(channels,save_channel_name,cJSON_CreateStringArray(names,1));
        }
        else
        {
            cJSON_AddItemToArray(cJSON_GetObjectItem(channels,save_channel_name),cJSON_CreateString(username));
        }
        printf("%s %s %s\n",token,username,save_channel_name);
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    fclose(channel);
}

void send_message(char *packet)
{
    FILE *channel;
    char *request = strtok (packet,", \n");
    char *message= strtok (NULL, ",\n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    cJSON *client_message=cJSON_CreateObject();
    char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
    char save_username[1000];
    strcpy(save_username,username);
    cJSON_AddItemToObject(client_message,"sender",cJSON_CreateString(save_username));
    cJSON_AddItemToObject(client_message,"content",cJSON_CreateString(message));
    char *channel_name=cJSON_GetObjectItem(tokens,save_username)->valuestring;

    char *save_channel_name=strtok(channel_name,".");
    printf("%s\n",save_channel_name);
    char address[1500]="./channels/";
    char plus[10]=".json";
    strcat(channel_name,plus);
    strcat(address,channel_name);
    channel = fopen(address,"r");

    char channel_info[10000];
    fscanf(channel,"%[^EOF]",channel_info);
    fclose(channel);
    cJSON *content=cJSON_Parse(channel_info);
    cJSON_AddItemToArray(cJSON_GetObjectItem(content,"messages"),client_message);
    char *print=cJSON_Print(content);
    channel = fopen(address,"w");
    fprintf(channel,"%s",print);
    fclose(channel);

    cJSON *response=cJSON_CreateObject();
    cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
    char *print2=cJSON_Print(response);
    printf("%s\n",print2);
    send(client_socket, print2, 1024, 0);
}

void refresh(char *packet)
{
    cJSON *response=cJSON_CreateObject();
    cJSON_AddItemToObject(response,"type",cJSON_CreateString("list"));
    cJSON_AddItemToObject(response,"content",cJSON_CreateArray());
    FILE *channel;
    char *request = strtok (packet,", \n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
    char *channel_name=cJSON_GetObjectItem(tokens,username)->valuestring;
    char save_username[1000];
    strcpy(save_username,username);
    //char save_channel_name[1000];
    char *save_channel_name=strtok(channel_name,".");
    char address[1500]="./channels/";
    char plus[10]=".json";
    strcat(channel_name,plus);
    strcat(address,channel_name);
    channel = fopen(address,"r");
    char channel_info[10000];
    fscanf(channel,"%[^']']",channel_info);
    fclose(channel);
    cJSON *content=cJSON_Parse(channel_info);


    cJSON_GetObjectItem(content,"messages")->string="content";
    cJSON_AddItemToObject(content,"type",cJSON_CreateString("List"));
    char *print=cJSON_Print(content);
    printf("%s\n",print);
    send(client_socket, print, 10000, 0);
    fclose(channel);
}

void channel_members(char *packet)
{
    char *request = strtok (packet,", \n");
    request = strtok (NULL,", \n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    cJSON *response=cJSON_CreateObject();
    cJSON_AddItemToObject(response,"type",cJSON_CreateString("list"));
    cJSON_AddItemToObject(response,"content",cJSON_CreateArray());
    char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
    char *save_username=strtok(username,".");
    char *channel_name=cJSON_GetObjectItem(tokens,username)->valuestring;
    char *save_channel_name=strtok(channel_name,".");

    if(!(cJSON_GetObjectItem(channels,"type")))
    {
        cJSON_AddItemToObject(channels,"type",cJSON_CreateString("List"));
    }
    cJSON_GetObjectItem(channels,save_channel_name)->string="content";
    char *print=cJSON_Print(channels);
    printf("%s\n",print);
    send(client_socket, print, 10000, 0);
    cJSON_GetObjectItem(channels,"content")->string=save_channel_name;
}

void leave_channel(char *packet)
{
    FILE *channel;
    char *request = strtok (packet,", \n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
    char *channel_name=cJSON_GetObjectItem(tokens,username)->valuestring;
    char address[1500]="./channels/";
    char plus[10]=".json";
    strcat(channel_name,plus);
    strcat(address,channel_name);
    printf("%s\n",address);

    channel = fopen(address,"r");
    //printf("%s\n",content);
    char *save_channel_name=strtok(channel_name,".");
    char *client_message=username;
    strcat(client_message," left .");
    cJSON *message=cJSON_CreateObject();
    cJSON_AddItemToObject(message,"sender",cJSON_CreateString("server"));
    cJSON_AddItemToObject(message,"content",cJSON_CreateString(client_message));
    char channel_info[10000];
    fscanf(channel,"%[^EOF]",channel_info);
    fclose(channel);
    cJSON *content=cJSON_Parse(channel_info);
    cJSON_AddItemToArray(cJSON_GetObjectItem(content,"messages"),message);
    client_message=cJSON_Print(content);
    fopen(address,"w");
    fprintf(channel,"%s",client_message);
    fclose(channel);
    char *save_username=strtok(username,".");


    cJSON_DeleteItemFromArray(cJSON_GetObjectItem(channels,save_channel_name),cJSON_GetArraySize(cJSON_GetObjectItem(channels,save_channel_name))-1);
    cJSON_DeleteItemFromObject(tokens,username);

    cJSON *response=cJSON_CreateObject();
    cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
    char *print=cJSON_Print(response);
    printf("%s\n",print);
    send(client_socket, print, 1024, 0);
}

void logout(char *packet)
{

    char *request = strtok (packet,", \n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
        return;
    }
    cJSON_DeleteItemFromObject(tokens,token);
    memset(token, 0, strlen(token));

    cJSON *response=cJSON_CreateObject();
    cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(response,"content",cJSON_CreateString(""));
    char *print=cJSON_Print(response);
    printf("%s\n",print);
    send(client_socket, print, 1024, 0);
}

void search_members(char *packet)
{
    int flag=0;
    char *request = strtok (packet,", \n");
    request = strtok (NULL,", \n");
    char *member_name= strtok (NULL, ",\n");
    char *auth_token = strtok (NULL, ", \n");
    if(strcmp(auth_token,token))
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("Authentication failed"));
        char *print=cJSON_Print(response);
        printf("%s\n",print);
        send(client_socket, print, 1024, 0);
    }
    char *username=cJSON_GetObjectItem(tokens,token)->valuestring;
    char *channel_name=cJSON_GetObjectItem(tokens,username)->valuestring;
    char *save_username=strtok(username,".");
    char *save_channel_name=strtok(channel_name,".");
    int i=0;
    char *name;
    while(i<cJSON_GetArraySize(cJSON_GetObjectItem(channels,save_channel_name)))
    {
        name=cJSON_GetArrayItem(cJSON_GetObjectItem(channels,save_channel_name),i)->valuestring;
        if(strcmp(member_name,name)==0)
        {
            cJSON *response=cJSON_CreateObject();
            cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
            cJSON_AddItemToObject(response,"content",cJSON_CreateString("True"));
            char *print=cJSON_Print(response);
            send(client_socket, print, 1024, 0);
            flag=1;
            break;
        }
        i++;
    }
    if(flag!=1)
    {
        cJSON *response=cJSON_CreateObject();
        cJSON_AddItemToObject(response,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(response,"content",cJSON_CreateString("False"));
        char *print=cJSON_Print(response);
        send(client_socket, print, 1024, 0);
    }
}


