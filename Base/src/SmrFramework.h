#ifndef _SMR_FRAMEWORK_H
#define _SMR_FRAMEWORK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <time.h>
#include <exception>
#include <termios.h>

#ifdef __GNUC__
#ifdef __x86_64__
#define IS64BIT
#else
#define IS32BIT
#endif
#endif

namespace SmrFramework {

  typedef int8_t    Int8;
  typedef int16_t   Int16;
  typedef int32_t   Int32;
  typedef int64_t   Int64;
  typedef u_int8_t  UInt8;
  typedef u_int16_t UInt16;
  typedef u_int32_t UInt32;
  typedef u_int64_t UInt64;
  typedef u_int8_t  Byte;
  typedef u_int16_t Word;
  typedef u_int32_t Dword;
  typedef u_int64_t Qword;
  typedef double    Double;
  typedef float     Single;
  typedef bool      Boolean;

  class String;
  class BitStream;
  class Line;
  class Convert;

  class Object {
    protected:
      char* objectType;
    public:
      Object();
      virtual ~Object();
      virtual char*   Type();
      virtual char*   AsCharArray();
      virtual char*   ToCharArray();
      virtual Boolean Equals(Object* b);
      virtual String  ToString();
    };

  class Exception : public std::exception {
    protected:
      char    message[256];
      Object *sender;
    public:
      Exception(const char* msg);
      Exception(Object* obj, const char* msg);
      ~Exception() throw ();
      char* Message();
      Object* Sender();
    };

  class CastException : public Exception {
    protected:
    public:
      CastException(const char* msg);
      CastException(Object* obj, const char* msg);
      ~CastException() throw ();
    };

  class FormatException : public Exception {
    protected:
    public:
      FormatException(const char* msg);
      FormatException(Object* obj, const char* msg);
      ~FormatException() throw ();
    };

  class NullException : public Exception {
    protected:
    public:
      NullException(const char* msg);
      NullException(Object* obj, const char* msg);
      ~NullException() throw ();
    };

  class RangeException : public Exception {
    protected:
    public:
      RangeException(const char* msg);
      RangeException(Object* obj, const char* msg);
      ~RangeException() throw ();
    };

  class InvalidOpException : public Exception {
    protected:
    public:
      InvalidOpException(const char* msg);
      InvalidOpException(Object* obj, const char* msg);
      ~InvalidOpException() throw ();
    };

  class MemoryException : public Exception {
    protected:
    public:
      MemoryException(const char* msg);
      MemoryException(Object* obj, const char* msg);
      ~MemoryException() throw ();
    };

  class NotFoundException : public Exception {
    protected:
    public:
      NotFoundException(const char* msg);
      NotFoundException(Object* obj, const char* msg);
      ~NotFoundException() throw ();
    };

  class Thread : public Object {
    private:
      pthread_t threadId;
      void*     _arg;
    protected:
      bool         terminate;
      static void* entryPoint(void* pthis);
      void        *arg();
      void         arg(void* a);
      Int32        run(void* a);
      virtual void setup();
      virtual void execute(void* a);
    public:
      Thread();
      Int32     Start(void* a);
      void      Terminate();
      pthread_t ThreadId();
    };

  class Callback : public Object {
    protected:
    public:
      Callback();
      virtual ~Callback();
      virtual Boolean Call(void* arg) = 0;
    };

  class GridObject : public Object {
    protected:
    public:
      GridObject();
      ~GridObject();
      Boolean Passable();
    };

  class HexGrid : public Object {
    protected:
      UInt32 height;
      UInt32 width;
      GridObject* grid[100][100];
    public:
      static const Byte DirNorth = 1;
      static const Byte DirNorthEast = 2;
      static const Byte DirSouthEast = 3;
      static const Byte DirSouth = 4;
      static const Byte DirSouthWest = 5;
      static const Byte DirNorthWest = 6;
      HexGrid();
      HexGrid(UInt32 w,UInt32 h);
      virtual ~HexGrid();
      virtual GridObject*  At(UInt32 pos);
      virtual GridObject*  AtPut(UInt32 pos, GridObject* v);
      virtual void     Clear();
      virtual Byte     DirectionTo(UInt32 spos, UInt32 tpos);
      virtual Int32    Distance(UInt32 pos1, UInt32 pos2);
      virtual Int32    Find(GridObject* obj);
      virtual UInt32   Height();
      virtual UInt32   Height(UInt32 i);
      virtual GridObject*  Move(UInt32 psrc, UInt32 pdst);
      virtual Int32    Next(UInt32 pos, Byte dir);
      virtual UInt32   Width();
      virtual UInt32   Width(UInt32 i);
    };

  class Huffman : public Object {
    protected:
      Dword* codes;
      Dword* values;
      Byte*  bits;
      Int32  numEntries;
      Int32  maxEntries;
      Int32  starts[64];
    public:
      Huffman(Int32 max);
      virtual ~Huffman();
      virtual bool Add(Dword code, Dword value, Byte size);
      virtual void Clear();
      virtual Dword Lookup(Dword code, Byte size);
      virtual Dword Next(BitStream* stream);
    };

  class Flate : public Object {
    protected:
      Huffman* lengths;
      Huffman* distances;
      Byte* lzWindow;
      Word  wPos;
      Word  cPos;
      char  huffmanMode;
      char  readMode;
      Word  compression;
      Word  window;
      Word  check;
      Word  preset;
      Word  flevel;
      char  mode;
      Word  final;
      Dword length;
      Byte  current;
      Int32 mask;
      Byte* buffer;
      Int32 bpos;
      Int32 blength;
      BitStream* stream;
      Int32   _read_flate();
      void    _flate_build_huffman(Int32 count, Dword* lengths, Huffman* table);
      void    _fixed_flate_huffman();
      Word    _flate_read_huffman(UInt32 max, Huffman* table);
      void    _flate_read_lengths(int count, Dword* lengths, Huffman* table);
      void    _dynamic_flate_huffman();
    public:
      Flate(BitStream* s, char m);
      virtual ~Flate();
      virtual Int32 Read(Byte* buffer, int len);
    };

  class LZW : public Object {
    protected:
      Int32  bitSize;           /* Starting bit size */
      Int32  maxCodes;          /* Maximum size of code table */
      Int32  dictPos;           /* Position of next entry */
      Int32* dictCode;          /* Code portion of dictionary entry */
      Byte*  dictChar;          /* Character portion of dictionary entry */
      Int32  currentCode;       /* Code for current string */
      Int32  currentChar;       /* Char for current string */
      Int32  oldCode;
      Int32  output[4096];      /* holds output from decompressor */
      Int32  scanDictionary(Int32 code, Int32 chr);
      Int32  buildString(Int32 entry);
    public:
      LZW(Int32 bits, Int32 maxBits);
      virtual ~LZW();
      virtual Int32  Compress(Int32 code);
      virtual Int32  Decompress(Int32 newCode);
      virtual Int32  DictPosition();
      virtual void   InitDictionary();
      virtual Int32* Output();
    };

  class Hash : public Object {
    protected:
    public:
      Hash();
      virtual ~Hash();
      static String Md5(const char* msg, UInt32 size);
      static String Sha1(const char* msg, UInt32 size);
    };
      
  class Palette : public Object {
    protected:
      UInt32* colors;
      UInt32* counts;
      UInt32  numColors;
      Byte    redMask;
      Byte    greenMask;
      Byte    blueMask;
    public:
      Palette();
      virtual ~Palette();
      virtual void     Add(Byte red, Byte green, Byte blue);
      virtual UInt32   Count();
      virtual Byte     Blue(UInt32 index);
      virtual Byte     Green(UInt32 index);
      virtual Byte     Red(UInt32 index);
      virtual UInt32   Lookup(Byte red, Byte green, Byte blue);
      virtual Palette* Reduce(UInt32 maxColors);
      virtual void     SignificantBits(Byte bits);
    };

  class File : public Object {
    protected:
      int file;
      int mode;
    public:
      static const int ReadOnly = 1;
      static const int WriteOnly = 2;
      static const int ReadWrite = 3;
      static const int Truncate = 4;
      File();
      File(int f,int m);
      File(const char* fname);
      File(const char* fname,int m);
      File(String fname);
      File(String fname,int m);
      File(String* fname);
      File(String* fname,int m);
      virtual ~File();
      static String  Basename(const char* path);
      static String  Basename(String* path);
      static String  Basename(String path);
      static void    Copy(const char* src,const char* dest);
      static File*   Create(const char* pathname);
      static void    Delete(const char* pathname);
      static bool    Exists(const char* pathname);
      static bool    Exists(String* pathname);
      static bool    Exists(String pathname);
      static String  Extension(const char* path);
      static String  Extension(String* path);
      static String  Extension(String path);
      static void    Move(const char* oldname, const char* newname);
      static File*   OpenForAppend(const char* pathname);
      static File*   OpenForReading(const char* pathname);
      static File*   OpenForReadingWriting(const char* pathname);
      static File*   OpenForWriting(const char* pathname);
      static String  Path(const char* path);
      static String  Path(String* path);
      static String  Path(String path);
      virtual UInt32 Append();
      virtual void   Close();
      virtual bool   EndOfFile();
      virtual void   Flush();
      virtual UInt32 Position();
      virtual UInt32 Position(UInt32 pos);
      virtual Byte   ReadByte();
      virtual UInt32 ReadBytes(Byte* buffer, UInt32 count);
      virtual Int16  ReadInt16();
      virtual UInt16 ReadUInt16();
      virtual Int32  ReadInt32();
      virtual UInt32 ReadUInt32();
      virtual Int64  ReadInt64();
      virtual UInt64 ReadUInt64();
      virtual Single ReadSingle();
      virtual Double ReadDouble();
      virtual UInt32 Reset();
      virtual UInt32 Write(Byte value);
      virtual UInt32 Write(void* buffer,int count);
      virtual UInt32 Write(Int16 value);
      virtual UInt32 Write(UInt16 value);
      virtual UInt32 Write(Int32 value);
      virtual UInt32 Write(UInt32 value);
      virtual UInt32 Write(Int64 value);
      virtual UInt32 Write(UInt64 value);
      virtual UInt32 Write(Single value);
      virtual UInt32 Write(Double value);
    };

  class Stream : public Object {
    protected:
      Byte accessMode;
      Boolean deleteResource;
    public:
      static const Byte ReadOnly = 1;
      static const Byte WriteOnly = 2;
      static const Byte ReadWrite = 3;
      static const Byte Truncate = 4;
      Stream(Byte mode);
      virtual ~Stream();
      virtual void Close();
      virtual Boolean DeleteResource();
      virtual Boolean DeleteResource(Boolean b);
      virtual bool EndOfStream();
      virtual void Flush();
      virtual UInt32 Position();
      virtual UInt32 Position(UInt32 pos);
      virtual UInt32 Write(Byte v);
      virtual UInt32 Write(Word i);
      virtual UInt32 Write(Dword d);
      virtual UInt32 Write(Qword q);
      virtual UInt32 Write(Byte* b,UInt32 len);
      virtual Byte ReadByte();
      virtual Word ReadWord();
      virtual Dword ReadDword();
      virtual Qword ReadQword();
      virtual UInt32 ReadByteArray(Byte* b,UInt32 maxlen);
      virtual UInt32 Rewind();
      };

  class FileStream : public Stream {
    protected:
      File* file;
      char* filename;
    public:
      FileStream(const char* path);
      FileStream(const char* path,Byte mode);
      FileStream(String path);
      FileStream(String path,Byte mode);
      FileStream(String* path);
      FileStream(String* path,Byte mode);
      virtual ~FileStream();
      virtual void Close();
      virtual bool EndOfStream();
      virtual void Flush();
      virtual UInt32 Position();
      virtual UInt32 Position(UInt32 pos);
      virtual UInt32 Write(Byte v);
      virtual UInt32 Write(Word i);
      virtual UInt32 Write(Dword d);
      virtual UInt32 Write(Qword q);
      virtual UInt32 Write(Byte* b,UInt32 len);
      virtual Byte ReadByte();
      virtual Word ReadWord();
      virtual Dword ReadDword();
      virtual Qword ReadQword();
      virtual UInt32 ReadByteArray(Byte* b,UInt32 maxlen);
      virtual UInt32 Rewind();
    };

  class MemoryStream : public Stream {
    protected:
      Byte* data;
      UInt32 dataSize;
      UInt32 dataPointer;
    public:
      MemoryStream(UInt32 size);
      MemoryStream(Byte* buffer,UInt32 size);
      MemoryStream(const char* path);
      MemoryStream(const char* path,Byte mode);
      virtual ~MemoryStream();
      virtual void Close();
      virtual bool EndOfStream();
      virtual void Expand(UInt32 size);
      virtual void Flush();
      virtual void Save(const char* filename);
      virtual UInt32 Position();
      virtual UInt32 Position(UInt32 pos);
      virtual UInt32 Write(Byte v);
      virtual UInt32 Write(Word i);
      virtual UInt32 Write(Dword d);
      virtual UInt32 Write(Qword q);
      virtual UInt32 Write(Byte* b,UInt32 len);
      virtual Byte ReadByte();
      virtual Word ReadWord();
      virtual Dword ReadDword();
      virtual Qword ReadQword();
      virtual UInt32 ReadByteArray(Byte* b,UInt32 maxlen);
      virtual UInt32 Rewind();
    };

  class StringStream : public Stream {
    protected:
      String* data;
      UInt32  dataSize;
      UInt32  dataPointer;
    public:
      StringStream(String* s);
      virtual ~StringStream();
      virtual void Close();
      virtual bool EndOfStream();
      virtual UInt32 Position();
      virtual UInt32 Position(UInt32 pos);
      virtual UInt32 Write(Byte v);
      virtual UInt32 Write(Word i);
      virtual UInt32 Write(Dword d);
      virtual UInt32 Write(Qword q);
      virtual UInt32 Write(Byte* b,UInt32 len);
      virtual Byte ReadByte();
      virtual Word ReadWord();
      virtual Dword ReadDword();
      virtual Qword ReadQword();
      virtual UInt32 ReadByteArray(Byte* b,UInt32 maxlen);
      virtual UInt32 Rewind();
    };

  class StreamWriter : public Object {
    protected:
      Stream* stream;
      int     autoFlush;
      char*   newLine;
    public:
      StreamWriter(const char* filename);
      StreamWriter(String filename);
      StreamWriter(String* filename);
      virtual ~StreamWriter();
      virtual void AutoFlush(int i);
      virtual void Flush();
      virtual void NewLine(const char* term);
      virtual int Write(char c);
      virtual int Write(const char* c);
      virtual int Write(const char* c, int count);
      virtual int Write(String* s);
      virtual int Write(String s);
      virtual int Write(int i);
      virtual int Write(double d);
      virtual int Write(bool b);
      virtual int WriteLine();
      virtual int WriteLine(char c);
      virtual int WriteLine(const char* c);
      virtual int WriteLine(const char* c, int count);
      virtual int WriteLine(String* s);
      virtual int WriteLine(String s);
      virtual int WriteLine(int i);
      virtual int WriteLine(double d);
      virtual int WriteLine(bool b);
    };

  class StreamReader : public Object {
    protected:
      Stream* stream;
    public:
      StreamReader(Stream* s);
      StreamReader(const char* filename);
      StreamReader(String filename);
      StreamReader(String* filename);
      virtual ~StreamReader();
      virtual Boolean DeleteResource();
      virtual Boolean DeleteResource(Boolean b);
      virtual bool    EndOfStream();
      virtual Byte    Peek();
      virtual Byte    Read();
      virtual Byte    ReadByte();
      virtual Word    ReadWord();
      virtual Dword   ReadDword();
      virtual UInt32  Read(Byte* buffer,int len);
      virtual String  ReadLine();
    };

  class Image : public Object {
    protected:
      UInt32 width;
      UInt32 height;
      Byte*  data;
      char*  pathname;
      Byte   red;
      Byte   green;
      Byte   blue;
      Stream* file;
      void   readBMP();
      void   readGIF();
      void   readJPG();
      void   readPNG();
      void   readXBM();
      void   readXPM();
      void   loadImage();
    public:
      Image(int w, int h);
      Image(const char* path);
      Image(Stream* stream);
      virtual ~Image();
      virtual void   DrawingColor(Byte r, Byte g, Byte b);
      virtual void   DrawLine(UInt32 x1, UInt32 y1, UInt32 x2, UInt32 y2);
      virtual void   DrawPoint(UInt32 x, UInt32 y);
      virtual void   DrawRectangle(UInt32 x, UInt32 y, UInt32 w, UInt32 h);
      virtual void   FillRectangle(UInt32 x, UInt32 y, UInt32 w, UInt32 h);
      virtual Image* FlipHorizontal();
      virtual Image* FlipVertical();
      virtual Image* FromPalette(Palette* palette);
      virtual Palette* GetPalette();
      virtual Palette* GetPalette(Byte bits);
      virtual Image* Grayscale();
      virtual UInt32 Height();
      virtual Image* Invert();
      virtual Image* Quantize(Byte numBits);
      virtual Image* Rotate180();
      virtual Image* RotateClockwise();
      virtual Image* RotateCounterClockwise();
      virtual void   SaveAsBMP(const char* pathname);
      virtual void   SaveAsXPM(const char* pathname);
      virtual Image* Scale(Double percent);
      virtual Image* Scale(UInt32 w, UInt32 h);
      virtual Image* SubImage(UInt32 x, UInt32 y, UInt32 w, UInt32 h);
      virtual Image* Transform(Line src, Line dst);
      virtual UInt32 Width();
      virtual Byte*  ImageData();
    };

  class TimeSpan : public Object {
    protected:
      int days;
      int hours;
      int minutes;
      int seconds;
    public:
      TimeSpan();
      TimeSpan(String* s);
      TimeSpan(int d,int h,int m, int s);
      TimeSpan(double value);
      virtual ~TimeSpan();
      static int  Compare(TimeSpan* tsa, TimeSpan* tsb);
      static void Normalize(int* days,int* hours,int* mins,int* secs);
      virtual TimeSpan   Add(TimeSpan value);
      virtual TimeSpan   Add(TimeSpan* value);
      virtual TimeSpan   AddHours(int amount);
      virtual TimeSpan   AddMinutes(int amount);
      virtual TimeSpan   AddSeconds(int amount);
      virtual TimeSpan   AddDays(int amount);
      virtual Double     AsSeconds();
      virtual Double     AsMinutes();
      virtual Double     AsHours();
      virtual Double     AsDays();
      virtual int        CompareTo(TimeSpan* tsb);
      virtual int        Days();
      virtual int        Hours();
      virtual int        Minutes();
      virtual Boolean    Negative();
      virtual int        Seconds();
      virtual TimeSpan   Subtract(TimeSpan* value);
      virtual double     ToDouble();
      virtual String     ToString();
    };

  class HrTimer : public Object {
    protected:
      UInt64  seconds;
      UInt32  microSeconds;
      UInt64  lapSeconds;
      UInt32  lapMicroSeconds;
      Boolean running;
    public:
      HrTimer();
      virtual ~HrTimer();
      virtual UInt64  Microseconds();
      virtual UInt64  Milliseconds();
      virtual void    Restart();
      virtual Boolean Running();
      virtual UInt64  Seconds();
      virtual void    Start();
      virtual void    Stop();
      virtual Double  ToDouble();
    };

  class DateTime : public Object {
    protected:
      char timeMode;
      int xyz;
      int month;
      int day;
      int year;
      int hour;
      int minute;
      int second;
      int microsecond;
      int    tzOffset;
      time_t epochSeconds;
      void setupTime();
      void _convertFromString(const char* dt);
    public:
      DateTime();
      DateTime(const char* dt);
      DateTime(String dt);
      DateTime(String* dt);
      DateTime(char mode);
      DateTime(const char* dt,char mode);
      DateTime(String dt,char mode);
      DateTime(String* dt,char mode);
      DateTime(int mo,int dy,int yr,int hr=0,int mn=0,int sc=0);
      virtual ~DateTime();
      static int        Compare(DateTime* a, DateTime* b);
      static int        DaysInMonth(int month, int year);
      static int        DaysInYear(int year);
      static int        Equals(DateTime* a, DateTime* b);
      static DateTime   FromJulianDay(double jd);
      static int        IsLeapYear(int year);
      static double     JulianDay(int m, int d, int y);
      static DateTime   Now();
      static DateTime   Now(char mode);
      virtual void      operator = (const DateTime &dt);
      virtual Boolean   operator == (const DateTime &dt);
      virtual Boolean   operator != (const DateTime &dt);
      virtual Boolean   operator > (const DateTime &dt);
      virtual Boolean   operator >= (const DateTime &dt);
      virtual Boolean   operator < (const DateTime &dt);
      virtual Boolean   operator <= (const DateTime &dt);
      virtual DateTime  Add(TimeSpan* value);
      virtual DateTime  AddDays(int amount);
      virtual DateTime  AddHours(int amount);
      virtual DateTime  AddMinutes(int amount);
      virtual DateTime  AddMonths(int amount);
      virtual DateTime  AddSeconds(int amount);
      virtual DateTime  AddYears(int amount);
      virtual int       CompareTo(DateTime* b);
      virtual int       Day();
      virtual int       DayOfWeek();
      virtual int       DayOfYear();
      virtual int       Equals(DateTime* b);
      virtual int       Hour();
      virtual int       MicroSecond();
      virtual int       Minute();
      virtual int       Month();
      virtual void      Normalize(int* month,int* day,int* year,int* hr,int* min,int* sec);
      virtual int       Second();
      virtual TimeSpan  Subtract(DateTime* value);
      virtual DateTime  Subtract(TimeSpan* value);
      virtual double    ToDouble();
      virtual double    ToJulianDay();
      virtual String    ToString();
      virtual int       Year();
    };

  class StopWatch : public Object {
    protected:
      DateTime startTime;
      DateTime stopTime;
      Boolean  running;
      UInt64   startMilli;
      UInt64   stopMilli;
    public:
      StopWatch();
      virtual ~StopWatch();
      virtual void     Restart();
      virtual Boolean  Running();
      virtual UInt64   Seconds();
      virtual UInt64   Milliseconds();
      virtual void     Start();
      virtual void     Stop();
      virtual TimeSpan Time();
    };
      
  template <class T>
  class Array : public Object {
    protected:
      T*         items;
      Int32      min;
      Int32      max;
    public:
      Array(UInt32 size);
      Array(Int32 minimum, Int32 maximum);
      virtual ~Array();
      virtual T&       operator [] (Int32 index);
    };

  template <class T>
  class List : public Object {
    protected:
      T*       items;
      UInt32   count;
      UInt32   pos;
      Int32    (*comparer)(T,T);
    public:
      List();
      virtual ~List();
      virtual T&       operator [] (UInt32 index);
      virtual void     Add(T o);
      virtual T        At(UInt32 n);
      virtual int      AtEnd();
      virtual void     AtPut(UInt32 n,T o);
      virtual void     Clear();
      virtual void     Comparer(Int32 (*func)(T,T));
      virtual int      Contains(T o);
      virtual UInt32   Count();
      virtual void     Delete(T o);
      virtual T        First();
      virtual int      IndexOf(T o);
      virtual void     InsertAt(UInt32 pos, T o);
      virtual List<T>* Intersect(List<T>* b);
      virtual int      LastIndexOf(T o);
      virtual T        Next();
      virtual UInt32   Occurances(T o);
      virtual void     Remove(T o);
      virtual void     RemoveAt(UInt32 pos);
      virtual void     Sort();
      virtual List<T>* Union(List<T>* b);
    };

  template <class T>
  class Set : public List<T> {
    protected:
    public:
      Set();
      virtual ~Set();
      virtual void Add(T o);
      virtual Set<T>* Intersect(Set<T>* b);
      virtual Set<T>* Union(Set<T>* b);
    };

  template <class T>
  class Stack : public Object {
    protected:
      T*    items;
      int   count;
      int   pos;
    public:
      Stack();
      virtual ~Stack();
      virtual T    At(int index);
      virtual void Clear();
      virtual int  Contains(T value);
      virtual int  Count();
      virtual void Dup();
      virtual T    First();
      virtual T    Next();
      virtual T    Peek();
      virtual void Push(T item);
      virtual T    Pop();
      virtual void Swap();
    };

  template <class T>
  class Queue : public Object {
    protected:
      T* items;
      UInt32 head;
      UInt32 tail;
      Int32  maxItems;
      bool   overwrite;
    public:
      Queue();
      Queue(Int32 max);
      virtual ~Queue();
      virtual bool  Add(T value);
      virtual bool  AtEnd();
      virtual void  Clear();
      virtual Int32 Count();
      virtual bool  Empty();
      virtual T     Next();
      virtual bool  Overwrite();
      virtual bool  Overwrite(bool b);
      virtual T     Peek();
      virtual Int32 Size();
    };
      
  template <class K,class V>
  class Dictionary : public Object {
    protected:
      K* keys;
      V* values;
      UInt32 numEntries;
      Boolean (*comparator)(K v1, K v2);
    public:
      Dictionary();
      Dictionary(Boolean (*c)(K,K));
      virtual ~Dictionary();
      virtual V        Add(K key,V value);
      virtual void     Clear();
      virtual Boolean  ContainsKey(K key);
      virtual Boolean  ContainsValue(V value);
      virtual UInt32   Count();
      virtual V        Lookup(K key);
      virtual K        KeyAt(UInt32 i);
      virtual List<K>* Keys();
      virtual V        Remove(K key);
      virtual V        ValueAt(UInt32 i);
      virtual List<V>* Values();
    };

  class Utils : public Object {
    protected:
    public:
      Utils();
      ~Utils();
      static void Swap(Byte* a, Byte* b);
      static void Swap(Int8* a, Int8* b);
      static void Swap(Int16* a, Int16* b);
      static void Swap(UInt16* a, UInt16* b);
      static void Swap(Int32* a, Int32* b);
      static void Swap(UInt32* a, UInt32* b);
      static void Swap(Int64* a, Int64* b);
      static void Swap(UInt64* a, UInt64* b);
      static void Swap(Single* a, Single* b);
      static void Swap(Double* a, Double* b);
      static void Swap(Byte &a, Byte &b);
      static void Swap(Int8 &a, Int8 &b);
      static void Swap(Int16 &a, Int16 &b);
      static void Swap(Int32 &a, Int32 &b);
      static void Swap(Int64 &a, Int64 &b);
      static void Swap(Single &a, Single &b);
      static void Swap(Double &a, Double &b);
      static Byte   RotateLeft(Byte v, UInt32 count);
      static UInt16 RotateLeft(UInt16 v, UInt32 count);
      static UInt32 RotateLeft(UInt32 v, UInt32 count);
      static UInt64 RotateLeft(UInt64 v, UInt32 count);
      static Byte   RotateRight(Byte v, UInt32 count);
      static UInt16 RotateRight(UInt16 v, UInt32 count);
      static UInt32 RotateRight(UInt32 v, UInt32 count);
      static UInt64 RotateRight(UInt64 v, UInt32 count);
      static char*  Strip(char* buffer);
    };

  class Forth;

  class ForthProgram : public Object {
    protected:
      Forth*  forth;
      UInt32* program;
      UInt32  numTokens;
      UInt32  pos;
      char*   name;
    public:
      ForthProgram(Forth* f);
      virtual ~ForthProgram();
      virtual void   Add(UInt32 token);
      virtual UInt32 At(UInt32 p);
      virtual bool   AtEnd();
      virtual UInt32 Count();
      virtual UInt32 Exec();
      virtual char*  GetName();
      virtual UInt32 GetPos();
      virtual UInt32 Next();
      virtual void   Reset();
      virtual void   SetName(const char* n);
      virtual void   SetPos(UInt32 p);
    };

  class ForthContext : public Object {
    public:
      ForthContext();
      virtual ~ForthContext();
      virtual Int32  At(UInt32 pos) = 0;
      virtual void   Push(Int32 value) = 0;
      virtual Int32  Pop() = 0;
      virtual void   Clear() = 0;
      virtual UInt32 Count() = 0;
      virtual void   Output(const char* msg) = 0;
      virtual void   Write(UInt32 address,Int32 value) = 0;
      virtual Int32  Read(UInt32 address) = 0;
      virtual void   Function(UInt32 number) = 0;
    };

  class Forth : public Object {
    protected:
      ForthContext*  context;
      Stack<UInt32>* rstack;
      ForthProgram** functions;
      UInt32         numFunctions;
      char**         variables;
      UInt32*        variableLocations;
      UInt32         numVariables;
      UInt32         findToken(const char* token);
    public:
      Forth(ForthContext* c);
      virtual ~Forth();
      virtual ForthContext* GetContext();
      virtual UInt32        Exec(ForthProgram* program);
      virtual ForthProgram* Parse(const char* text);
      virtual char*         PopString();
      virtual void          PushString(const char* str);
      virtual UInt32        Variable(const char* name);
      virtual UInt32        Variable(const char* name,UInt32 pos);
    };

  class String : public Object {
    protected:
      Byte* string;
      UInt32 size;
      UInt32 length;
      UInt32 pos;
      void  _setValue(Byte* s, UInt32 l);
    public:
      String();
      String(const char* s);
      String(Byte* s);
      String(const char* s,UInt32 len);
      String(Byte* s,UInt32 len);
      String(String* s);
      String(const String &s);
      String(char s);
      virtual ~String();
      virtual String&        operator= (const String &s);
      virtual void           operator = (const char* s);
      virtual bool           operator == (const String &s);
      virtual bool           operator == (const char* s);
      virtual bool           operator != (const String &s);
      virtual bool           operator != (const char* s);
      virtual bool           operator > (const String &s);
      virtual bool           operator > (const char* s);
      virtual bool           operator >= (const String &s);
      virtual bool           operator >= (const char* s);
      virtual bool           operator < (const String &s);
      virtual bool           operator < (const char* s);
      virtual bool           operator <= (const String &s);
      virtual bool           operator <= (const char* s);
      virtual String         operator + (char c);
      virtual String         operator + (const char* s);
      virtual String         operator + (String* s);
      virtual String         operator + (String s);
      virtual Byte          &operator [] (UInt32 i);
      virtual String         Append(char c);
      virtual String         Append(const char* s);
      virtual String         Append(String* s);
      virtual String         Append(String s);
      virtual char           At(UInt32 pos);
      virtual char           CharAt(UInt32 pos);
      virtual void           Clear();
      virtual int            CompareTo(const char* s);
      virtual int            CompareTo(String* s);
      virtual int            CompareTo(String s);
      virtual bool           Contains(char c);
      virtual bool           Contains(const char* s);
      virtual bool           Contains(String* s);
      virtual bool           Contains(String s);
      virtual UInt32         Count(char c);
      virtual void           Dispose();
      virtual bool           EndsWith(const char* s);
      virtual bool           EndsWith(String* s);
      virtual bool           EndsWith(String s);
      virtual bool           Equals(const char* s);
      virtual bool           Equals(String* s);
      virtual bool           Equals(String s);
      virtual bool           Equals(const char* s,Boolean igncase);
      virtual bool           Equals(String* s,Boolean igncase);
      virtual bool           Equals(String s,Boolean igncase);
      virtual char           First();
      virtual int            IndexOf(char c);
      virtual int            IndexOf(const char* s);
      virtual int            IndexOf(String* s);
      virtual int            IndexOf(String s);
      virtual char           Last();
      virtual int            LastIndexOf(char c);
      virtual int            LastIndexOf(const char* s);
      virtual int            LastIndexOf(String* s);
      virtual int            LastIndexOf(String s);
      virtual UInt32         Length();
      virtual char           Next();
      virtual String         PadLeft(UInt32 len);
      virtual String         PadLeft(UInt32 len, char c);
      virtual String         PadRight(UInt32 len);
      virtual String         PadRight(UInt32 len, char c);
      virtual String         Reverse();
      virtual String         Rot13();
      virtual void           SetValue(const char* s);
      virtual List<String*>* Split(char c);
      virtual bool           StartsWith(const char* s);
      virtual bool           StartsWith(String* s);
      virtual bool           StartsWith(String s);
      virtual String         Substring(int start);
      virtual String         Substring(int start, int length);
      virtual char*          AsCharArray();
      virtual char*          ToCharArray();
      virtual List<String*>* Tokenize(String* separators);
      virtual List<String*>* Tokenize(String separators);
      virtual String         ToLower();
      virtual String         ToString();
      virtual String         ToUpper();
      virtual String         Trim();
      virtual String         TrimLeft();
      virtual String         TrimRight();
    };

  class Base64 : public Object {
    protected:
      char alphabet[65];
      char padding;
      char outchars[5];
      Byte outbytes[4];
      char* encodeGroup(Byte* inp);
      int   decodeGroup(const char* inp);
    public:
      Base64();
      virtual ~Base64();
      virtual String  Encode(const Byte* msg,int len);
      virtual String  Encode(String* msg);
      virtual String  Decode(const char* msg);
      virtual String  Decode(String* msg);
      virtual void    SetAlphabet(const char* alpha);
      virtual void    SetPadding(char p);
    };

  class Log : public Object {
    protected:
      List<String*>* filenames;
    public:
      Log(const char* filename);
      Log(String* filename);
      virtual ~Log();
      virtual void Close();
      virtual void Add(const char* filename);
      virtual void Add(String* filename);
      virtual void Write(const char* message);
    };

  class RegEx : public Object {
    protected:
      static const Byte TOKEN_LITERAL = 0;
      static const Byte TOKEN_START = 1;
      static const Byte TOKEN_ANY = 2;
      static const Byte TOKEN_VAR_START = 3;
      static const Byte TOKEN_VAR_END = 4;
      static const Byte TOKEN_INCLUDE = 5;          // []
      static const Byte TOKEN_EXCLUDE = 6;          // [^]
      static const Byte TOKEN_END = 7;
      List<Byte>*    tokenTypes;
      List<String*>* tokens;
      List<UInt32>*  minCount;
      List<UInt32>*  maxCount;
      UInt32         varStart[32];
      UInt32         varEnd[32];
      UInt32         varCount;
      String         vars[32];
      virtual Boolean matchNext(const char* input, UInt32 inPos, UInt32 tokenPos);
    public:
      RegEx(const char* expression);
      virtual ~RegEx();
      virtual Boolean Match(const char* input);
      virtual String  Var(Byte index);
      virtual Byte    VarCount();
    };

  class FileInfo : public Object {
    protected:
      int access;
      int blockCount;
      int blockSize;
      int device;
      int inode;
      int owner;
      int group;
      int links;
      int size;
      int fileType;
      DateTime* lastAccessTime;
      DateTime* lastModificationTime;
      DateTime* lastStatusTime;
      int getFileInfo(const char* path);
    public:
      static const int RegularFile  = 1;
      static const int SymbolicLink = 2;
      static const int Socket       = 3;
      static const int BlockDevice  = 4;
      static const int Directory    = 5;
      static const int CharDevice   = 6;
      static const int NamedPipe    = 7;
      static const int Unknown      = 8;
      FileInfo(const char* path);
      FileInfo(String* path);
      virtual ~FileInfo();
      virtual int BlockCount();
      virtual int BlockSize();
      virtual int Device();
      virtual int Inode();
      virtual int Group();
      virtual int Links();
      virtual int Size();
      virtual int FileType();
      virtual int Owner();
      virtual int OwnerRead();
      virtual int OwnerWrite();
      virtual int OwnerExecute();
      virtual int GroupRead();
      virtual int GroupWrite();
      virtual int GroupExecute();
      virtual int OtherRead();
      virtual int OtherWrite();
      virtual int OtherExecute();
      virtual DateTime* LastAccessTime();
      virtual DateTime* LastModificationTime();
      virtual DateTime* LastStatusTime();
    };

  class Directory : public Object {
    protected:
    public:
      Directory();
      ~Directory();
      static void           CreateDirectory(const char* path);
      static void           CreateDirectory(String* path);
      static void           Delete(const char* path);
      static void           Delete(String* path);
      static int            Exists(const char* path);
      static int            Exists(String* path);
      static String*        GetCurrentDirectory();
      static List<String*>* GetAllFiles(const char* path,List<String*>* results);
      static List<String*>* GetFiles(const char* path);
      static List<String*>* GetFiles(String* path);
      static void           SetCurrentDirectory(const char* path);
      static void           SetCurrentDirectory(String* path);
    };

  class BitStream : public Object {
    protected:
      Stream* stream;
      bool    deleteStreamOnDispose;
      Byte    currentByte;
      Byte    mask;
      bool    littleEndian;
      bool    written;
      bool    fillLowToHigh;
    public:
      BitStream(Stream* s);
      virtual ~BitStream();
      virtual bool    DeleteStreamOnDispose();
      virtual bool    DeleteStreamOnDispose(bool b);
      virtual bool    FillLowToHigh();
      virtual bool    FillLowToHigh(bool b);
      virtual bool    LittleEndian();
      virtual bool    LittleEndian(bool b);
      virtual void    Flush();
      virtual Dword   Read(Byte bits);
      virtual void    Write(Byte bits, Dword value);
    };

  class Lexer : public Object {
    protected:
      StreamReader*    source;
      List<RegEx*>*    regExes;
      List<UInt32>*    regExValues;
      List<Callback*>* regExCallbacks;
      Byte             last;
      Byte             token[1024];
      Word             tokenPos;
      UInt32           result;
      Byte             holdingBuffer[1024];
      Word             holdingBufferPos;
      String           value;
      void    init();
      Boolean addByteToToken();
      UInt32  attemptMatch();
      void    moveToHoldingBuffer(UInt32 pos, UInt32 count);
    public:
      Lexer();
      Lexer(StreamReader* s);
      virtual ~Lexer();
      virtual void AddPattern(const char* s, UInt32 v, Callback* cb = NULL);
      virtual Boolean AtEnd();
      virtual UInt32  Next();
      virtual void    Reset();
      virtual void    Source(StreamReader* s);
      virtual String  Value();
    };

  class Parser : public Object {
    protected:
      Lexer*  lex;
      Boolean ownsLexer;
      UInt32  nextToken;
      List<String*>* tokens;
      List<UInt32>*  tokenValues;
      virtual void init();
    public:
      Parser();
      Parser(Lexer* l);
      virtual ~Parser();
      virtual UInt32  AddToken(String name, UInt32 token = 0);
      virtual UInt32  AddToken(String name, String pattern, UInt32 token = 0);
      virtual Lexer*  Lex();
      virtual Lexer*  Lex(Lexer* l);
      virtual UInt32  LookupToken(String t);
      virtual String* LookupToken(UInt32 t);
    };

  class Socket : public Object {
    protected:
      Int32   sock;
      UInt32  addressFamily;
      UInt32  socketType;
      UInt32  protocol;
      Word    port;
      Boolean connected;
      Socket(Int32 sockdes, Word family, Word port, UInt32 address);
    public:
      static const UInt32 AF_Unix = AF_UNIX;
      static const UInt32 AF_Inet = AF_INET;
      static const UInt32 ST_Stream = SOCK_STREAM;
      static const UInt32 ST_Dgram = SOCK_DGRAM;
      static const UInt32 ST_Raw = SOCK_RAW;
      Socket();
      virtual ~Socket();
      virtual Socket* Accept();
      virtual UInt32  AddressFamily();
      virtual void    Bind(Word port);
      virtual void    Bind(const char* address, Word port);
      virtual void    Close();
      virtual void    Connect(const char* hostname, Word port);
      virtual Boolean Connected();
      virtual Boolean DataAvailable(Int32 waitms);
      virtual Int32   Linger();
      virtual Int32   Linger(bool onoff, Int32 seconds);
      virtual void    Listen();
      virtual void    Listen(Int32 backlog);
      virtual Word    Port();
      virtual bool    ReuseAddress();
      virtual bool    ReuseAddress(bool state);
      virtual Int32   Receive(Byte* buffer, UInt32 maxlen);
      virtual Int32   Receive(Byte* buffer, UInt32 maxlen, Int32 maxms);
      virtual Int32   Send(const Byte* buffer, UInt32 len);
      virtual UInt32  SocketType();
    };

  class Tests : public Object {
    protected:
    public:
      Tests();
      ~Tests();
      static Boolean IsAlpha(char value);
      static Boolean IsAlpha(const char* str);
      static Boolean IsAlpha(String* str);
      static Boolean IsAlphaNumeric(char value);
      static Boolean IsAlphaNumeric(const char* str);
      static Boolean IsAlphaNumeric(String* str);
      static Boolean IsBcd(const char* str);
      static Boolean IsBcd(String* str);
      static Boolean IsBinary(const char* str);
      static Boolean IsBinary(String* str);
      static Boolean IsInteger(const char* str);
      static Boolean IsInteger(String* str);
      static Boolean IsFloat(const char* str);
      static Boolean IsFloat(String* str);
      static Boolean IsHex(const char* str);
      static Boolean IsHex(String* str);
      static Boolean IsNumeric(char value);
      static Boolean IsNumeric(const char* str);
      static Boolean IsNumeric(String* str);
      static Boolean IsOctal(const char* str);
      static Boolean IsOctal(String* str);
    };

  class Fraction : public Object {
    protected:
      UInt32 whole;
      UInt32 denominator;
      UInt32 numerator;
      bool   negative;
      UInt32 leastCommonDenominator(UInt32 d1, UInt32 d2);
      void   reduce();
    public:
      Fraction(Double n);
      Fraction(Int32 n, Int32 d);
      Fraction(Int32 w, Int32 n, Int32 d);
      virtual ~Fraction();
      virtual Fraction operator+ (const Fraction& a);
      virtual Fraction operator- (const Fraction& a);
      virtual Fraction operator* (const Fraction& a);
      virtual Fraction operator/ (const Fraction& a);
      virtual bool     operator== (const Fraction& a);
      virtual bool     operator!= (const Fraction& a);
      virtual bool     operator> (const Fraction& a);
      virtual bool     operator>= (const Fraction& a);
      virtual bool     operator< (const Fraction& a);
      virtual bool     operator<= (const Fraction& a);
      virtual UInt32  Denominator();
      virtual UInt32  Denominator(UInt32 d);
      virtual bool    Negative();
      virtual bool    Negative(bool b);
      virtual UInt32  Numerator();
      virtual UInt32  Numerator(UInt32 n);
      virtual String  ToString();
      virtual UInt32  Whole();
      virtual UInt32  Whole(UInt32 w);
    };

  class Convert : public Object {
    protected:
    public:
      Convert();
      ~Convert();
      static char ToChar(char value);
      static char ToChar(const char* value);
      static char ToChar(String* value);
      static char ToChar(String value);
      static char ToChar(UInt8 value);
      static char ToChar(UInt16 value);
      static char ToChar(UInt32 value);
      static char ToChar(UInt64 value);
      static char ToChar(Int8 value);
      static char ToChar(Int16 value);
      static char ToChar(Int32 value);
      static char ToChar(Int64 value);
      static char ToChar(Double value);
      static char ToChar(Single value);
      static char ToChar(Fraction value);

      static Int8 ToInt8(char value);
      static Int8 ToInt8(const char* value);
      static Int8 ToInt8(String* value);
      static Int8 ToInt8(String value);
      static Int8 ToInt8(UInt8 value);
      static Int8 ToInt8(UInt16 value);
      static Int8 ToInt8(UInt32 value);
      static Int8 ToInt8(UInt64 value);
      static Int8 ToInt8(Int8 value);
      static Int8 ToInt8(Int16 value);
      static Int8 ToInt8(Int32 value);
      static Int8 ToInt8(Int64 value);
      static Int8 ToInt8(Double value);
      static Int8 ToInt8(Single value);
      static Int8 ToInt8(Fraction value);

      static Int16 ToInt16(char value);
      static Int16 ToInt16(const char* value);
      static Int16 ToInt16(String* value);
      static Int16 ToInt16(String value);
      static Int16 ToInt16(UInt8 value);
      static Int16 ToInt16(UInt16 value);
      static Int16 ToInt16(UInt32 value);
      static Int16 ToInt16(UInt64 value);
      static Int16 ToInt16(Int8 value);
      static Int16 ToInt16(Int16 value);
      static Int16 ToInt16(Int32 value);
      static Int16 ToInt16(Int64 value);
      static Int16 ToInt16(Double value);
      static Int16 ToInt16(Single value);
      static Int16 ToInt16(Fraction value);

      static Int32 ToInt32(char value);
      static Int32 ToInt32(const char* value);
      static Int32 ToInt32(String* value);
      static Int32 ToInt32(String value);
      static Int32 ToInt32(UInt8 value);
      static Int32 ToInt32(UInt16 value);
      static Int32 ToInt32(UInt32 value);
      static Int32 ToInt32(UInt64 value);
      static Int32 ToInt32(Int8 value);
      static Int32 ToInt32(Int16 value);
      static Int32 ToInt32(Int32 value);
      static Int32 ToInt32(Int64 value);
      static Int32 ToInt32(Double value);
      static Int32 ToInt32(Single value);
      static Int32 ToInt32(Fraction value);

      static Int64 ToInt64(char value);
      static Int64 ToInt64(const char* value);
      static Int64 ToInt64(String* value);
      static Int64 ToInt64(String value);
      static Int64 ToInt64(UInt8 value);
      static Int64 ToInt64(UInt16 value);
      static Int64 ToInt64(UInt32 value);
      static Int64 ToInt64(UInt64 value);
      static Int64 ToInt64(Int8 value);
      static Int64 ToInt64(Int16 value);
      static Int64 ToInt64(Int32 value);
      static Int64 ToInt64(Int64 value);
      static Int64 ToInt64(Double value);
      static Int64 ToInt64(Single value);
      static Int64 ToInt64(Fraction value);

      static UInt8 ToUInt8(char value);
      static UInt8 ToUInt8(const char* value);
      static UInt8 ToUInt8(String* value);
      static UInt8 ToUInt8(String value);
      static UInt8 ToUInt8(UInt8 value);
      static UInt8 ToUInt8(UInt16 value);
      static UInt8 ToUInt8(UInt32 value);
      static UInt8 ToUInt8(UInt64 value);
      static UInt8 ToUInt8(Int8 value);
      static UInt8 ToUInt8(Int16 value);
      static UInt8 ToUInt8(Int32 value);
      static UInt8 ToUInt8(Int64 value);
      static UInt8 ToUInt8(Double value);
      static UInt8 ToUInt8(Single value);
      static UInt8 ToUInt8(Fraction value);

      static UInt16 ToUInt16(char value);
      static UInt16 ToUInt16(const char* value);
      static UInt16 ToUInt16(String* value);
      static UInt16 ToUInt16(String value);
      static UInt16 ToUInt16(UInt8 value);
      static UInt16 ToUInt16(UInt16 value);
      static UInt16 ToUInt16(UInt32 value);
      static UInt16 ToUInt16(UInt64 value);
      static UInt16 ToUInt16(Int8 value);
      static UInt16 ToUInt16(Int16 value);
      static UInt16 ToUInt16(Int32 value);
      static UInt16 ToUInt16(Int64 value);
      static UInt16 ToUInt16(Double value);
      static UInt16 ToUInt16(Single value);
      static UInt16 ToUInt16(Fraction value);

      static UInt32 ToUInt32(char value);
      static UInt32 ToUInt32(const char* value);
      static UInt32 ToUInt32(String* value);
      static UInt32 ToUInt32(String value);
      static UInt32 ToUInt32(UInt8 value);
      static UInt32 ToUInt32(UInt16 value);
      static UInt32 ToUInt32(UInt32 value);
      static UInt32 ToUInt32(UInt64 value);
      static UInt32 ToUInt32(Int8 value);
      static UInt32 ToUInt32(Int16 value);
      static UInt32 ToUInt32(Int32 value);
      static UInt32 ToUInt32(Int64 value);
      static UInt32 ToUInt32(Double value);
      static UInt32 ToUInt32(Single value);
      static UInt32 ToUInt32(Fraction value);

      static UInt64 ToUInt64(char value);
      static UInt64 ToUInt64(const char* value);
      static UInt64 ToUInt64(String* value);
      static UInt64 ToUInt64(String value);
      static UInt64 ToUInt64(UInt8 value);
      static UInt64 ToUInt64(UInt16 value);
      static UInt64 ToUInt64(UInt32 value);
      static UInt64 ToUInt64(UInt64 value);
      static UInt64 ToUInt64(Int8 value);
      static UInt64 ToUInt64(Int16 value);
      static UInt64 ToUInt64(Int32 value);
      static UInt64 ToUInt64(Int64 value);
      static UInt64 ToUInt64(Double value);
      static UInt64 ToUInt64(Single value);
      static UInt64 ToUInt64(Fraction value);

      static Byte ToByte(char value);
      static Byte ToByte(const char* value);
      static Byte ToByte(String* value);
      static Byte ToByte(String value);
      static Byte ToByte(UInt8 value);
      static Byte ToByte(UInt16 value);
      static Byte ToByte(UInt32 value);
      static Byte ToByte(UInt64 value);
      static Byte ToByte(Int8 value);
      static Byte ToByte(Int16 value);
      static Byte ToByte(Int32 value);
      static Byte ToByte(Int64 value);
      static Byte ToByte(Double value);
      static Byte ToByte(Single value);
      static Byte ToByte(Fraction value);

      static Word ToWord(char value);
      static Word ToWord(const char* value);
      static Word ToWord(String* value);
      static Word ToWord(String value);
      static Word ToWord(UInt8 value);
      static Word ToWord(UInt16 value);
      static Word ToWord(UInt32 value);
      static Word ToWord(UInt64 value);
      static Word ToWord(Int8 value);
      static Word ToWord(Int16 value);
      static Word ToWord(Int32 value);
      static Word ToWord(Int64 value);
      static Word ToWord(Double value);
      static Word ToWord(Single value);
      static Word ToWord(Fraction value);

      static Dword ToDword(char value);
      static Dword ToDword(const char* value);
      static Dword ToDword(String* value);
      static Dword ToDword(String value);
      static Dword ToDword(UInt8 value);
      static Dword ToDword(UInt16 value);
      static Dword ToDword(UInt32 value);
      static Dword ToDword(UInt64 value);
      static Dword ToDword(Int8 value);
      static Dword ToDword(Int16 value);
      static Dword ToDword(Int32 value);
      static Dword ToDword(Int64 value);
      static Dword ToDword(Double value);
      static Dword ToDword(Single value);
      static Dword ToDword(Fraction value);

      static Qword ToQword(char value);
      static Qword ToQword(const char* value);
      static Qword ToQword(String* value);
      static Qword ToQword(String value);
      static Qword ToQword(UInt8 value);
      static Qword ToQword(UInt16 value);
      static Qword ToQword(UInt32 value);
      static Qword ToQword(UInt64 value);
      static Qword ToQword(Int8 value);
      static Qword ToQword(Int16 value);
      static Qword ToQword(Int32 value);
      static Qword ToQword(Int64 value);
      static Qword ToQword(Double value);
      static Qword ToQword(Single value);
      static Qword ToQword(Fraction value);

      static Single ToSingle(char value);
      static Single ToSingle(const char* value);
      static Single ToSingle(String* value);
      static Single ToSingle(String value);
      static Single ToSingle(UInt8 value);
      static Single ToSingle(UInt16 value);
      static Single ToSingle(UInt32 value);
      static Single ToSingle(UInt64 value);
      static Single ToSingle(Int8 value);
      static Single ToSingle(Int16 value);
      static Single ToSingle(Int32 value);
      static Single ToSingle(Int64 value);
      static Single ToSingle(Double value);
      static Single ToSingle(Single value);
      static Single ToSingle(Fraction value);

      static Double ToDouble(char value);
      static Double ToDouble(const char* value);
      static Double ToDouble(String* value);
      static Double ToDouble(String value);
      static Double ToDouble(UInt8 value);
      static Double ToDouble(UInt16 value);
      static Double ToDouble(UInt32 value);
      static Double ToDouble(UInt64 value);
      static Double ToDouble(Int8 value);
      static Double ToDouble(Int16 value);
      static Double ToDouble(Int32 value);
      static Double ToDouble(Int64 value);
      static Double ToDouble(Double value);
      static Double ToDouble(Single value);
      static Double ToDouble(Fraction value);

      static String ToString(char value);
      static String ToString(const char* value);
      static String ToString(String* value);
      static String ToString(UInt8 value);
      static String ToString(UInt16 value);
      static String ToString(UInt32 value);
      static String ToString(UInt64 value);
      static String ToString(Int8 value);
      static String ToString(Int16 value);
      static String ToString(Int32 value);
      static String ToString(Int64 value);
      static String ToString(Double value);
      static String ToString(Single value);
      static String ToString(Boolean value);
      static String ToString(Fraction value);

      static String ToString(Int8 value,const char* format);
      static String ToString(Int8 value,String format);
      static String ToString(Int8 value,String* format);
      static String ToString(UInt8 value,const char* format);
      static String ToString(UInt8 value,String format);
      static String ToString(UInt8 value,String* format);
      static String ToString(Int16 value,const char* format);
      static String ToString(Int16 value,String format);
      static String ToString(Int16 value,String* format);
      static String ToString(UInt16 value,const char* format);
      static String ToString(UInt16 value,String format);
      static String ToString(UInt16 value,String* format);
      static String ToString(Int32 value,const char* format);
      static String ToString(Int32 value,String format);
      static String ToString(Int32 value,String* format);
      static String ToString(UInt32 value,const char* format);
      static String ToString(UInt32 value,String format);
      static String ToString(UInt32 value,String* format);
      static String ToString(Double value,const char* format);
      static String ToString(Double value,String format);
      static String ToString(Double value,String* format);
      static String ToString(Single value,const char* format);
      static String ToString(Single value,String format);
      static String ToString(Single value,String* format);

      static char* ToCharArray(char value);
      static char* ToCharArray(const char* value);
      static char* ToCharArray(String* value);
      static char* ToCharArray(UInt8 value);
      static char* ToCharArray(UInt16 value);
      static char* ToCharArray(UInt32 value);
      static char* ToCharArray(UInt64 value);
      static char* ToCharArray(Int8 value);
      static char* ToCharArray(Int16 value);
      static char* ToCharArray(Int32 value);
      static char* ToCharArray(Int64 value);
      static char* ToCharArray(Double value);
      static char* ToCharArray(Single value);
      static char* ToCharArray(Fraction value);

      static char* ToCharArray(char value,char* dest,UInt32 maxLen);
      static char* ToCharArray(const char* value,char* dest,UInt32 maxLen);
      static char* ToCharArray(String* value,char* dest,UInt32 maxLen);
      static char* ToCharArray(UInt8 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(UInt16 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(UInt32 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(UInt64 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Int8 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Int16 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Int32 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Int64 value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Double value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Single value,char* dest,UInt32 maxLen);
      static char* ToCharArray(Fraction value,char* dest,UInt32 maxLen);

      static Boolean ToBoolean(char value);
      static Boolean ToBoolean(const char* value);
      static Boolean ToBoolean(String* value);
      static Boolean ToBoolean(String value);
      static Boolean ToBoolean(UInt8 value);
      static Boolean ToBoolean(UInt16 value);
      static Boolean ToBoolean(UInt32 value);
      static Boolean ToBoolean(UInt64 value);
      static Boolean ToBoolean(Int8 value);
      static Boolean ToBoolean(Int16 value);
      static Boolean ToBoolean(Int32 value);
      static Boolean ToBoolean(Int64 value);
      static Boolean ToBoolean(Double value);
      static Boolean ToBoolean(Single value);
      static Boolean ToBoolean(Fraction value);

      static Int16 SwitchEndian(Int16 value);
      static Int32 SwitchEndian(Int32 value);
      static UInt16 SwitchEndian(UInt16 value);
      static UInt32 SwitchEndian(UInt32 value);

      static Byte    ToBitSize(UInt32 value);
      static char    ToHexChar(Byte v);
      static String* ToHexString(Byte* buffer, UInt32 len);

      static Byte    FromHexChar(char v);
      static List<Byte>* FromHexString(const char* str);
      static List<Byte>* FromHexString(String* str);

      static UInt8  BcdToUInt8(Byte* buffer, int bytes);
      static UInt16 BcdToUInt16(Byte* buffer, int bytes);
      static UInt32 BcdToUInt32(Byte* buffer, int bytes);
      static UInt64 BcdToUInt64(Byte* buffer, int bytes);
      static UInt8  BcdToUInt8(Byte value);
      static UInt8  BcdToUInt8(Word value);
      static UInt8  BcdToUInt8(Dword value);
      static UInt16 BcdToUInt16(Byte value);
      static UInt16 BcdToUInt16(Word value);
      static UInt16 BcdToUInt16(Dword value);
      static UInt32 BcdToUInt32(Byte value);
      static UInt32 BcdToUInt32(Word value);
      static UInt32 BcdToUInt32(Dword value);

      static UInt32 UIntToBcd(Byte digits, UInt64 value, Byte* dest);

      static Double AcresToHectares(Double d);
      static Double AcresToSquareFeet(Double d);
      static Double AcresToSquareMeters(Double d);
      static Double AcresToSquareYards(Double d);
      static Double CelsiusToFahrenheit(Double d);
      static Double CelsiusToKelvin(Double d);
      static Double CentimetersToInches(Double d);
      static Double CubicCentimetersToCubicInches(Double d);
      static Double CubicInchesToCubicCentimeters(Double d);
      static Double CubicInchesToGallons(Double d);
      static Double CubicInchesToLiters(Double d);
      static Double CubicInchesToOunces(Double d);
      static Double CubicInchesToPints(Double d);
      static Double CubicInchesToQuarts(Double d);
      static Double FahrenheitToCelsius(Double d);
      static Double FahrenheitToKelvin(Double d);
      static Double FathomsToFeet(Double d);
      static Double FathomsToMeters(Double d);
      static Double FeetToMeters(Double d);
      static Double FurlongsToFeet(Double d);
      static Double FurlongsToMeters(Double d);
      static Double GallonsToCubicInches(Double d);
      static Double GallonsToLiters(Double d);
      static Double GallonsToOunces(Double d);
      static Double GallonsToPints(Double d);
      static Double GallonsToQuarts(Double d);
      static Double GramsToKilograms(Double d);
      static Double GramsToOunces(Double d);
      static Double GramsToPounds(Double d);
      static Double HectaresToAcres(Double d);
      static Double HectaresToSquareFeet(Double d);
      static Double HectaresToSquareMeters(Double d);
      static Double HectaresToSquareYards(Double d);
      static Double InchesToCentimeters(Double d);
      static Double InchesToMeters(Double d);
      static Double KelvinToCelsius(Double d);
      static Double KelvinToFahrenheit(Double d);
      static Double KilogramsToGrams(Double d);
      static Double KilogramsToOunces(Double d);
      static Double KilogramsToPounds(Double d);
      static Double KilometersToMeters(Double d);
      static Double KilometersToMiles(Double d);
      static Double KilometersToNauticalMiles(Double d);
      static Double KilometersToYards(Double d);
      static Double KilometersToFeet(Double d);
      static Double LeaguesToMeters(Double d);
      static Double LightYearsToKilometers(Double d);
      static Double LightYearsToMiles(Double d);
      static Double LightYearsToParsecs(Double d);
      static Double LitersToCubicInches(Double d);
      static Double LitersToGallons(Double d);
      static Double LitersToOunces(Double d);
      static Double LitersToPints(Double d);
      static Double LitersToQuarts(Double d);
      static Double MetersToYards(Double d);
      static Double MetersToFathoms(Double d);
      static Double MetersToFeet(Double d);
      static Double MetersToFurlongs(Double d);
      static Double MetersToInches(Double d);
      static Double MetersToLeagues(Double d);
      static Double MetersToMiles(Double d);
      static Double MilesToKilometers(Double d);
      static Double MilesToMeters(Double d);
      static Double MilesToNauticalMiles(Double d);
      static Double MilesToYards(Double d);
      static Double MilesToFeet(Double d);
      static Double NauticalMilesToFeet(Double d);
      static Double NauticalMilesToMeters(Double d);
      static Double NauticalMilesToMiles(Double d);
      static Double NauticalMilesToKilometers(Double d);
      static Double NauticalMilesToYards(Double d);
      static Double OuncesToCubicInches(Double d);
      static Double OuncesToGallons(Double d);
      static Double OuncesToGrams(Double d);
      static Double OuncesToKilograms(Double d);
      static Double OuncesToLiters(Double d);
      static Double OuncesToPints(Double d);
      static Double OuncesToPounds(Double d);
      static Double OuncesToQuarts(Double d);
      static Double ParsecToKilometers(Double d);
      static Double ParsecToLightYears(Double d);
      static Double ParsecToMiles(Double d);
      static Double PintsToCubicInches(Double d);
      static Double PintsToGallons(Double d);
      static Double PintsToLiters(Double d);
      static Double PintsToOunces(Double d);
      static Double PintsToQuarts(Double d);
      static Double PoundsToGrams(Double d);
      static Double PoundsToKilograms(Double d);
      static Double PoundsToOunces(Double d);
      static Double PoundsToStones(Double d);
      static Double QuartsToCubicInches(Double d);
      static Double QuartsToGallons(Double d);
      static Double QuartsToLiters(Double d);
      static Double QuartsToOunces(Double d);
      static Double QuartsToPints(Double d);
      static Double SquareCentimetersToSquareInches(Double d);
      static Double SquareCentimetersToSquareFeet(Double d);
      static Double SquareCentimetersToSquareMeters(Double d);
      static Double SquareCentimetersToSquareYards(Double d);
      static Double SquareFeetToSquareCentimeters(Double d);
      static Double SquareFeetToSquareInches(Double d);
      static Double SquareFeetToSquareMeters(Double d);
      static Double SquareFeetToSquareYards(Double d);
      static Double SquareInchesToSquareCentimeters(Double d);
      static Double SquareInchesToSquareFeet(Double d);
      static Double SquareInchesToSquareMeters(Double d);
      static Double SquareInchesToSquareYards(Double d);
      static Double SquareKilometersToSquareMeters(Double d);
      static Double SquareKilometersToSquareMiles(Double d);
      static Double SquareKilometersToSquareYards(Double d);
      static Double SquareMetersToSquareCentimeters(Double d);
      static Double SquareMetersToSquareFeet(Double d);
      static Double SquareMetersToSquareInches(Double d);
      static Double SquareMetersToSquareYards(Double d);
      static Double SquareMilesToSquareKilometers(Double d);
      static Double SquareMilesToSquareMeters(Double d);
      static Double SquareMilesToSquareYards(Double d);
      static Double SquareYardsToSquareCentimeters(Double d);
      static Double SquareYardsToSquareFeet(Double d);
      static Double SquareYardsToSquareInches(Double d);
      static Double SquareYardsToSquareMeters(Double d);
      static Double StonesToPounds(Double d);
      static Double YardsToMeters(Double d);

      };

  class System : public Object {
    protected:
    public:
      System();
      ~System();
      static String  DomainName();
      static int     GroupID();
      static String  GroupName();
      static String  GroupName(int groupID);
      static List<int>* Groups();
      static String  HostName();
      static String  Machine();
      static List<String*>* Members(int groupID);
      static String  OperatingSystem();
      static String  Release();
      static int     UserID();
      static String  UserName();
      static String  Version();
    };

  class Environment : public Object {
    protected:
      char** environment;
    public:
      Environment(char** envp);
      ~Environment();
      String Get(const char* name);
    };

  class Terminal : public Object {
    protected:
      UInt32         background;
      bool           bold;
      bool           buffered;
      bool           cursorShown;
      bool           echo;
      UInt32         foreground;
      bool           inverse;
      bool           underline;
      UInt32         width;
      struct termios original;
      void           setAttributes();
    public:
      Terminal();
      virtual ~Terminal();
      static const UInt32 Black   = 0;
      static const UInt32 Red     = 1;
      static const UInt32 Green   = 2;
      static const UInt32 Yellow  = 3;
      static const UInt32 Blue    = 4;
      static const UInt32 Magenta = 5;
      static const UInt32 Cyan    = 6;
      static const UInt32 White   = 7;
      static const UInt32 Key_F1               = 0x8001;
      static const UInt32 Key_F2               = 0x8002;
      static const UInt32 Key_F3               = 0x8003;
      static const UInt32 Key_F4               = 0x8004;
      static const UInt32 Key_F5               = 0x8005;
      static const UInt32 Key_F6               = 0x8006;
      static const UInt32 Key_F7               = 0x8007;
      static const UInt32 Key_F8               = 0x8008;
      static const UInt32 Key_F9               = 0x8009;
      static const UInt32 Key_F10              = 0x800a;
      static const UInt32 Key_F11              = 0x800b;
      static const UInt32 Key_F12              = 0x800c;
      static const UInt32 Key_Shift_F1         = 0x8011;
      static const UInt32 Key_Shift_F2         = 0x8012;
      static const UInt32 Key_Shift_F3         = 0x8013;
      static const UInt32 Key_Shift_F4         = 0x8014;
      static const UInt32 Key_Shift_F5         = 0x8015;
      static const UInt32 Key_Shift_F6         = 0x8016;
      static const UInt32 Key_Shift_F7         = 0x8017;
      static const UInt32 Key_Shift_F8         = 0x8018;
      static const UInt32 Key_Shift_F9         = 0x8019;
      static const UInt32 Key_Shift_F10        = 0x801a;
      static const UInt32 Key_Shift_F11        = 0x801b;
      static const UInt32 Key_Shift_F12        = 0x801c;
      static const UInt32 Key_Alt_F1           = 0x8021;
      static const UInt32 Key_Alt_F2           = 0x8022;
      static const UInt32 Key_Alt_F3           = 0x8023;
      static const UInt32 Key_Alt_F4           = 0x8024;
      static const UInt32 Key_Alt_F5           = 0x8025;
      static const UInt32 Key_Alt_F6           = 0x8026;
      static const UInt32 Key_Alt_F7           = 0x8027;
      static const UInt32 Key_Alt_F8           = 0x8028;
      static const UInt32 Key_Alt_F9           = 0x8029;
      static const UInt32 Key_Alt_F10          = 0x802a;
      static const UInt32 Key_Alt_F11          = 0x802b;
      static const UInt32 Key_Alt_F12          = 0x802c;
      static const UInt32 Key_Ctrl_F1          = 0x8041;
      static const UInt32 Key_Ctrl_F2          = 0x8042;
      static const UInt32 Key_Ctrl_F3          = 0x8043;
      static const UInt32 Key_Ctrl_F4          = 0x8044;
      static const UInt32 Key_Ctrl_F5          = 0x8045;
      static const UInt32 Key_Ctrl_F6          = 0x8046;
      static const UInt32 Key_Ctrl_F7          = 0x8047;
      static const UInt32 Key_Ctrl_F8          = 0x8048;
      static const UInt32 Key_Ctrl_F9          = 0x8049;
      static const UInt32 Key_Ctrl_F10         = 0x804a;
      static const UInt32 Key_Ctrl_F11         = 0x804b;
      static const UInt32 Key_Ctrl_F12         = 0x804c;
      static const UInt32 Key_Insert           = 0x8101;
      static const UInt32 Key_Delete           = 0x8102;
      static const UInt32 Key_Home             = 0x8103;
      static const UInt32 Key_End              = 0x8104;
      static const UInt32 Key_PageUp           = 0x8105;
      static const UInt32 Key_PageDn           = 0x8106;
      static const UInt32 Key_UpArrow          = 0x8107;
      static const UInt32 Key_DownArrow        = 0x8108;
      static const UInt32 Key_LeftArrow        = 0x8109;
      static const UInt32 Key_RightArrow       = 0x810a;
      static const UInt32 Key_Keypad5          = 0x810b;
      static const UInt32 Key_Ctrl_Insert      = 0x8141;
      static const UInt32 Key_Ctrl_Delete      = 0x8142;
      static const UInt32 Key_Ctrl_Home        = 0x8143;
      static const UInt32 Key_Ctrl_End         = 0x8144;
      static const UInt32 Key_Ctrl_PageUp      = 0x8145;
      static const UInt32 Key_Ctrl_PageDn      = 0x8146;
      static const UInt32 Key_Ctrl_UpArrow     = 0x8147;
      static const UInt32 Key_Ctrl_DownArrow   = 0x8148;
      static const UInt32 Key_Ctrl_LeftArrow   = 0x8149;
      static const UInt32 Key_Ctrl_RightArrow  = 0x814a;
      static const UInt32 Key_Alt_Insert       = 0x8121;
      static const UInt32 Key_Alt_Delete       = 0x8122;
      static const UInt32 Key_Alt_Home         = 0x8123;
      static const UInt32 Key_Alt_End          = 0x8124;
      static const UInt32 Key_Alt_PageUp       = 0x8125;
      static const UInt32 Key_Alt_PageDn       = 0x8126;
      static const UInt32 Key_Alt_UpArrow      = 0x8127;
      static const UInt32 Key_Alt_DownArrow    = 0x8128;
      static const UInt32 Key_Alt_LeftArrow    = 0x8129;
      static const UInt32 Key_Alt_RightArrow   = 0x812a;
      static const UInt32 Key_Shift_Insert     = 0x8111;
      static const UInt32 Key_Shift_Delete     = 0x8112;
      static const UInt32 Key_Shift_Home       = 0x8113;
      static const UInt32 Key_Shift_End        = 0x8114;
      static const UInt32 Key_Shift_PageUp     = 0x8115;
      static const UInt32 Key_Shift_PageDn     = 0x8116;
      static const UInt32 Key_Shift_UpArrow    = 0x8117;
      static const UInt32 Key_Shift_DownArrow  = 0x8118;
      static const UInt32 Key_Shift_LeftArrow  = 0x8119;
      static const UInt32 Key_Shift_RightArrow = 0x811a;
      static const UInt32 Key_Shift_Tab        = 0x811b;
      virtual void   Background(UInt32 i);
      virtual void   Bold(bool b);
      virtual void   Box(UInt32 x, UInt32 y, UInt32 w, UInt32 h);
      virtual void   Buffered(bool b);
      virtual void   Center(const char* msg);
      virtual void   Center(String* msg);
      virtual void   ClearScreen();
      virtual void   Echo(bool b);
      virtual void   Flush();
      virtual void   Foreground(UInt32 i);
      virtual UInt32 Inkey();
      virtual Byte   Input(char* buffer, UInt32 maxlen);
      virtual Byte   InputAt(UInt32 x,UInt32 y,char* buffer, UInt32 maxlen);
      virtual void   Inverse(bool b);
      virtual bool   KeyPressed();
      virtual void   Position(UInt32 x, UInt32 y);
      virtual void   ShowCursor(bool b);
      virtual void   Underline(bool b);
      virtual UInt32 Width();
      virtual UInt32 Width(UInt32 i);
      virtual void   WrapAround(bool b);
      virtual void   Write(const char* msg);
      virtual void   Write(String* msg);
      virtual void   WriteAt(UInt32 x,UInt32 y,const char* msg);
      virtual void   WriteAt(UInt32 x,UInt32 y,String* msg);
      virtual void   WriteLn(const char* msg);
      virtual void   WriteLn(String* msg);
      virtual void   WriteLnAt(UInt32 x,UInt32 y,const char* msg);
      virtual void   WriteLnAt(UInt32 x,UInt32 y,String* msg);
    };

  class Math : public Object {
    protected:
    public:
      static constexpr Double E = 2.7182818284590452354;
      static constexpr Double PI = 3.14159265358979323846;
      static Int8 Abs(Int8 n);
      static Int16 Abs(Int16 n);
      static Int32 Abs(Int32 n);
      static Int64 Abs(Int64 n);
      static Single Abs(Single n);
      static Double Abs(Double n);
      static double ACos(double n);
      static double ACosH(double n);
      static double ASin(double n);
      static double ASinH(double n);
      static double ATan(double n);
      static double ATanH(double n);
      static Int8   Aug(Int8 n);
      static Int16  Aug(Int16 n);
      static Int32  Aug(Int32 n);
      static Int64  Aug(Int64 n);
      static Single Aug(Single n);
      static Double Aug(Double n);
      static Int32  Ceiling(Single n);
      static Int32  Ceiling(Double n);
      static double Cos(double angle);
      static double CosH(double angle);
      static double DegToRad(double deg);
      static Int8   Dim(Int8 n);
      static Int16  Dim(Int16 n);
      static Int32  Dim(Int32 n);
      static Int64  Dim(Int64 n);
      static Single Dim(Single n);
      static Double Dim(Double n);
      static double Exp(double n);
      static double Exp10(double n);
      static Int32  Floor(Single n);
      static Int32  Floor(Double n);
      static double Log(double n);
      static double Log10(double n);
      static Int8   Max(Int8 a, Int8 b);
      static Int16  Max(Int16 a, Int16 b);
      static Int32  Max(Int32 a, Int32 b);
      static Int64  Max(Int64 a, Int64 b);
      static UInt8  Max(UInt8 a, UInt8 b);
      static UInt16 Max(UInt16 a, UInt16 b);
      static UInt32 Max(UInt32 a, UInt32 b);
      static UInt64 Max(UInt64 a, UInt64 b);
      static Single Max(Single a, Single b);
      static Double Max(Double a, Double b);
      static Int8   Min(Int8 a, Int8 b);
      static Int16  Min(Int16 a, Int16 b);
      static Int32  Min(Int32 a, Int32 b);
      static Int64  Min(Int64 a, Int64 b);
      static UInt8  Min(UInt8 a, UInt8 b);
      static UInt16 Min(UInt16 a, UInt16 b);
      static UInt32 Min(UInt32 a, UInt32 b);
      static UInt64 Min(UInt64 a, UInt64 b);
      static Single Min(Single a, Single b);
      static Double Min(Double a, Double b);
      static double Pow(double x, double y);
      static double RadToDeg(double rad);
      static double Ratio(double start, double end, double index);
      static double Root(double n, double rt);
      static Int32  Round(Single n);
      static Int32  Round(Double n);
      static double Sign(double n);
      static double Sin(double angle);
      static double SinH(double angle);
      static double Sqr(double n);
      static double Sqrt(double n);
      static double Tan(double n);
      static double TanH(double n);
      static Int32  Trunc(Single n);
      static Int32  Trunc(Double n);
    };

  class ComplexNumber : public Object {
    protected:
      double real;
      double imaginary;
    public:
      ComplexNumber(double r, double i);
      virtual ~ComplexNumber();
      virtual void           operator = (const ComplexNumber& a);
      virtual Boolean        operator == (const ComplexNumber& a);
      virtual Boolean        operator != (const ComplexNumber& a);
      virtual ComplexNumber  operator + (const ComplexNumber& a);
      virtual ComplexNumber  operator - (const ComplexNumber& a);
      virtual ComplexNumber  operator * (const ComplexNumber& a);
      virtual ComplexNumber  operator / (const ComplexNumber& a);
      virtual ComplexNumber  operator += (const ComplexNumber& a);
      virtual ComplexNumber  operator -= (const ComplexNumber& a);
      virtual ComplexNumber  operator *= (const ComplexNumber& a);
      virtual ComplexNumber  operator /= (const ComplexNumber& a);
      virtual double         Abs();
      virtual ComplexNumber  Conj();
      virtual ComplexNumber  Cos();
      virtual ComplexNumber  Cosec();
      virtual ComplexNumber  Cotan();
      virtual ComplexNumber  Exp();
      virtual double         Imaginary();
      virtual ComplexNumber  Log();
      virtual ComplexNumber  Neg();
      virtual ComplexNumber  Pow(ComplexNumber b);
      virtual double         Real();
      virtual ComplexNumber  Sec();
      virtual ComplexNumber  Sin();
      virtual ComplexNumber  Sqr();
      virtual ComplexNumber  Sqrt();
      virtual ComplexNumber  Tan();
      virtual ComplexNumber  Add(ComplexNumber b);
      virtual ComplexNumber  Sub(ComplexNumber b);
      virtual ComplexNumber  Mul(ComplexNumber b);
      virtual ComplexNumber  Div(const ComplexNumber b);
      virtual String         ToString();
    };

  class Vector : public Object {
    protected:
      Double x;
      Double y;
      Double z;
    public:
      Vector();
      Vector(double x, double y, double z);
      virtual ~Vector();
      virtual Vector operator+ (const Vector& a);
      virtual Vector operator- (const Vector& a);
      virtual Vector operator* (const double& a);
      virtual void   operator+= (const Vector& a);
      virtual void   operator-= (const Vector& a);
      virtual void   operator*= (const double& a);
      virtual Vector Cross(const Vector& b);
      virtual Double Dot(const Vector& a);
      virtual Double Length();
      virtual Vector Neg();
      virtual Vector Norm();
      virtual Vector Scale(double scale);
      virtual Double X(double v);
      virtual Double Y(double v);
      virtual Double Z(double v);
      virtual Double X();
      virtual Double Y();
      virtual Double Z();
    };

  class Point : public Object {
    protected:
      double x;
      double y;
    public:
      Point();
      Point(double x,double y);
      virtual ~Point();
      virtual Point Translate(double deltaX, double deltaY);
      virtual Point Translate(Point delta);
      virtual Point Translate(Point* delta);
      virtual double X();
      virtual double Y();
    };

  class Line : public Object {
    protected:
      Point start;
      Point end;
      Double slope;
      Double offset;
      virtual void computeStats();
    public:
      Line();
      Line(double x1, double y1, double x2, double y2);
      Line(Point s, Point e);
      Line(Point* s, Point* e);
      virtual ~Line();
      virtual Point   Center();
      virtual Point   End();
      virtual double  Height();
      virtual Boolean Intersects(Line *b);
      virtual Double  IntersectPos(Line *b);
      virtual double  Length();
      virtual double  Offset();
      virtual void    PointToRef(Point p, Double* dist, Double* frac);
      virtual Point   RefToPoint(Double dist, Double fract);
      virtual double  Slope();
      virtual Point   Start();
      virtual Line    Translate(double deltaX, double deltaY);
      virtual Line    Translate(Point delta);
      virtual Line    Translate(Point* delta);
      virtual double  Width();
    };

  class Rectangle : public Object {
    protected:
      Point  start;
      double width;
      double height;
    public:
      Rectangle();
      Rectangle(double x, double y, double width, double height);
      Rectangle(Point start, Point end);
      Rectangle(Point* start, Point* end);
      virtual ~Rectangle();
      virtual Double Area();
      virtual Point  Center();
      virtual double Height();
      virtual Point  Start();
      virtual Rectangle Translate(double deltaX, double deltaY);
      virtual Rectangle Translate(Point delta);
      virtual Rectangle Translate(Point* delta);
      virtual double Width();
      virtual double X();
      virtual double Y();
      };

  class Polygon : public Object {
    protected:
      List<Point*>* points;
      UInt32        pos;
    public:
      Polygon();
      virtual ~Polygon();
      virtual UInt32        Add(Point p);
      virtual Boolean       AtEnd();
      virtual UInt32        Count();
      virtual Line          First();
      virtual Line          Next();
      virtual Double        Perimeter();
      virtual List<Point*>* Points();
      virtual void          Reset();
    };
  
  class Length : public Object {
    protected:
      Double value;
      Double convert(Double v, Byte unit);
    public:
      static constexpr Double METERS = 0;
      static constexpr Double KILOMETERS = 1;
      static constexpr Double CENTIMETERS = 2;
      static constexpr Double INCHES = 3;
      static constexpr Double FEET = 4;
      static constexpr Double YARDS = 5;
      static constexpr Double MILES = 6;
      static constexpr Double LEAGUES = 7;
      static constexpr Double FATHOMS = 8;
      static constexpr Double FURLONGS = 9;
      Length();
      Length(Double v, Byte unit);
      ~Length();
      virtual Length operator+ (const Length& a);
      virtual Length operator- (const Length& a);
      virtual Length operator* (const Length& a);
      virtual Length operator/ (const Length& a);
      virtual Length operator+= (const Length& a);
      virtual Length operator-= (const Length& a);
      virtual Length operator*= (const Length& a);
      virtual Length operator/= (const Length& a);
      virtual Boolean operator> (const Length& a);
      virtual Boolean operator< (const Length& a);
      virtual Boolean operator>= (const Length& a);
      virtual Boolean operator<= (const Length& a);
      virtual Boolean operator== (const Length& a);
      virtual Boolean operator!= (const Length& a);
      Length  Add(Length a);
      Length  Add(Double v, Byte unit);
      Double  AsCentimeters();
      Double  AsFathoms();
      Double  AsFeet();
      Double  AsFurlongs();
      Double  AsInches();
      Double  AsKilometers();
      Double  AsLeagues();
      Double  AsMeters();
      Double  AsMiles();
      Double  AsYards();
      Length  Div(Length a);
      Length  Div(Double v, Byte unit);
      Boolean Equals(Length* b);
      Length  Mul(Length a);
      Length  Mul(Double v, Byte unit);
      Length  Sub(Length a);
      Length  Sub(Double v, Byte unit);
    };

  class Lfsr : public Object {
    protected:
      UInt32 seed;
      UInt32 taps;
    public:
      Lfsr();
      Lfsr(UInt32 s, UInt32 t);
      virtual ~Lfsr();
      virtual void Shift(int count);
      virtual UInt32 Next(int bits);
      virtual Byte   NextByte();
      virtual Word   NextWord();
      virtual Dword  NextDword();
      virtual Qword  NextQword();
    };

  class Random : public Object {
    protected:
      Lfsr   clocking;
      Lfsr   rng;
      UInt32 bitsize(UInt32 n);
    public:
      Random();
      Random(UInt32 ss);
      virtual ~Random();
      virtual UInt32 Next(UInt32 range);
    };

  class NeuralNetwork : public Object {
    protected:
      UInt32   numInputNodes;
      UInt32   numHiddenNodes;
      UInt32   numOutputNodes;
      Double   learningRate;
      Double **inputWeights;
      Double  *inputValues;
      Double **hiddenWeights;
      Double  *hiddenValues;
      Double  *hiddenThresholds;
      Double  *outputValues;
      Double  *outputThresholds;
      Double  *expectedValues;
      void     activateNetwork();
      void     cleanStorage();
      void     setupNodes();
      Double   updateWeights();
    public:
      NeuralNetwork(UInt32 inputs, UInt32 hidden, UInt32 outputs);
      virtual ~NeuralNetwork();
      virtual List<Double>* Execute(List<Double>* inputs);
      virtual Double LearningRate();
      virtual Double LearningRate(Double d);
      virtual UInt32 NumInputNodes();
      virtual UInt32 NumHiddenNodes();
      virtual UInt32 NumOutputNodes();
      virtual void   SetTopography(UInt32 inputs, UInt32 hidden, UInt32 outputs);
    };

class JsonBase {
  protected:
    Byte jsonType;
  public:
    JsonBase();
    virtual ~JsonBase();
    virtual Byte    JsonType();
    virtual String* ToString();
  };

class JsonNull : public JsonBase {
  protected:
  public:
    JsonNull();
    virtual ~JsonNull();
    virtual String* ToString();
  };

class JsonBoolean : public JsonBase {
  protected:
    Boolean value;
  public:
    JsonBoolean(Boolean b);
    JsonBoolean(char* b);
    JsonBoolean(String b);
    JsonBoolean(String *b);
    ~JsonBoolean();
    virtual String* ToString();
    virtual Boolean Value();
  };
class JsonNumber : public JsonBase {
  protected:
    Int32  ivalue;
    Int64  lvalue;
    Double dvalue;
  public:
    JsonNumber(UInt32);
    JsonNumber(Int32);
    JsonNumber(UInt64);
    JsonNumber(Int64);
    JsonNumber(Double);
    virtual ~JsonNumber();
    virtual Int32   AsInteger();
    virtual Double  AsDouble();
    virtual String* ToString();
  };

class JsonString : public JsonBase {
  protected:
    String *value;
  public:
    JsonString(char* s);
    JsonString(String s);
    JsonString(String *s);
    virtual ~JsonString();
    virtual String *ToString();
    virtual String *Value();
  };
class JsonArray : public JsonBase {
  protected:
    List<JsonBase*> *items;
  public:
    JsonArray();
    JsonArray(const char* s);
    ~JsonArray();
    void Add(Boolean value);
    void Add(Int32 value);
    void Add(UInt32 value);
    void Add(Int64 value);
    void Add(UInt64 value);
    void Add(Double value);
    void Add(const char* value);
    void Add(String value);
    void Add(String *value);
    void Add(JsonBase* obj);
    JsonBase* At(UInt32 i);
    UInt32 Count();
    virtual String* ToString();
  };

class Json : public JsonBase {
  protected:
    List<String*> *keys;
    List<JsonBase*> *values;
  public:
    static const int NULLOBJ = 0;
    static const int NUMBER  = 1;
    static const int STRING  = 2;
    static const int BOOLEAN = 3;
    static const int ARRAY   = 4;
    static const int OBJECT  = 5;
    Json();
    Json(const char* s);
    virtual ~Json();
    void Add(const char* key, Boolean value);
    void Add(const char* key, Int32 value);
    void Add(const char* key, UInt32 value);
    void Add(const char* key, Int64 value);
    void Add(const char* key, UInt64 value);
    void Add(const char* key, Double value);
    void Add(const char* key, const char* value);
    void Add(const char* key, String value);
    void Add(const char* key, String *value);
    void Add(const char* key, JsonBase* obj);
    JsonBase* Get(const char* key);
    virtual String* ToString();
  };

class HttpRequest {
  protected:
    Byte           method;
    String        *host;
    String        *uri;
    UInt32         port;
    String        *protocol;
    UInt32         httpResult;
    String        *body;
    Boolean        debug;
    List<String*> *requestHeaders;
    List<String*> *requestHeaderValues;
    List<String*> *responseHeaders;
    List<String*> *responseHeaderValues;
    char          *_readLine(char* input, char* buffer);
  public:
    static const int POST = 1;
    static const int GET = 2;
    HttpRequest(const char* uri, int p);
    ~HttpRequest();
    void AddRequestHeader(const char* key, const char* value);
    String* Body();
    Boolean Debug();
    Boolean Debug(Boolean b);
    Byte Method();
    Byte Method(Byte i);
    String* Protocol();
    int Send(const char* message);
    UInt32 Result();
  };

class AssociativeArray : public Object {
  protected:
    List<String*> *keys;
    List<Object*> *values;
  public:
    AssociativeArray();
    ~AssociativeArray();
    void    Add(char* s, Object* o);
    void    Add(String s, Object* o);
    void    Add(String* s, Object* o);
    UInt32  Count();
    Object *At(char* s);
    Object *At(String s);
    Object *At(String *s);
    void    AtPut(char* s, Object* o);
    void    AtPut(String s, Object* o);
    void    AtPut(String *s, Object* o);
  };

class HttpListener;

class HttpServer : public Object {
  typedef struct {
    char **keys;
    char **values;
    UInt32 count;
    } DICT;
  protected:
    char  **rHeaderKeys;
    char  **rHeaderValues;
    UInt32  numRHeaders;
    char  **headerKeys;
    char  **headerValues;
    UInt32  numHeaders;
    char  **getKeys;
    char  **getValues;
    UInt32  numGet;
    char  **postKeys;
    char  **postValues;
    UInt32  numPost;
    char   *body;
    char    method[16];
    char   *resource;
    UInt16  port;
    char   *protocol;
    UInt32  responseCode;
    char    responseDescription[32];
    Socket *server;
    Boolean terminate;
    HttpListener* listener;
    void    _clearData();
    DICT    _decodeParameters(Byte* message);
    Int32   _parse(Byte* message, UInt32 length);
  public:
    HttpServer();
    ~HttpServer();
    void    AddResponseHeader(const char* key, const char* value);
    char   *Body();
    char   *Get(const char* key);
    char   *Get(UInt32 pos);
    char   *GetKey(UInt32 pos);
    UInt32  GetCount();
    char   *Header(const char* key);
    char   *Header(UInt32 pos);
    char   *HeaderKey(UInt32 pos);
    UInt32  HeaderCount();
    void    Listener(HttpListener* l);
    char   *Method();
    UInt16  Port();
    UInt16  Port(UInt16 i);
    char   *Post(const char* key);
    char   *Post(UInt32 pos);
    char   *PostKey(UInt32 pos);
    UInt32  PostCount();
    char   *Protocol();
    char   *Resource();
    void    SetResponseCode(UInt32 code, const char* description);
    void    Start();
  };

class HttpListener : public Object {
  public:
    HttpListener();
    virtual ~HttpListener();
    virtual Byte* Request(HttpServer* server, UInt32 *resultLength);
  };

#include <SmrArray.h>
#include <SmrList.h>
#include <SmrSet.h>
#include <SmrStack.h>
#include <SmrQueue.h>
#include <SmrDictionary.h>

  }

#endif

