
  template <class T>
  Array<T>::Array(UInt32 size) {
    objectType = (char*)"Array";
    min = 0;
    max = size-1;
    items = (T*)malloc(sizeof(T) * size);
    if (items == NULL)
      throw MemoryException("Could not allocate memory for array");
    }

  template <class T>
  Array<T>::Array(Int32 minimum, Int32 maximum) {
    objectType = (char*)"Array";
    min = minimum;
    max = maximum;
    if (min > max)
      throw InvalidOpException("Minimum cannot be greater than maximum");
    items = (T*)malloc(sizeof(T) * (1+max-min));
    if (items == NULL)
      throw MemoryException("Could not allocate memory for array");
    }

  template <class T>
  Array<T>::~Array() {
    free(items);
    }

  template <class T>
  T& Array<T>::operator [] (Int32 index) {
    if (index < min || index > max)
      throw RangeException("Index out of range");
    return items[index];
    }

