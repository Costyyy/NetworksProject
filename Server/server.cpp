/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include "server.h"

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
  bool dc = false;
  do
  {
    dc = false;
    temp = connectClient((struct thData *)arg);

    //process the client's requests
    if (temp != nullptr)
      dc = processClient((struct thData *)arg, temp);
  }while(dc);

  close((intptr_t)arg);
  printf("Disconnected\n");
  return (NULL);
};

//get data about the championships from the database
int callback_update(void *data, int argc, char **argv, char **azColName)
{
  int i;
  struct GameData *gData = (struct GameData *)data;
  char stmt[128];

  //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  (gData->gameId)[(gData->count)] = atoi(argv[1]);
  strcpy((gData->user)[(gData->count)++], argv[0]);
  return 0;
}

bool sendUpdate(void *arg0, void *arg1)
{
  thData tdL = *((struct thData *)arg0);
  userInfo *uAcc = (struct userInfo *)arg1;
  GameData *gData = new GameData;
  sqlite3_exec(tdL.db, "select username, game_id from championships natural join accounts;", callback_update, gData, nullptr);
  if (write(tdL.cl, gData, sizeof(GameData)) <= 0)
  {
    printf("error\n");
    return true;
  }
  return false;
}

bool createEntry(void *arg0, void *arg1)
{
  thData tdL = *((struct thData *)arg0);
  userInfo *uAcc = (struct userInfo *)arg1;
  int gameId;
  if ((read(tdL.cl, &gameId, sizeof(int))) <= 0)
  {
    return true;
  }
  char stmt[128];
  sprintf(stmt, "insert into championships values (%d,%d);", uAcc->id, gameId);
  sqlite3_exec(tdL.db, stmt, nullptr, nullptr, nullptr);
}

bool processClient(void *arg, void *userData)
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
      return false;
    }
    switch (req)
    {
    case 0:
      uAcc->valid = false;
      return true;
      break;
    case 1:
      printf("Message received\n");
      break;
    case 2:
      done = sendUpdate(arg, userData);
      break;
    case 3:
      done = createEntry(arg, userData);
      break;
    }
  }
}

static int callback(void *user, int argc, char **argv, char **azColName)
{
  userInfo *data = (userInfo *)user;
  data->id = atoi(argv[2]);
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
  thData tdL;
  userInfo *uAcc = new userInfo;
  tempInfo *tempAcc = new tempInfo;
  tdL = *((struct thData *)arg);

  if (read(tdL.cl, tempAcc, sizeof(tempInfo)) <= 0)
  {
    printf("[Thread %d]\n", tdL.idThread);
    perror("Eroare la read() de la client.\n");
    return nullptr;
  }
  uAcc->username = tempAcc->username;
  uAcc->password = tempAcc->password;
  std::cout << tempAcc->username << '\n';
  std::cout << tempAcc->password << '\n';
  char stmt[512];
  int res;
  char *zErrMsg = 0;
  sprintf(stmt, "select username, password, id from accounts where username = '%s';", (uAcc->username).c_str());
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
}
