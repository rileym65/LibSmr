  template <class T>
  Queue<T>::Queue() {
    this->objectType = (char*)"Queue";
    this->items = NULL;
    this->head  = 0;
    this->tail  = 0;
    this->maxItems = -1;
    this->overwrite = false;
    }

  template <class T>
  Queue<T>::Queue(Int32 max) {
    this->objectType = (char*)"Queue";
    this->maxItems = max + 1;
    this->items = (T*)malloc(sizeof(T) * this->maxItems);
    this->head  = 0;
    this->tail  = 0;
    this->overwrite = false;
    }

  template <class T>
  Queue<T>::~Queue() {
    if (this->items != NULL) free(this->items);
    }

  template <class T>
  bool Queue<T>::Add(T value) {
    if (maxItems < 0) {
      if (this->items != NULL)
        this->items = (T*)realloc(this->items, sizeof(T) * (this->tail + 1));
      else
        this->items = (T*)malloc(sizeof(T));
      this->items[tail++] = value;
      return true;
      }
    else {
      if (((tail+1) % maxItems) == head && !overwrite) 
        throw InvalidOpException("Queue full");
      items[tail++] = value;
      if (tail >= maxItems) tail = 0;
      if (tail == head) {
        head++;
        if (head >= maxItems) head = 0;
        }
      return true;
      }
    }

  template <class T>
  bool Queue<T>::AtEnd() {
    return (head == tail) ? true : false;
    }

  template <class T>
  void Queue<T>::Clear() {
    if (maxItems < 0) {
      if (items != NULL) free(items);
      }
    head = 0;
    tail = 0;
    }

  template <class T>
  Int32 Queue<T>::Count() {
    if (maxItems < 0) return tail;
    if (tail == head) {
      return 0;
      }
    if (tail > head) return (tail - head);
    return tail + (maxItems - head);
    }

  template <class T>
  bool Queue<T>::Empty() {
    return (head == tail) ? true : false;
    }

  template <class T>
  bool Queue<T>::Overwrite() {
    return overwrite;
    }

  template <class T>
  T Queue<T>::Next() {
    int i;
    T ret;
    if (maxItems < 0) {
      if (tail == 0) throw InvalidOpException("Empty queue");
      ret = items[0];
      for (i=0; i<tail-1; i++) items[i] = items[i+1];
      tail--;
      if (tail == 0) {
        free(items);
        }
      else {
        this->items = (T*)realloc(this->items, sizeof(T) * (this->tail));
        }
      }
    else {
      if (head == tail) throw InvalidOpException("Empty queue");
      ret = items[head++];
      if (head >= maxItems) head = 0;
      }
    return ret;
    }

  template <class T>
  bool Queue<T>::Overwrite(bool b) {
    overwrite = b;
    return overwrite;
    }

  template <class T>
  T Queue<T>::Peek() {
    int i;
    T ret;
    if (maxItems < 0) {
      if (tail == 0) throw InvalidOpException("Empty queue");
      ret = items[0];
      }
    else {
      if (head == tail) throw InvalidOpException("Empty queue");
      ret = items[head];
      }
    return ret;
    }

  template <class T>
  Int32 Queue<T>::Size() {
    if (maxItems < 0) return tail;
    return maxItems - 1;
    }

