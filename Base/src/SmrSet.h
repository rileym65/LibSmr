
  template <class T>
  Set<T>::Set() : List<T>()  {
    this->objectType = (char*)"Set";
    }
  
  template <class T>
  Set<T>::~Set() {
    }
  
  template <class T>
  void Set<T>::Add(T o) {
    if (this->Contains(o)) return;
    List<T>::Add(o);
    }

  template <class T>
  Set<T>* Set<T>::Intersect(Set<T>* b) {
    UInt32 i;
    UInt32 j;
    UInt32 count1;
    UInt32 count2;
    Set<T>* ret;
    ret = new Set<T>();
    for (i=0; i<this->count; i++) {
      count1 = this->Occurances(this->items[i]);
      count2 = b->Occurances(this->items[i]);
      count1 = Math::Min(count1, count2);
      for (j=0; j<count1; j++) ret->Add(this->items[i]);
      }
    return ret;
    }


  template <class T>
  Set<T>* Set<T>::Union(Set<T>* b) {
    UInt32 i;
    Set<T>* ret;
    ret = new Set<T>();
    for (i=0; i<this->count; i++) ret->Add(this->items[i]);
    for (i=0; i<b->Count(); i++) ret->Add(b->At(i));
    return ret;
    }

