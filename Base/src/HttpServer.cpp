#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "SmrFramework.h"

using namespace SmrFramework;

void ProcessEnded(int signum) {
  int status;
  while ((status = waitpid(-1,&status,WNOHANG)) > 0) {
    }
  }

HttpServer::HttpServer() {
  objectType = (char*)"HttpListener";
  body = NULL;
  protocol = NULL;
  resource = NULL;
  server = NULL;
  terminate = false;
  listener = NULL;
  getKeys = NULL;
  getValues = NULL;
  numGet = 0;
  postKeys = NULL;
  postValues = NULL;
  numPost = 0;
  headerKeys = NULL;
  headerValues = NULL;
  numHeaders = 0;
  rHeaderKeys = NULL;
  rHeaderValues = NULL;
  numRHeaders = 0;
  signal(SIGCHLD, &ProcessEnded);
  }

HttpServer::~HttpServer() {
  _clearData();
  if (server != NULL) delete(server);
  }

void HttpServer::_clearData() {
  UInt32 i;
  if (body != NULL) free(body);
  if (protocol != NULL) free(protocol);
  if (resource != NULL) free(resource);
  if (numGet > 0) {
    for (i=0; i<numGet; i++) {
      free(getKeys[i]);
      free(getValues[i]);
      }
    free(getKeys);
    free(getValues);
    }
  if (numPost > 0) {
    for (i=0; i<numPost; i++) {
      free(postKeys[i]);
      free(postValues[i]);
      }
    free(getKeys);
    free(getValues);
    }
  if (numHeaders > 0) {
    for (i=0; i<numHeaders; i++) {
      free(headerKeys[i]);
      free(headerValues[i]);
      }
    free(headerKeys);
    free(headerValues);
    }
  if (numRHeaders > 0) {
    for (i=0; i<numRHeaders; i++) {
      free(rHeaderKeys[i]);
      free(rHeaderValues[i]);
      }
    free(rHeaderKeys);
    free(rHeaderValues);
    }
  resource = NULL;
  protocol = NULL;
  body = NULL;
  numGet = 0;
  getKeys = NULL;
  getValues = NULL;
  numPost = 0;
  postKeys = NULL;
  postValues = NULL;
  numHeaders = 0;
  headerKeys = NULL;
  headerValues = NULL;
  numRHeaders = 0;
  rHeaderKeys = NULL;
  rHeaderValues = NULL;
  }

HttpServer::DICT HttpServer::_decodeParameters(Byte* message) {
  UInt32  i;
  UInt32  pos;
  UInt32  keyStart;
  UInt32  keyLength;
  UInt32  valueStart;
  UInt32  valueLength;
  char   *key;
  char   *value;
  DICT    result;
  result.keys = NULL;
  result.values = NULL;
  result.count = 0;
  pos = 0;
  while (message[pos] > ' ') {
    keyStart = pos;
    keyLength = 0;
    while (message[pos] > ' ' && message[pos] != '=') {
      if (message[pos] == '%') pos += 2;
      pos++;
      keyLength++;
      }
    if (message[pos] != '=') return result;
    pos++;
    valueStart = pos;
    valueLength = 0;
    while (message[pos] > ' ' && message[pos] != '&') {
      if (message[pos] == '%') pos += 2;
      pos++;
      valueLength++;
      }
    if (message[pos] == '&') pos++;
    if (keyLength > 0) {
      key = (char*)malloc(keyLength + 1);
      value = (char*)malloc(valueLength + 1);
      for (i=0; i<keyLength; i++) {
        if (message[keyStart] == '+') key[i] = ' ';
        else if (message[keyStart] == '%') {
          keyStart++;
          if (message[keyStart] >= '0' && message[keyStart] <= '9')
            key[i] = (message[keyStart] - '0') << 4;
          else if (message[keyStart] >= 'a' && message[keyStart] <= 'f')
            key[i] = (message[keyStart] - 87) << 4;
          else if (message[keyStart] >= 'A' && message[keyStart] <= 'F')
            key[i] = (message[keyStart] - 55) << 4;
          else key[i] = 0;
          keyStart++;
          if (message[keyStart] >= '0' && message[keyStart] <= '9')
            key[i] |= (message[keyStart] - '0');
          else if (message[keyStart] >= 'a' && message[keyStart] <= 'f')
            key[i] |= (message[keyStart] - 87);
          else if (message[keyStart] >= 'A' && message[keyStart] <= 'F')
            key[i] |= (message[keyStart] - 55);
          }
        else key[i] = message[keyStart];
        keyStart++;
        }
      key[keyLength] = 0;
      for (i=0; i<valueLength; i++) {
        if (message[valueStart] == '+') value[i] = ' ';
        else if (message[valueStart] == '%') {
          valueStart++;
          if (message[valueStart] >= '0' && message[valueStart] <= '9')
            value[i] = (message[valueStart] - '0') << 4;
          else if (message[valueStart] >= 'a' && message[valueStart] <= 'f')
            value[i] = (message[valueStart] - 87) << 4;
          else if (message[valueStart] >= 'A' && message[valueStart] <= 'F')
            value[i] = (message[valueStart] - 55) << 4;
          else value[i] = 0;
          valueStart++;
          if (message[valueStart] >= '0' && message[valueStart] <= '9')
            value[i] |= (message[valueStart] - '0');
          else if (message[valueStart] >= 'a' && message[valueStart] <= 'f')
            value[i] |= (message[valueStart] - 87);
          else if (message[valueStart] >= 'A' && message[valueStart] <= 'F')
            value[i] |= (message[valueStart] - 55);
          }
        else value[i] = message[valueStart];
        valueStart++;
        }
      value[valueLength] = 0;
      if (++result.count == 1) {
        result.keys   = (char**)malloc(sizeof(char*));
        result.values = (char**)malloc(sizeof(char*));
        }
      else {
        result.keys   = (char**)realloc(result.keys,sizeof(char*)*result.count);
        result.values = (char**)realloc(result.values,sizeof(char*)*result.count);
        }
      result.keys[result.count-1] = key;
      result.values[result.count-1] = value;
      }
    }
  return result;
  }

Int32 HttpServer::_parse(Byte* message, UInt32 length) {
  UInt32 pos;
  UInt32 start;
  char   key[1024];
  char   value[1024];
  DICT   dict;
  UInt32 p;
  _clearData();
  pos = 0;
  if (strncasecmp((char*)message,"GET",3) == 0) {
    strcpy(method, "GET");
    pos += 3;
    }
  else if (strncasecmp((char*)message,"POST",4) == 0) {
    strcpy(method, "POST");
    pos += 4;
    }
  else return -1;
  if (message[pos] != ' ') return -1;
  while (message[pos] == ' ') pos++;
  start = pos;
  while (message[pos] != 0 &&
         message[pos] > ' ' &&
         message[pos] != '?') pos++;
  if (start == pos) return -1;
  resource = (char*)malloc((pos-start) + 1);
  strncpy(resource, (char*)(message+start), pos-start);
  resource[pos-start] = 0;
  if (message[pos] == '?') {
    pos++;
    dict = _decodeParameters(message+pos);
    getKeys = dict.keys;
    getValues = dict.values;
    numGet = dict.count;
    while (message[pos] > ' ') pos++;
    }
  if (message[pos] == ' ') {
    while (message[pos] == ' ') pos++;
    start = pos;
    while (message[pos] > ' ') pos++;
    if (start == pos) return -1;
    protocol = (char*)malloc((pos-start) + 1);
    strncpy(protocol, (char*)(message+start), pos-start);
    protocol[pos-start] = 0;
    }
  if (message[pos] == 10 && message[pos+1] == 13) pos += 2;
  else if (message[pos] == 13 && message[pos+1] == 10) pos += 2;
  else if (message[pos] == 13 && message[pos+1] == 13) pos += 1;
  else if (message[pos] == 10 && message[pos+1] == 10) pos += 1;
  else return -1;

  while (pos < length &&
         message[pos] != 0 &&
         message[pos] != 10 &&
         message[pos] != 13) {
    p = 0;
    while (message[pos] > ' ' && message[pos] != ':')
      key[p++] = message[pos++];
    key[p] = 0;
    if (message[pos] == ':') pos++;
    while (message[pos] == ' ') pos++;
    p = 0;
    while (message[pos] >= ' ')
      value[p++] = message[pos++];
    value[p] = 0;
    if (strlen(key) > 0 && strlen(value) > 0) {
      if (++numHeaders == 1) {
        headerKeys = (char**)malloc(sizeof(char*));
        headerValues = (char**)malloc(sizeof(char*));
        }
      else {
        headerKeys = (char**)realloc(headerKeys, sizeof(char*) * numHeaders);
        headerValues = (char**)realloc(headerValues, sizeof(char*) * numHeaders);
        }
      headerKeys[numHeaders-1] = (char*)malloc(strlen(key) + 1);
      headerValues[numHeaders-1] = (char*)malloc(strlen(value) + 1);
      strcpy(headerKeys[numHeaders-1], key);
      strcpy(headerValues[numHeaders-1], value);
      }
    if (message[pos] == 10 && message[pos+1] == 13) pos += 2;
    else if (message[pos] == 13 && message[pos+1] == 10) pos += 2;
    else if (message[pos] == 13 && message[pos+1] == 13) pos += 1;
    else if (message[pos] == 10 && message[pos+1] == 10) pos += 1;
    }

  if (message[pos] == 10 && message[pos+1] == 13) pos += 2;
  else if (message[pos] == 13 && message[pos+1] == 10) pos += 2;
  else if (message[pos] == 13 && message[pos+1] == 13) pos += 1;
  else if (message[pos] == 10 && message[pos+1] == 10) pos += 1;
  if (length > pos) {
    body = (char*)malloc((length-pos) + 1);
    memcpy(body, message+pos, length-pos);
    body[length-pos] = 0;
    dict = _decodeParameters((Byte*)body);
    postKeys = dict.keys;
    postValues = dict.values;
    numPost = dict.count;
    }

  return 0;
  }

void HttpServer::AddResponseHeader(const char* key, const char* value) {
  if (++numRHeaders == 1) {
    rHeaderKeys = (char**)malloc(sizeof(char*));
    rHeaderValues = (char**)malloc(sizeof(char*));
    }
  else {
    rHeaderKeys = (char**)realloc(rHeaderKeys, sizeof(char*) * numRHeaders);
    rHeaderValues = (char**)realloc(rHeaderValues, sizeof(char*) * numRHeaders);
    }
  rHeaderKeys[numRHeaders-1] = (char*)malloc(strlen(key) + 1);
  rHeaderValues[numRHeaders-1] = (char*)malloc(strlen(value) + 1);
  strcpy(rHeaderKeys[numRHeaders-1], key);
  strcpy(rHeaderValues[numRHeaders-1], value);
  }

char* HttpServer::Body() {
  return body;
  }

char* HttpServer::Get(const char* key) {
  UInt32 i;
  for (i=0; i<numGet; i++)
    if (strcmp(key, getKeys[i]) == 0) return getValues[i];
  return NULL;
  }

char* HttpServer::Get(UInt32 pos) {
  if (pos >= numGet) return NULL;
  return getValues[pos];
  }

char* HttpServer::GetKey(UInt32 pos) {
  if (pos >= numGet) return NULL;
  return getKeys[pos];
  }

UInt32 HttpServer::GetCount() {
  return numGet;
  }

char* HttpServer::Header(const char* key) {
  UInt32 i;
  for (i=0; i<numHeaders; i++)
    if (strcmp(key, headerKeys[i]) == 0) return headerValues[i];
  return NULL;
  }

char* HttpServer::Header(UInt32 pos) {
  if (pos >= numHeaders) return NULL;
  return headerValues[pos];
  }

char* HttpServer::HeaderKey(UInt32 pos) {
  if (pos >= numHeaders) return NULL;
  return headerKeys[pos];
  }

UInt32 HttpServer::HeaderCount() {
  return numHeaders;
  }

void HttpServer::Listener(HttpListener* l) {
  listener = l;
  }

char* HttpServer::Method() {
  return method;
  }

UInt16 HttpServer::Port() {
  return port;
  }

UInt16 HttpServer::Port(UInt16 i) {
  port = i;
  return port;
  }

char* HttpServer::Post(const char* key) {
  UInt32 i;
  for (i=0; i<numPost; i++)
    if (strcmp(key, postKeys[i]) == 0) return postValues[i];
  return NULL;
  }

char* HttpServer::Post(UInt32 pos) {
  if (pos >= numPost) return NULL;
  return postValues[pos];
  }

char* HttpServer::PostKey(UInt32 pos) {
  if (pos >= numPost) return NULL;
  return postKeys[pos];
  }

UInt32 HttpServer::PostCount() {
  return numPost;
  }

char* HttpServer::Protocol() {
  return protocol;
  }

char* HttpServer::Resource() {
  return resource;
  }

void HttpServer::SetResponseCode(UInt32 code, const char* description) {
  responseCode = code;
  strcpy(responseDescription, description);
  }

void HttpServer::Start() {
  UInt32  i;
  int     size;
  int     length;
  Socket* client;
  Byte    line[1024];
  Byte    message[16384];
  Byte   *result;
  UInt32  resultLength;
  pid_t   pid;
  if (server != NULL) delete(server);
  server = new Socket();
  server->ReuseAddress(true);
  server->Linger(true, 30);
  server->Bind(port);
  server->Listen();
  client = NULL;
  terminate = false;
  while (!terminate) {
    while (client == NULL) {
      try {
        client = server->Accept();
        pid = fork();
        if (pid != 0) {
          delete(client);
          client = NULL;
          }
        }
      catch (...) {
        printf("Error during connection\n");
        client = NULL;
        }
      }
    while (client != NULL) {
      try {
        length = 0;
        do {
          size = client->Receive(line, 1024, 50);
          if (size > 0) {
            memcpy(message+length, line, size);
            length += size;
            }
          } while (size != 0);
        message[length] = 0;
        if (length > 0) {
          printf("Message size: %d\n",length);
          if(_parse(message, length) < 0) {
            printf("Parse error\n");
            printf("-----------------------------------------------------\n");
            printf("%s\n",message);
            printf("-----------------------------------------------------\n");
            }
          else {
            if (listener != NULL) {
              responseCode = 202;
              strcpy(responseDescription,"OK");
              result = listener->Request(this, &resultLength);
            printf("return -----------------------------------------------------\n");
              sprintf((char*)message,"HTTP/1.1 %d %s\r\n",responseCode,
                                                          responseDescription);
            printf("%s",message);
              client->Send(message, strlen((char*)message));
              for (i=0; i<numRHeaders; i++) {
                sprintf((char*)message,"%s: %s\r\n",rHeaderKeys[i], rHeaderValues[i]);
            printf("%s",message);
                client->Send(message, strlen((char*)message));
                }
              sprintf((char*)message,"Content-Length: %d\r\n\r\n",resultLength);
            printf("%s",message);
            printf("-----------------------------------------------------\n");
              client->Send(message, strlen((char*)message));
              if (result != NULL) {
                client->Send(result, resultLength);
                free(result);
                }
              }
            else {
              strcpy((char*)message,"HTTP/1.1 202 Accepted\r\n");
              strcat((char*)message,"Server: PiSmib\r\n");
              strcat((char*)message,"Content-Type: text/html; charset=UTF-8\r\n");
              strcat((char*)message,"Content-Length: 0");
              strcat((char*)message,"\r\n");
              client->Send(message, strlen((char*)message));
              }
            client->Close();
            delete(client);
            client = NULL;
            exit(0);
            }
          }
        }
      catch (Exception &e) {
        printf("Exception caught: %s\n",e.Message());
        delete(client);
        client = NULL;
        exit(0);
        }
      }
    }
  }

