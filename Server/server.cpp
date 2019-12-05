/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

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
  std::string username;
  std::string password;
  bool valid = false;
};

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
userInfo *connectClient(void *);
void processClient(void *, void *);
int read_line(FILE *fp, char *usr)
{
  char tmp[256] = {0};
  while (fp && fgets(tmp, sizeof(tmp), fp))
  {
    if (strstr(tmp, usr))
      return 1;
  }
  return 0;
}

int main()
{
  struct sockaddr_in server; // structura folosita de server
  struct sockaddr_in from;
  int nr; //mesajul primit de trimis la client
  int sd; //descriptorul de socket
  int pid;
  pthread_t th[100]; //Identificatorii thread-urilor care se vor crea
  int i = 0;
  sqlite3 *db;
  //open the database
  int rc = sqlite3_open("./accounts", &db);

  /* crearea unui socket */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }
  /* utilizarea optiunii SO_REUSEADDR */
  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  /* pregatirea structurilor de date */
  bzero(&server, sizeof(server));
  bzero(&from, sizeof(from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
  server.sin_family = AF_INET;
  /* acceptam orice adresa */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  /* utilizam un port utilizator */
  server.sin_port = htons(PORT);

  /* atasam socketul */
  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen(sd, 2) == -1)
  {
    perror("[server]Eroare la listen().\n");
    return errno;
  }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
  {
    int client;
    thData *td; //parametru functia executata de thread
    unsigned int length = sizeof(from);

    printf("[server]Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    // client= malloc(sizeof(int));
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
    {
      perror("[server]Eroare la accept().\n");
      continue;
    }

    /* s-a realizat conexiunea, se astepta mesajul */

    // int idThread; //id-ul threadului
    // int cl; //descriptorul intors de accept

    td = (struct thData *)malloc(sizeof(struct thData));
    td->idThread = i++;
    td->cl = client;
    td->db = db;

    pthread_create(&th[i], NULL, &treat, td);

  } //while
};

static void *treat(void *arg)
{
  struct thData tdL;
  tdL = *((struct thData *)arg);
  printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
  fflush(stdout);
  pthread_detach(pthread_self());
  userInfo *temp;
  temp = connectClient((struct thData *)arg);

  //process the client's requests
  processClient((struct thData *)arg, temp);
  close((intptr_t)arg);
  printf("Disconnected\n");
  return (NULL);
};

void processClient(void *arg, void *userData)
{
  thData tdL = *((struct thData *)arg);
  userInfo *uAcc = (struct userInfo *)userData;
  int req = -1;
  bool done = false;
  while (!done)
  {
    //read request
    if (read(tdL.cl, &req, sizeof(int)) <= 0)
    {
      printf("[Thread %d]\n", tdL.idThread);
      perror("Eroare la read() de la client.\n");
    }
    switch(req)
    {
      case 0:
      done = true;
      break;
      case 1:
      printf("Message received\n");
      break;
      case 2:
      sendUpdate();
      break;
      case 3:

      break;

    }


  }
}

static int callback(void *user, int argc, char **argv, char **azColName)
{
  userInfo *data = (userInfo *)user;
  if ((strcmp((data->username).c_str(), argv[0]) == 0) && (strcmp((data->password).c_str(), argv[1]) == 0))
  {
    data->valid = true;
  }
  else
  {
    data->valid = false;
  }
  return 0;
}

userInfo *connectClient(void *arg)
{
  int nr, i = 0;
  char info[100];
  thData tdL;
  userInfo *uAcc = new userInfo;
  tdL = *((struct thData *)arg);

  if (read(tdL.cl, info, 100) <= 0)
  {
    printf("[Thread %d]\n", tdL.idThread);
    perror("Eroare la read() de la client.\n");
  }
  std::string userAcc = info;
  std::size_t pos = userAcc.find(":");
  uAcc->username = userAcc.substr(0, pos);
  uAcc->password = userAcc.substr(pos + 1);
  std::cout << uAcc->username << '\n';
  std::cout << uAcc->password << '\n';
  char stmt[512];
  int res;
  char *zErrMsg = 0;
  sprintf(stmt, "select username, password from accounts where username = '%s';", (uAcc->username).c_str());
  sqlite3_exec(tdL.db, stmt, callback, (void *)uAcc, &zErrMsg);
  if (uAcc->valid)
  {
    res = 1;
    write(tdL.cl, &res, sizeof(int));
  }
  else
  {
    res = 0;
    write(tdL.cl, &res, sizeof(int));
  }

  return uAcc;
  /*pregatim mesajul de raspuns */
  //printf("[Thread %d]Trimitem mesajul inapoi...%d\n",tdL.idThread, nr);

  /* returnam mesajul clientului */
  /*if (write (tdL.cl, &nr, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	
*/
}
