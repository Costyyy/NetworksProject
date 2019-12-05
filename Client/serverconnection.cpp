#include "serverconnection.h"

serverConnection::serverConnection()
{
       port = 14821;
}
void serverConnection::connectTo(const char ip[])
{
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
    }
    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(ip);
    /* portul de conectare */
    server.sin_port = htons(port);
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
      {
        perror("[client]Eroare la connect().\n");
      }
}
