
  template <class T>
  List<T>::List() {
    objectType = (char*)"List";
    items = NULL;
    count = 0;
    pos = 0;
    comparer = NULL;
    }
  
  template <class T>
  List<T>::~List() {
    Clear();
    }
  
  template <class T>
  T& List<T>::operator [] (UInt32 index) {
    if (index >= count) throw RangeException("Index out of range");
    return items[index];
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
  T List<T>::At(UInt32 n) {
    if (n >= count) throw RangeException("Index out of range");
    return items[n];
    }

  template <class T>
  int List<T>::AtEnd() {
    return (pos >= count) ? -1 : 0;
    }

  template <class T>
  void List<T>::AtPut(UInt32 n, T o) {
    if (n >= count) throw RangeException("Index out of range");
    items[n] = o;
    }

  template <class T>
  void List<T>::Clear() {
    if (count > 0 && items != NULL) {
      free(items);
      }
    count = 0;
    }

  template <class T>
  int List<T>::Contains(T o) {
    UInt32 i;
    for (i=0; i<count; i++)
      if (items[i] == o) return -1;
    return 0;
    }

  template <class T>
  UInt32 List<T>::Count() {
    return count;
    }

  template <class T>
  void List<T>::Delete(T o) {
    int pos;
    while ((pos = IndexOf(o)) >= 0) {
      RemoveAt(pos);
      }
    }

  template <class T>
  T List<T>::First() {
    pos = 0;
    return Next();
    }

  template <class T>
  int List<T>::IndexOf(T o) {
    UInt32 i;
    for (i=0; i<count; i++)
      if (items[i] == o) return i;
    return -1;
    }

  template <class T>
  void List<T>::InsertAt(UInt32 pos, T o) {
    UInt32 i;
    while (count <= pos) Add((T)0);
    Add((T)0);
    for (i=count-1; i>pos; i--) items[i] = items[i-1];
    items[pos] = o;
    }

  template <class T>
  List<T>* List<T>::Intersect(List<T>* b) {
    UInt32 i;
    UInt32 j;
    UInt32 count1;
    UInt32 count2;
    List<T>* ret;
    ret = new List<T>();
    for (i=0; i<count; i++) {
      count1 = Occurances(items[i]);
      count2 = b->Occurances(items[i]);
      count1 = Math::Min(count1, count2);
      for (j=0; j<count1; j++) ret->Add(items[i]);
      }
    return ret;
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
    if (pos >= count) return (T)0;
    return items[pos++];
    }

  template <class T>
  UInt32 List<T>::Occurances(T o) {
    UInt32 i;
    UInt32 count;
    count = 0;
    for (i=0; i<count; i++)
      if (items[i] == o) count++;
    return count;
    }

  template <class T>
  void List<T>::Remove(T o) {
    int pos;
    pos = IndexOf(o);
    while (pos >= 0) {
      RemoveAt(pos);
      pos = IndexOf(o);
      }
    }

  template <class T>
  void List<T>::RemoveAt(UInt32 pos) {
    UInt32 i;
    if (pos >= count) return;
    for (i=pos; i<count-1; i++)
      items[i] = items[i+1];
    count--;
    if (count == 0) {
      free(items);
      items = NULL;
      }
    else items = (T*)realloc(items,sizeof(T) * count);
    }

  template <class T>
  List<T>* List<T>::Union(List<T>* b) {
    UInt32 i;
    List<T>* ret;
    ret = new List<T>();
    for (i=0; i<count; i++) ret->Add(items[i]);
    for (i=0; i<b->Count(); i++) ret->Add(b->At(i));
    return ret;
    }

  template <class T>
  void List<T>::Comparer(Int32 (*func)(T,T)) {
    comparer = func;
    }

  template <class T>
  void List<T>::Sort() {
    Boolean flag;
    T       tmp;
    UInt32  i;
    if (comparer == NULL) return;
    flag = true;
    while (flag) {
      flag = false;
      for (i=0; i<count-1; i++)
        if (comparer(items[i],items[i+1]) > 0) {
          flag = true;
          tmp = items[i];
          items[i] = items[i+1];
          items[i+1] = tmp;
          }
      }
    }

