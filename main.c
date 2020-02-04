//In the name of God
//pourya momtaz <std id>:98106061
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>
#include <unistd.h>
#include <windows.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 100
#define PORT 12345
#define SA struct sockaddr

char token[1000];
int client_socket,flag=0,flag2=0;
int socket_making(int client_socket,struct sockaddr_in (servaddr));
void connect_server(struct sockaddr_in (servaddr));
void account_menu();
void register_client(struct sockaddr_in (servaddr),int server_socket);
void login(struct sockaddr_in (servaddr),int server_socket);
void main_menu();
void create_channel(struct sockaddr_in (servaddr),int server_socket);
void join_channel(struct sockaddr_in (servaddr),int server_socket);
void logout(struct sockaddr_in (servaddr),int server_socket);
void chat_menu();
void chat(struct sockaddr_in (servaddr),int server_socket);
void refresh(struct sockaddr_in (servaddr),int server_socket);
void members(struct sockaddr_in (servaddr),int server_socket);
void leave(struct sockaddr_in (servaddr),int server_socket);
void SetColor(int ForgC);
void ClearConsoleToColors(int ForgC, int BackC);
void centralize(char* str);
void search_members(struct sockaddr_in (servaddr),int server_socket);

int main()
{
    ClearConsoleToColors(0,15);
    printf("Hello dear _USER_\n\nWelcome to ZATMOM chat room\n\nHere you can see the <account menu>\n\n");
    Sleep(3000);
    system("cls");
    account_menu();
    if(flag2==1)
    {
        SetColor(4);
        printf("\n***Thank you for using our chat room***\n");
        SetColor(0);
        return 0;
    }
    main_menu();
    if(flag==1)
    {
        account_menu();
        if(flag2==1)
        {
            SetColor(4);
            printf("\n***Thank you for using our chat room***\n");
            SetColor(0);
            return 0;
        }
        main_menu();
    }
    return 0;
}

void account_menu()
{
    printf("Account Menu:\n1:Register\n2:Login\n3:Exit\n\n\n");
    int server_socket;
    struct sockaddr_in servaddr;
    printf("Enter the number of service that you want:");
    int choice ;
    scanf("%d",&choice);
    if(choice==1)
        register_client(servaddr,server_socket);
    else if(choice==2)
        login(servaddr,server_socket);
    else if(choice==3)
    {
        flag2=1;
        return;
    }
}

void register_client(struct sockaddr_in (servaddr),int server_socket)
{
    int n=0;
    server_socket = socket_making(server_socket,servaddr);
    char username[1000],password[1000];
    memset(username, 0, strlen(username));
    memset(password, 0, strlen(password));
    printf("\nEnter Username:");
    getchar();
    scanf("%[^\n]%*c",username);
    n=0;
    printf("Enter Password:");
    while ((password[n++] = getchar()) != '\n');
    char format[2000]="register ";
    strcat(format,username);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,password);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        account_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to account menu");
        getchar();
        Sleep(1500);
        system("cls");
        account_menu();
    }
}

void login(struct sockaddr_in (servaddr),int server_socket)
{
    int n=0;
    server_socket = socket_making(server_socket,servaddr);
    char username[1000],password[1000];
    memset(username, 0, strlen(username));
    memset(password, 0, strlen(password));
    printf("\nEnter Username:");
    getchar();
    scanf("%[^\n]%*c",username);
    n=0;
    printf("Enter Password:");
    while ((password[n++] = getchar()) != '\n');
    char format[2000]="login ";
    strcat(format,username);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,password);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    strcpy(token,content);
    // Print buffer which contains the server message
    closesocket(client_socket);
    if(strcmp(type,"Error"))
    {
        printf("\nwelcome dear ");
        SetColor(4);
        printf("%s\n",username);
        SetColor(0);
        closesocket(client_socket);
        printf("\npress any key to see main menu\n");
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to account menu\n");
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
        account_menu();
    }
}

void connect_server(struct sockaddr_in (servaddr))
{
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
}


int socket_making(int server_socket,struct sockaddr_in (servaddr))
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
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    return client_socket;
}

void main_menu()
{
    printf("Main Menu:\n1:Create channel\n2:Join channel\n3:Logout\n\n");
    int server_socket;
    struct sockaddr_in servaddr;
    printf("Enter the number of service that you want:");
    int choice ;
    scanf("%d",&choice);
    if(choice==1)
        create_channel(servaddr,server_socket);
    else if(choice==2)
        join_channel(servaddr,server_socket);
    else if(choice==3)
        logout(servaddr,server_socket);

}

void create_channel(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char channelname[1000];
    memset(channelname, 0, strlen(channelname));
    printf("\nEnter the name of your channel:");
    getchar();
    scanf("%[^\n]%*c",channelname);
    char format[2000]="create channel ";
    strcat(format,channelname);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,token);
    char format3[2]="\n";
    strcat(format,format3);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        main_menu();
    }
}

void join_channel(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char channelname[1000];
    memset(channelname, 0, strlen(channelname));
    printf("\nEnter the name of channel you wanna join:");
    getchar();
    scanf("%[^\n]%*c",channelname);
    char format[2000]="join channel ";
    strcat(format,channelname);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,token);
    char format3[2]="\n";
    strcat(format,format3);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        main_menu();
    }
}

void logout(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char format[2000]="logout ";
    strcat(format,token);
    char format2[2]="\n";
    strcat(format,format2);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        flag=1;
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        main_menu();
    }
}

void chat_menu()
{
    printf("Main Menu:\n1:Send Message\n2:Refresh\n3:Channel Members\n4:Leave Channel\n5:Search Members\n\n");
    int server_socket;
    struct sockaddr_in servaddr;
    printf("Enter the number of service that you want:");
    int choice ;
    scanf("%d",&choice);
    if(choice==1)
        chat(servaddr,server_socket);
    else if(choice==2)
        refresh(servaddr,server_socket);
    else if(choice==3)
        members(servaddr,server_socket);
    else if(choice==4)
        leave(servaddr,server_socket);
    else if(choice==5)
        search_members(servaddr,server_socket);

}

void chat(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char message[100000];
    memset(message, 0, strlen(message));
    printf("\nEnter your message:");
    getchar();
    scanf("%[^\n]%*c",message);
    char format[102000]="send ";
    strcat(format,message);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,token);
    char format3[2]="\n";
    strcat(format,format3);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        Sleep(500);
        system("cls");
        closesocket(client_socket);
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        main_menu();
    }
}

void refresh(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    getchar();
    char format[1000]="refresh ";
    strcat(format,token);
    char format3[2]="\n";
    strcat(format,format3);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[100000]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 100000, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        for(int i=0; i<cJSON_GetArraySize(cJSON_GetObjectItem(pointer,"content")); i++)
        {
            cJSON* message=cJSON_GetArrayItem(cJSON_GetObjectItem(pointer,"content"),i);
            char* message_sender=cJSON_GetObjectItem(message,"sender")->valuestring;
            char* message_content=cJSON_GetObjectItem(message,"content")->valuestring;
            if(strcmp(message_sender,"server"))
            {
                SetColor(6);
                printf("\t\t\t%s : %s\n\n",message_sender,message_content);
            }
            else
            {
                SetColor(1);
                centralize(message_content);
                printf("%s\n",message_content);
            }
        }
        SetColor(0);
        printf("\npress any key to get back to chat menu\n");
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
        chat_menu();
    }
}

void members(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char format[500]="channel members ";
    strcat(format,token);
    char format2[2]="\n";
    strcat(format,format2);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[100000]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 100000, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        for(int i=0; i<cJSON_GetArraySize(cJSON_GetObjectItem(pointer,"content")); i++)
        {
            char* channel_member=cJSON_GetArrayItem(cJSON_GetObjectItem(pointer,"content"),i)->valuestring;
            printf("%d.%s\n",i+1,channel_member);
        }
        printf("\npress any key to get back to chat menu\n");
        getchar();
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
        closesocket(client_socket);
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to main menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        chat_menu();
    }
}

void leave(struct sockaddr_in (servaddr),int server_socket)
{
    server_socket = socket_making(server_socket,servaddr);
    char format[500]="leave ";
    strcat(format,token);
    char format2[2]="\n";
    strcat(format,format2);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        printf("\nLoading ...");
        Sleep(2000);
        system("cls");
        closesocket(client_socket);
        main_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to chat menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        chat_menu();
    }
}

void search_members(struct sockaddr_in (servaddr),int server_socket)
{
    char member_name[1000];
    memset(member_name, 0, strlen(member_name));
    printf("\nEnter the name of member you wanna look for:");
    getchar();
    scanf("%[^\n]%*c",member_name);
    server_socket = socket_making(server_socket,servaddr);
    char format[500]="search members ";
    strcat(format,member_name);
    char format2[3]=", ";
    strcat(format,format2);
    strcat(format,token);
    char format3[2]="\n";
    strcat(format,format3);
    connect_server(servaddr);
    send(server_socket, format, sizeof(format), 0);
    char response[1024]= {0};
    memset(response, 0, strlen(response));
    recv(server_socket, response, 1024, 0);
    cJSON* pointer = cJSON_Parse(response);
    char* type=cJSON_GetObjectItem(pointer,"type")->valuestring;
    char* content=cJSON_GetObjectItem(pointer,"content")->valuestring;
    if(strcmp(type,"Error"))
    {
        SetColor(4);
        printf("\n%s\n",content);
        SetColor(0);
        closesocket(client_socket);
        printf("\npress any key to see chat menu\n");
        getchar();
        printf("Loading ...");
        Sleep(1500);
        system("cls");
        chat_menu();
    }
    else
    {
        printf("\nThere is an error in your request : %s\n",content);
        closesocket(client_socket);
        printf("Press any key to get back to chat menu");
        getchar();
        printf("\nLoading ...");
        Sleep(1500);
        system("cls");
        chat_menu();
    }
}

void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void ClearConsoleToColors(int ForgC, int BackC)
 {
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 COORD coord = {0, 0};
  DWORD count;
 CONSOLE_SCREEN_BUFFER_INFO csbi;
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}

void centralize(char* str)
{
    for(int i=1;i<=(60-(strlen(str)/2));i++)
        printf(" ");
}
