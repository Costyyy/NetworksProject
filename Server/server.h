#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
/* portul folosit */
#define PORT 14821

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData
{
  int idThread; //id-ul thread-ului tinut in evidenta de acest program
  int cl;       //descriptorul intors de accept
  int rc;       //database descriptor
  sqlite3 *db;

} thData;

struct userInfo
{
  int id;
  bool valid = false;
  std::string username;
  std::string password;
};

struct tempInfo
{
    char username[100];
    char password[100];
};

struct GameData
{
  char user[100][50];
  int gameId[50];
  int count = 0;
};

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
userInfo *connectClient(void *);
bool processClient(void *, void *);
bool sendUpdate(void *, void *);