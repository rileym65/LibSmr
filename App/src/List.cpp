/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include "SmrFramework.h"

namespace SmrFramework {

  template <class T>
  List<T>::List() {
    objectType = (char*)"List";
    items = NULL;
    deleteObjectsOnClear = -1;
    count = 0;
    pos = 0;
    }
  
  template <class T>
  List<T>::~List() {
    Clear();
    }
  
  template <class T>
  void List<T>::Add(T o) {
    count++;
    if (count == 1)
      items = (T*)malloc(sizeof(T));
    else
      items = (T*)realloc(items,sizeof(T) * count);
    items[count-1] = o;
    }

  template <class T>
  T List<T>::At(int n) {
    if (n < 0 || n >= count) return NULL;
    return items[n];
    }

  template <class T>
  int List<T>::AtEnd() {
    return (pos >= count) ? -1 : 0;
    }

  template <class T>
  void List<T>::AtPut(int n, T o) {
    if (n < 0) return;
    while (count <= n) Add(NULL);
    items[n] = o;
    }

  template <class T>
  void List<T>::Clear() {
    int i;
    T o;
    if (count > 0) {
      if (deleteObjectsOnClear) {
        while (count > 0) {
          o = items[0];
          Remove(o);
          delete(o);
          }
        }
      free(items);
      }
    count = 0;
    }

  template <class T>
  int List<T>::Contains(T o) {
    int i;
    for (i=0; i<count; i++)
      if (items[i] == o) return -1;
    return 0;
    }

  template <class T>
  int List<T>::Count() {
    return count;
    }

  template <class T>
  void List<T>::Delete(T o) {
    int i;
    int pos;
    while ((pos = IndexOf(o)) >= 0) {
      RemoveAt(pos);
      }
//    delete(o);
    }

  template <class T>
  void List<T>::DeleteObjectsOnClear(int b) {
    deleteObjectsOnClear = b;
    }

  template <class T>
  T List<T>::First() {
    pos = 0;
    return Next();
    }

  template <class T>
  int List<T>::IndexOf(T o) {
    int i;
    for (i=0; i<count; i++)
      if (items[i] == o) return i;
    return -1;
    }

  template <class T>
  void List<T>::InsertAt(int pos, T o) {
    int i;
    while (count <= pos) Add(NULL);
    Add(NULL);
    for (i=count-1; i>pos; i--) items[i] = items[i-1];
    items[pos] = o;
    }

  template <class T>
  int List<T>::LastIndexOf(T o) {
    int i;
    for (i=count-1; i>=0; i--)
      if (items[i] == o) return i;
    return -1;
    }

  template <class T>
  T List<T>::Next() {
    if (pos >= count) return NULL;
    return items[pos++];
    }

  template <class T>
  void List<T>::Remove(T o) {
    int i;
    int pos;
    pos = IndexOf(o);
    while (pos >= 0) {
      RemoveAt(pos);
      pos = IndexOf(o);
      }
    }

  template <class T>
  void List<T>::RemoveAt(int pos) {
    int i;
    if (pos < 0 || pos >= count) return;
    for (i=0; i<count-1; i++)
      items[i] = items[i+1];
    count--;
    if (count == 0) {
      free(items);
      items = NULL;
      }
    else items = (T*)realloc(items,sizeof(T) * count);
    }

  }
