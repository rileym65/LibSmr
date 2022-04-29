/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <poll.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Socket::Socket(Int32 sockdes, Word family, Word port, UInt32 address) {
    objectType = (char*)"Socket";
    addressFamily = family;
    sock = sockdes;
    protocol = 0;
    socketType = ST_Stream;
    this->port = port;
    connected = true;
    }

  Socket::Socket() {
    objectType = (char*)"Socket";
    addressFamily = AF_Inet;
    socketType = ST_Stream;
    protocol = 0;
    port = 0;
    connected = false;
    sock = socket(addressFamily, socketType, 0);
    if (sock < 0) throw InvalidOpException(this, "Could not create socket");
    }
  
  Socket::~Socket() {
    if (sock >= 0) close(sock);
    }
  
  Socket* Socket::Accept() {
    Int32 client_sock;
    struct sockaddr_in client;
    socklen_t len;
    len = sizeof(client);
    client_sock = accept(sock, (struct sockaddr*)&client, &len);
    if (client_sock < 0) throw InvalidOpException(this, "Accept failed");
    return new Socket(client_sock,
                      (Word)client.sin_family,
                      (Word)client.sin_port,
                      (UInt32)client.sin_addr.s_addr);
    }

  UInt32 Socket::AddressFamily() {
    return addressFamily;
    }

  void   Socket::Bind(Word port) {
    char            hostname[1024];
    struct utsname  sysname;
    struct hostent *host_ptr;
    struct sockaddr_in server_name;

    if (uname(&sysname) < 0) throw InvalidOpException(this, "Could not obtain host name");
    strcpy(hostname, sysname.nodename);
    host_ptr = gethostbyname(hostname);
    if (host_ptr == NULL) throw InvalidOpException(this, "Could not obtain host name");
    memset(&server_name, 0, sizeof(server_name));
    memcpy(&server_name.sin_addr, host_ptr->h_addr, host_ptr->h_length);
    server_name.sin_addr.s_addr = htonl(INADDR_ANY);
    server_name.sin_family = addressFamily;
    server_name.sin_port = htons(port);
    if (bind(sock, (struct sockaddr*)&server_name, sizeof(server_name)) < 0)
      throw InvalidOpException(this, "Failed to bind to address");
    }

  void   Socket::Bind(const char* address, Word port) {
    char            hostname[1024];
    struct utsname  sysname;
    struct hostent *host_ptr;
    struct sockaddr_in server_name;
    struct in_addr  addr;
    inet_aton(address,&addr);
    if (uname(&sysname) < 0) throw InvalidOpException(this, "Could not obtain host name");
    strcpy(hostname, sysname.nodename);
    host_ptr = gethostbyname(hostname);
    if (host_ptr == NULL) throw InvalidOpException(this, "Could not obtain host name");
    memset(&server_name, 0, sizeof(server_name));
    memcpy(&server_name.sin_addr, host_ptr->h_addr, host_ptr->h_length);
    server_name.sin_addr.s_addr = htonl(addr.s_addr);
    server_name.sin_family = addressFamily;
    server_name.sin_port = htons(port);
    if (bind(sock, (struct sockaddr*)&server_name, sizeof(server_name)) < 0)
      throw InvalidOpException(this, "Failed to bind to address");
    }

  void Socket::Close() {
    if (sock < 0) throw InvalidOpException(this, "Socket already closed");
    close(sock);
    sock = -1;
    connected = false;
    }

  void Socket::Connect(const char* hostname, Word port) {
    struct hostent    *host_ptr;
    struct sockaddr_in svc_name;
    int                status;
    host_ptr = gethostbyname(hostname);
    if (host_ptr == NULL) {
      host_ptr = gethostbyaddr(hostname, strlen(hostname),  addressFamily);
      if (host_ptr == NULL) throw InvalidOpException(this, "Failed to resolve host");
      }
    svc_name.sin_family = addressFamily;
    svc_name.sin_port = htons(port);
    memcpy(&svc_name.sin_addr, host_ptr->h_addr, host_ptr->h_length);
    status = connect(sock, (struct sockaddr*)&svc_name, sizeof(svc_name));
printf("Socket status: %d\n",status);
    if (status < 0) throw InvalidOpException(this, "Failed to connect");
    this->port = ntohs(svc_name.sin_port);
    connected = true;
    }

  Boolean Socket::Connected() {
    return connected;
    }

  Boolean Socket::DataAvailable(Int32 waitms) {
    struct pollfd pdata;
    pdata.fd = sock;
    pdata.events = POLLIN;
    if (poll(&pdata, 1, waitms) <= 0) return false;
    return true;
    }

  Int32  Socket::Linger() {
    struct linger ling;
    socklen_t     size;
    size = sizeof(ling);
    if (getsockopt(sock, SOL_SOCKET, SO_LINGER,(char *)&ling, &size) < 0)
      throw InvalidOpException(this, "Failed to Get socket option");
    if (ling.l_onoff == 0) return -1;
    return ling.l_linger;
    }

  Int32  Socket::Linger(bool onoff, Int32 seconds) {
    struct linger linger;
    linger.l_onoff = (onoff) ? -1 : 0;
    linger.l_linger = seconds;
    if (setsockopt(sock, SOL_SOCKET, SO_LINGER,(const char *)&linger, sizeof(linger)) < 0)
      throw InvalidOpException(this, "Failed to set socket option");
    return Linger();
    }

  void   Socket::Listen() {
    Listen(5);
    }

  void   Socket::Listen(Int32 backlog) {
    if (listen(sock, backlog) < 0) throw InvalidOpException(this, "Failed to enter listening mode");
    }

  Word   Socket::Port() {
    return port;
    }

  bool   Socket::ReuseAddress() {
    int on;
    socklen_t size;
    size = sizeof(on);
    if (getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, &size) < 0)
      throw InvalidOpException(this, "Failed to get socket option");
    return (size) ? true : false;
    }

  bool   Socket::ReuseAddress(bool state) {
    int on;
    on = (state) ? -1 : 0;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,(const char *)&on, sizeof(on)) < 0)
      throw InvalidOpException(this, "Failed to set socket option");

    return ReuseAddress();
    }

  UInt32 Socket::SocketType() {
    return socketType;
    }

  Int32   Socket::Receive(Byte* buffer, UInt32 maxlen) {
    Int32 status;
    status = recv(sock, buffer, maxlen, 0);
    if (status == 0) {
      close(sock);
      sock = -1;
      connected = false;
      }
    if (status < 0) {
      close(sock);
      sock = -1;
      connected = false;
      throw InvalidOpException(this, "Failed to read socket");
      }
    return status;
    }

  Int32   Socket::Receive(Byte* buffer, UInt32 maxlen, Int32 maxms) {
    Int32 status;
    struct pollfd pdata;
    pdata.fd = sock;
    pdata.events = POLLIN;
    if (!connected) throw InvalidOpException(this, "Socket not connected");
    if (poll(&pdata, 1, maxms) <= 0) return 0;
    status = recv(sock, buffer, maxlen, MSG_NOSIGNAL);
    if (status == 0) {
      close(sock);
      sock = -1;
      connected = false;
      }
    if (status < 0) {
      close(sock);
      sock = -1;
      connected = false;
      throw InvalidOpException(this, "Failed to read socket");
      }
    return status;
    }

  Int32   Socket::Send(const Byte* buffer, UInt32 len) {
    Int32 status;
    if (!connected) throw InvalidOpException(this, "Socket not connected");
    status = send(sock, buffer, len, MSG_NOSIGNAL);
    if (status == 0) {
      close(sock);
      sock = -1;
      connected = false;
      }
    if (status < 0) {
      close(sock);
      sock = -1;
      connected = false;
      throw InvalidOpException(this, "Failed to write socket");
      }
    return status;
    }



  }

