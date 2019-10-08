
template <class K,class V>
Dictionary<K,V>::Dictionary() {
  this->objectType = (char*)"Dictionary";
  this->keys = NULL;
  this->values = NULL;
  this->numEntries = 0;
  this->comparator = NULL;
  }

template <class K,class V>
Dictionary<K,V>::Dictionary(Boolean (*c)(K,K)) {
  this->objectType = (char*)"Dictionary";
  this->keys = NULL;
  this->values = NULL;
  this->numEntries = 0;
  this->comparator = c;
  }
  
template <class K,class V>
Dictionary<K,V>::~Dictionary() {
  this->Clear();
  }
  
template <class K,class V>
void Dictionary<K,V>::Clear() {
  if (numEntries > 0) {
    free(keys);
    free(values);
    numEntries = 0;
    }
  }

template <class K,class V>
V Dictionary<K,V>::Add(K key, V value) {
  UInt32 i;
  for (i=0; i<numEntries; i++)
    if ((comparator == NULL && keys[i] == key) ||
        (comparator != NULL && comparator(keys[i], key))) {
      values[i] = value;
      return value;
      }
  if (++numEntries == 1) {
    keys = (K*)malloc(sizeof(K));
    values = (V*)malloc(sizeof(V));
    }
  else {
    keys = (K*)realloc(keys,sizeof(K) * numEntries);
    values = (V*)realloc(values,sizeof(V) * numEntries);
    }
  keys[numEntries-1] = key;
  values[numEntries-1] = value;
  return value;
  }

template <class K,class V>
Boolean Dictionary<K,V>::ContainsKey(K key) {
  UInt32 i;
  for (i=0; i<numEntries; i++)
    if ((comparator == NULL && keys[i] == key) ||
        (comparator != NULL && comparator(keys[i], key))) {
      return true;
      }
  return false;
  }

template <class K,class V>
Boolean Dictionary<K,V>::ContainsValue(V value) {
  UInt32 i;
  for (i=0; i<numEntries; i++)
    if (values[i] == value) return true;
  return false;
  }

template <class K,class V>
UInt32 Dictionary<K,V>::Count() {
  return numEntries;
  }

template <class K,class V>
List<K>* Dictionary<K,V>::Keys() {
  UInt32   i;
  List<K>* ret;
  if (numEntries == 0) return NULL;
  ret = new List<K>();
  for (i=0; i<numEntries; i++)
    ret->Add(keys[i]);
  return ret;
  }

template <class K,class V>
V Dictionary<K,V>::Lookup(K key) {
  UInt32 i;
  for (i=0; i<numEntries; i++)
    if ((comparator == NULL && keys[i] == key) ||
        (comparator != NULL && comparator(keys[i], key))) {
      return values[i];
      }
  throw NotFoundException("Key not found");
  }

template <class K,class V>
V Dictionary<K,V>::Remove(K key) {
  UInt32 i;
  UInt32 j;
  V ret;
  for (i=0; i<numEntries; i++)
    if ((comparator == NULL && keys[i] == key) ||
        (comparator != NULL && comparator(keys[i], key))) {
      ret = values[i];
      for (j=i; j<numEntries-1; j++) {
        keys[j] = keys[j+1];
        values[j] = values[j+1];
        }
      return ret;
      }
  throw NotFoundException("Key not found");
  }

template <class K,class V>
List<V>* Dictionary<K,V>::Values() {
  UInt32   i;
  List<V>* ret;
  if (numEntries == 0) return NULL;
  ret = new List<V>();
  for (i=0; i<numEntries; i++)
    ret->Add(values[i]);
  return ret;
  }
  

