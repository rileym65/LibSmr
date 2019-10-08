
  template <class T>
  Stack<T>::Stack() {
    this->objectType = (char*)"Stack";
    this->items = NULL;
    this->count = 0;
    this->pos = -1;
    }
  
  template <class T>
  Stack<T>::~Stack() {
    this->Clear();
    }
  
  template <class T>
  void Stack<T>::Clear() {
    if (this->count > 0) {
      free(this->items);
      }
    this->count = 0;
    }

  template <class T>
  T Stack<T>::At(int index) {
    if (index < 0) throw new InvalidOpException("Invalid index");
    if (index >= this->count) throw new InvalidOpException("Invalid index");
    return this->items[(this->count-1) - index];
    }

  template <class T>
  int Stack<T>::Contains(T value) {
    int i;
    for (i=0; i<this->count; i++)
      if (this->items[i] == value) return -1;
    return 0;
    }

  template <class T>
  int Stack<T>::Count() {
    return this->count;
    }

  template <class T>
  void Stack<T>::Dup() {
    if (this->count <= 0) throw new InvalidOpException("Stack Empty");
    this->Push(this->items[this->count - 1]);
    }

  template <class T>
  T Stack<T>::First() {
    if (this->count <= 0) throw new InvalidOpException("Stack Empty");
    this->pos = this->count - 1;
    return this->Next();
    }

  template <class T>
  T Stack<T>::Next() {
    if (this->pos < 0) throw new InvalidOpException("Past bottom");
    return this->items[this->pos--];
    }

  template <class T>
  T Stack<T>::Peek() {
    if (this->count <= 0) throw new InvalidOpException("Stack Empty");
    return this->items[count - 1];
    }

  template <class T>
  void Stack<T>::Push(T item) {
    this->count++;
    if (this->count == 1)
      this->items = (T*)malloc(sizeof(T));
    else
      this->items = (T*)realloc(this->items,sizeof(T) * this->count);
    this->items[count-1] = item;
    }

  template <class T>
  T Stack<T>::Pop() {
    T ret;
    if (this->count <= 0) throw new InvalidOpException("Stack Empty");
    ret = this->items[count-1];
    count--;
    if (count == 0) {
      free(items);
      items = NULL;
      }
    else
      this->items = (T*)realloc(this->items,sizeof(T) * this->count);
    return ret;
    }

  template <class T>
  void Stack<T>::Swap() {
    T temp;
    if (this->count <= 1) throw new InvalidOpException("Stack Empty");
    temp = this->items[this->count-2];
    this->items[this->count-2] = this->items[this->count-1];
    this->items[this->count-1] = temp;
    }


