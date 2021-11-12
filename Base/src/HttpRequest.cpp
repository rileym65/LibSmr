#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "SmrFramework.h"

using namespace SmrFramework;

HttpRequest::HttpRequest(const char* uri) {
int i;
  char    temp[2048];
  char    buffer[2048];
  Boolean flag;
  UInt32 pos;
  pos = 0;
  body = NULL;
  flag = true;
  while (flag) {
    if (*uri == 0) flag = false;
    else if (*uri == '/' && *(uri+1) == '/') {
      temp[pos++] = *uri++;
      temp[pos++] = *uri++;
      }
    else if (*uri == '/') {
      flag = false;
      }
    else {
      temp[pos++] = *uri++;
      }
    }
  temp[pos] = 0;
  if (pos == 0) strcpy(temp,"http://localhost");
  if (strncasecmp(temp,"http://",7) == 0) {
    port = 80;
    strcpy(buffer,temp+7);
    strcpy(temp,buffer);
    }
  if (strncasecmp(temp,"https://",8) == 0) {
    port = 443;
    strcpy(buffer,temp+8);
    strcpy(temp,buffer);
    }
  if (*uri == '/') uri++;
  this->host = new String(temp);
  this->uri = new String(uri);
  requestHeaders = new List<String*>();
  requestHeaderValues = new List<String*>();
  responseHeaders = new List<String*>();
  responseHeaderValues = new List<String*>();
  method = POST;
  protocol = NULL;
  }

HttpRequest::~HttpRequest() {
  int i;
  delete(host);
  delete(uri);
  for (i=0; i<requestHeaders->Count(); i++)
    delete(requestHeaders->At(i));
  for (i=0; i<requestHeaderValues->Count(); i++)
    delete(requestHeaderValues->At(i));
  delete requestHeaders;
  delete requestHeaderValues;
  if (protocol != NULL) delete(protocol);
  for (i=0; i<responseHeaders->Count(); i++)
    delete(responseHeaders->At(i));
  for (i=0; i<responseHeaderValues->Count(); i++)
    delete(responseHeaderValues->At(i));
  delete responseHeaders;
  delete responseHeaderValues;
  if (body != NULL) delete(body);
  }

char* HttpRequest::_readLine(char* input, char* buffer) {
  Boolean flag;
  flag = true;
  while (flag) {
    if (*input == 0) flag = false;
    else if (*input == 10) {
      flag = false;
      input++;
      if (*input == 13) input++;
      }
    else if (*input == 13) {
      flag = false;
      input++;
      if (*input == 10) input++;
      }
    else *buffer++ = *input++;
    }
  *buffer = 0;
  return input;
  }

void HttpRequest::AddRequestHeader(const char* key, const char* value) {
  requestHeaders->Add(new String(key));
  requestHeaderValues->Add(new String(value));
  }
 
Byte HttpRequest::Method() {
  return method;
  }

Byte HttpRequest::Method(Byte i) {
  method = i;
  return method;
  }

int HttpRequest::Send(const char* message) {
  int     i;
  int     sock;
  int     status;
  Socket *client;
  char    msg[65536];
  char    line[65536];
  char    content[65536];
  int     size;
  char   *mpos;
  char   *lpos;
  int     pos;
  Boolean flag;
  Boolean chunked;
  char buffer[256];
  struct hostent *host_ptr;
  struct sockaddr_in addr;
  if (body != NULL) {
    delete(body);
    body = NULL;
    }
  if (method == POST) strcpy(msg, "POST /");
    else strcpy(msg, "GET /");
  strcat(msg, uri->AsCharArray());
  strcat(msg, " HTTP/1.1\r\n");
  strcat(msg, "Host: ");
  strcat(msg, host->AsCharArray());
  strcat(msg, "\r\n");
  sprintf(buffer,"Content-Length: %d\r\n",strlen(message));
  strcat(msg, buffer);
  for (i=0; i<requestHeaders->Count(); i++) {
    strcat(msg, requestHeaders->At(i)->AsCharArray());
    strcat(msg, ": ");
    strcat(msg, requestHeaderValues->At(i)->AsCharArray());
    strcat(msg, "\r\n");
    }
  strcat(msg, "Accept: */*\r\n");
  strcat(msg, "Accept-Encoding: identity\r\n");
  strcat(msg, "\r\n");
  strcat(msg, message);
printf("%s\n",msg);
printf("--------------------------------------\n");

  client = new Socket();
  client->Connect(host->AsCharArray(), port);
  client->Send((Byte*)msg, strlen(msg));
  status = client->Receive((Byte*)msg, 65535);

msg[status] = 0;
printf("result = %d\n",status);
printf("--------------------------------------\n");
printf("%s\n",msg);

  client->Close();
  delete(client);

  mpos = msg;
  if (*mpos == 0) {
    httpResult = 400;
    return httpResult;
    }
  mpos = _readLine(mpos, line);
  if (strlen(line) == 0) {
    httpResult = 400;
    return httpResult;
    }
  pos = 0;
printf("-->%s\n",line);
  lpos = line;
  while (*lpos != 0 && *lpos >' ') buffer[pos++] = *lpos++;
  buffer[pos] = 0;
  protocol = new String(buffer);
  pos = 0;
  lpos++;
  while (*lpos != 0 && *lpos > ' ') buffer[pos++] = *lpos++;
  buffer[pos] = 0;
  httpResult = atoi(buffer);
  
printf("protocol: %s\n",protocol->AsCharArray());
printf("Result  : %d\n",httpResult);

  chunked = false;
  mpos = _readLine(mpos, line);
  while (strlen(line) > 0) {
    lpos = line;
    pos = 0;
    while (*lpos > 0 && *lpos <= ' ') lpos++;
    while (*lpos != 0 && *lpos != ':') buffer[pos++] = *lpos++;
    buffer[pos] = 0;
    responseHeaders->Add(new String(buffer));
    if (*lpos != ':') {
      httpResult = 400;
      return httpResult;
      }
    lpos++;
    while (*lpos != 0 && *lpos <= ' ') lpos++;
    if (strncasecmp(lpos,"chunked", 7) == 0) chunked = true;
    responseHeaderValues->Add(new String(lpos));
    mpos = _readLine(mpos, line);
    }

  if (*mpos == 0) {
    httpResult = 400;
    return httpResult;
    }

  lpos = mpos;
  if (chunked) printf("Data is chunked\n"); else printf("Data is NOT chunked\n");
  if (chunked) {
    flag = true;
    pos = 0;
    while (flag) {
      size = 0;
      while (*mpos != 0 && *mpos != 10 && *mpos != 13) {
        if (*mpos >= '0' && *mpos <= '9') size = (size << 4) + (*mpos - '0');
        if (*mpos >= 'a' && *mpos <= 'f') size = (size << 4) + (*mpos - 87);
        if (*mpos >= 'A' && *mpos <= 'F') size = (size << 4) + (*mpos - 55);
        mpos++;
        }
      if (*mpos == 10 || *mpos == 13) mpos += 2;
      else {
        httpResult = 400;
        return httpResult;
        }
      if (size == 0) flag = false;
      else {
        for (i=0; i<size; i++) {
          content[pos++] = *mpos++;
          }
        mpos += 2;
        }
      }
    content[pos] = 0;
    }
  else {
    }
  body = new String(content);
  return httpResult;
  }

String* HttpRequest::Body() {
  return body;
  }

UInt32 HttpRequest::Result() {
  return httpResult;
  }

