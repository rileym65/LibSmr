#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SmrFramework.h>

using namespace SmrFramework;

class JpegBitStream : public BitStream {
  protected:
  public:
    JpegBitStream(Stream* s) : BitStream(s) { }
    Dword Read(Byte bits) {
      UInt32 i;
      Dword  ret;
      Dword  bit;
      ret = 0;
      bit = 1;
      for (i=0; i<bits; i++) {
        if (mask == 0) {
          currentByte = stream->ReadByte();
          if (currentByte == 255) {
            if (stream->ReadByte() != 0)
              currentByte = stream->ReadByte();
            }
          mask = (littleEndian) ? 1 : 128;
          }
        if (!fillLowToHigh) ret <<= 1;
        if (currentByte & mask) ret |= (fillLowToHigh) ? bit : 1;
        if (littleEndian) mask <<= 1; else mask >>= 1;
        bit <<= 1;
        }
      return ret;
      }
  };

#define IMAGE_BILEVEL          1
#define IMAGE_GRAY             2
#define IMAGE_PALETTE          3
#define IMAGE_RGB16            4
#define IMAGE_RGB24            5
#define IMAGE_RGB32            6

#define JPEG_APP0     0xe0
#define JPEG_APPF     0xef
#define JPEG_DHT      0xc4
#define JPEG_EOI      0xd9
#define JPEG_DQT      0xdb
#define JPEG_SOF0     0xc0
#define JPEG_SOF1     0xc1
#define JPEG_SOF2     0xc2
#define JPEG_SOF3     0xc3
#define JPEG_SOF5     0xc5
#define JPEG_SOF6     0xc6
#define JPEG_SOF7     0xc7
#define JPEG_SOF9     0xc9
#define JPEG_SOF10    0xc10
#define JPEG_SOF11    0xc11
#define JPEG_SOF13    0xc13
#define JPEG_SOF14    0xc14
#define JPEG_SOF15    0xc15
#define JPEG_SOI      0xd8
#define JPEG_SOS      0xda
#define JPEG_DRI      0xdd
#define JPEG_COM      0xfe


typedef struct {
  char   id[5];
  Byte   majorVersion;
  Byte   minorVersion;
  Byte   units;
  Byte   densityX[2];
  Byte   densityY[2];
  Byte   thumbX;
  Byte   thumbY;
  } JFIF_HEADER;

typedef struct {
  Byte   bitsPerSample;
  Byte   height[2];
  Byte   width[2];
  Byte   components;
  } JFIF_SOF;

typedef struct {
  Byte   hSampling;
  Byte   vSampling;
  Byte   qt;
  } JFIF_SOF_COMP;
typedef struct {
  Byte   compNumber;
  Byte   dcHuffTable;
  Byte   acHuffTable;
  } JFIF_SCAN_COMP;

typedef struct {
  Byte   flags;
  Byte   counts[16];
  } JFIF_DHT_HEADER;

typedef struct {
  Byte   selectionStart;
  Byte   selectionEnd;
  Byte   approx;
  } JFIF_SPECTRAL;

typedef Int32 BLOCK[8][8];

typedef struct {
  Byte*          y;                 /* Storage for decoded y component */
  Byte*          cb;                /* Storage for decoded Cb component */
  Byte*          cr;                /* Storage for decoded Cr component */
  Word           qt[4][64];         /* Storage for up to 4 quantize tables */
  Huffman*       dcht[4];           /* Up to 4 dc huffman tables */
  Huffman*       acht[4];           /* Up to 4 ac huffman tables */
  BLOCK*         yCoef;             /* Storage for y coefficients */
  BLOCK*         cBCoef;            /* Storage for Cb coefficients */
  BLOCK*         cRCoef;            /* Storage for Cr coefficients */
  Dword          width;             /* image width */
  Dword          height;            /* image height */
  Byte           compCount;         /* components in image */
  Byte           bitsPerSample;     /* sample size, 8 or 12 */
  Byte           fxMax;             /* max h sampling in image */
  Byte           fyMax;             /* max v sampling in image */
  Byte           pixelsX[4];        /* DU h pixels per component in scan */
  Byte           pixelsY[4];        /* DU v pixels per component in scan */
  Dword          dusX[4];           /* number of h dataunits in scan */
  Dword          dusY[4];           /* number of v dataunits in scan */
  Byte           mcuSizeX;          /* Size of mcu in h pixels in scan */
  Byte           mcuSizeY;          /* Size of mcu in v pixels in scan */
  Byte           dcTable;           /* dc huffman table for component */
  Byte           acTable;           /* ac huffman table for component */
  Dword          mcusX;             /* number of horizontal mcus */
  Dword          mcusY;             /* number of vertical mcus */
  Int32*          lastDC;            /* pointer to component lastDC */
  Int32           lastDCY;           /* last DC for Y component */
  Int32           lastDCCb;          /* last DC for Cb component */
  Int32           lastDCCr;          /* last DC for Cr component */
  Byte           numScanComps;      /* number of components in scan */
  Byte           frameType;         /* jpeg frame type */
  Int32           eobRun;            /* remaining blocks in eob run */
  Int32           zeroRun;           /* remaining coefficients in zero run */
  Dword          restartInt;        /* last restart interval */
  Dword          mcuCount;          /* Count of mcus processed */
  Byte           restartMarker;     /* last read restart marker */
  int            skipMcus;          /* Number of mcus to skip */
  int            rstX;              /* last restart marker x position */
  int            rstY;              /* last restart marker y position */
  size_t         rstPos;            /* last restart file position */
  int            compIDs[16];       /* Component IDs */
  JFIF_SOF_COMP  components[16];    /* component decoding info */
  JFIF_SCAN_COMP scanComps[16];     /* component info in scan */
  JFIF_SPECTRAL  spectral;          /* spectral selection for scan */
  } JPEG;


/* ********************************************** */
/* **** Matrix for dct, left shifted 11 bits **** */
/* ********************************************** */
Int32 dct[8][8] = { {  724,  724,  724,  724, 724, 724, 724,  724 },
                   { 1004,  851,  569,  200,-199,-568,-850,-1003 },
                   {  946,  392, -391, -945,-945,-391, 392,  946 },
                   {  851, -199,-1003, -568, 569,1004, 200, -850 },
                   {  724, -723, -723,  724, 724,-723,-723,  724 },
                   {  569,-1003,  200,  851,-850,-199,1004, -568 },
                   {  392, -945,  946, -391,-391, 946,-945,  392 },
                   {  200, -568,  851,-1003,1004,-850, 569, -199 } };

Int32 tdct[8][8] = {{ 724, 1004, 946,  851, 724,  569, 392,  200 },
                   { 724,  851, 392, -199,-723,-1003,-945, -568 },
                   { 724,  569,-391,-1003,-723,  200, 946,  851 },
                   { 724,  200,-945, -568, 724,  851,-391,-1003 },
                   { 724, -199,-945,  569, 724, -850,-391, 1004 },
                   { 724, -568,-391, 1004,-723, -199, 946, -850 },
                   { 724, -850, 392,  200,-723, 1004,-945,  569 },
                   { 724,-1003, 946, -850, 724, -568, 392, -199 } };

Int32 yqt[8][8] = { { 16, 11, 10, 16, 24, 40, 51, 61 },
                   { 12, 12, 14, 19, 26, 58, 60, 55 },
                   { 14, 13, 16, 24, 40, 57, 69, 56 },
                   { 14, 17, 22, 29, 51, 87, 80, 62 },
                   { 18, 22, 37, 56, 68,109,103, 77 },
                   { 24, 35, 55, 64, 81,104,113, 92 },
                   { 49, 64, 78, 87,103,121,120,101 },
                   { 72, 92, 95, 98,112,100,103, 99 } };

Int32 cqt[8][8] = { { 17, 18, 24, 47, 99, 99, 99, 99 },
                   { 18, 21, 26, 66, 99, 99, 99, 99 },
                   { 24, 26, 56, 99, 99, 99, 99, 99 },
                   { 47, 66, 99, 99, 99, 99, 99, 99 },
                   { 99, 99, 99, 99, 99, 99, 99, 99 },
                   { 99, 99, 99, 99, 99, 99, 99, 99 },
                   { 99, 99, 99, 99, 99, 99, 99, 99 },
                   { 99, 99, 99, 99, 99, 99, 99, 99 } };

/* ************************* */
/* **** Zigzag ordering **** */
/* ************************* */
Byte jpeg_zigzag[] = {  0, 1, 8,16, 9, 2, 3,10,17,24,32,25,18,11, 4, 5,
                       12,19,26,33,40,48,41,34,27,20,13, 6, 7,14,21,28,
                       35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
                       58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63 };


Dword _countsToLengths(Dword* counts,Dword maxLen,Dword* lengths) {
  Dword index;
  Dword ii,jj;
  index = 0;
  for (ii=0; ii<maxLen;ii++) {
    for (jj=0; jj<counts[ii]; jj++) {
      lengths[index++] = ii+1;
      }
    }
  return index;
  }

void _generateHuffmanCodes(Dword numCodes,Dword* lengths,Dword* codes) {
  Dword huffmanCodeCounter;
  Dword codeLengthCounter;
  Dword index;
  huffmanCodeCounter = 0;
  codeLengthCounter = 1;
  index = 0;
  while (index <numCodes) {
    if (lengths[index] == codeLengthCounter) {
      codes[index++] = huffmanCodeCounter;
      huffmanCodeCounter++;
      } else {
      huffmanCodeCounter <<= 1;
      codeLengthCounter++;
      }
    }
  }

Huffman* _countsValuesToHuffman(Dword* counts,Dword* values) {
  Dword i;
  Dword lengths[256];
  Dword codes[256];
  Dword index;
  Huffman* ret;
  ret = new Huffman(0);
  index = _countsToLengths(counts,16,lengths);
  _generateHuffmanCodes(index,lengths,codes);
  for (i=0; i<index; i++) {
    ret->Add(codes[i], values[i], lengths[i]);
//    rcsgl_addHuffmanCode(&ret,lengths[i],codes[i],values[i]);
    }
  return ret;
  }

void  rcsgl_yCbCrToRgb(Byte y,Byte cb,Byte cr,
                       Byte* r,Byte* g,Byte* b) {
  Int32 dRed,dGreen,dBlue;
  Int32 dY,dCb,dCr;
  dY = 0 | y;
  dCb = 0 | cb;
  dCr = 0 | cr;
  dCb -= 128;
  dCr -= 128;
  dRed = dY + (dCr*140200/100000);
  dGreen = dY - (dCb*34414/100000) - (dCr*71414/100000);
  dBlue = dY + (dCb*172200/100000);
  if (dRed < 0) dRed = 0;
  if (dGreen < 0) dGreen = 0;
  if (dBlue < 0) dBlue = 0;
  if (dRed > 255) dRed = 255;
  if (dGreen > 255) dGreen = 255;
  if (dBlue > 255) dBlue = 255;
  *r = (dRed & 0xff);
  *g = (dGreen & 0xff);
  *b = (dBlue & 0xff);
  }

/* ********************************************** */
/* **** Function to read size of each marker **** */
/* ********************************************** */
Word _readSize(Stream* fildes) {
  Byte temp;
  Word size;
  temp = fildes->ReadByte();
  size = temp<<8;
  temp = fildes->ReadByte();
  size |= temp;
  size -= 2;
  return size;
  }

/* ********************************* */
/* **** Multiply 2 8x8 matrices **** */
/* ********************************* */
void _mulmatrix(Int32 a[8][8],Int32 b[8][8],Int32 c[8][8]) {
  int x,y;
  int i;
  double temp;
  for (y=0; y<8; y++) {
    for (x=0; x<8; x++) {
      temp = 0;
      for (i=0; i<8; i++)
        temp += ((a[y][i] * b[i][x])>>11);
      c[y][x] = temp;
      }
    }
  }

/* ***************************************************** */
/* **** Function to decode a progressive data block **** */
/* ***************************************************** */
void _decodeProgDataBlock(JPEG* jpeg,BitStream* stream,int comp,int dx,int dy) {
  Int32 offset;
  BLOCK* dataunit;
  Dword  code;
  Dword  bits;
  Dword  bits2;
  Int32   dc;
  Int32   ac;
  int    j;
  Byte   approx;

  /* ********************************** */
  /* **** Select correct component **** */
  /* ********************************** */
  switch (comp) {
    case 1:dataunit = jpeg->cBCoef; break;
    case 2:dataunit = jpeg->cRCoef; break;
    default: dataunit = jpeg->yCoef;
    }
  approx = jpeg->spectral.approx &0xf;

  /* **************************************************** */
  /* **** Determine which datablock within component **** */
  /* **************************************************** */
  offset = jpeg->dusX[comp];
  offset *= (dy / jpeg->pixelsY[comp]);
  offset += (dx / jpeg->pixelsX[comp]);
  /* ***************************** */
  /* **** Check for a DC scan **** */
  /* ***************************** */
  if (jpeg->spectral.selectionStart == 0 &&
      jpeg->spectral.selectionEnd == 0) {

    /* *********************************** */
    /* **** Check for initial DC scan **** */
    /* *********************************** */
    if ((jpeg->spectral.approx & 0xf0) == 0) {
      code = jpeg->dcht[jpeg->dcTable]->Next(stream);
      if (code != 0) {
        bits = stream->Read(code);
        dc = (bits < (Dword)(1<<(code-1))) ? bits + (-1 << code) + 1 : bits;
        }
        else dc = 0;
      dc += *(jpeg->lastDC);
      *(jpeg->lastDC) = dc;
      dc <<= approx;
      dataunit[offset][0][0] = dc;

      /* ************************************ */
      /* **** otherwise refining DC scan **** */
      /* ************************************ */
      } else {
      bits = stream->Read(1);
      dataunit[offset][0][0] |= (bits << approx);
      }
    }

  /* ***************************** */
  /* **** Check for a AC scan **** */
  /* ***************************** */
  if (jpeg->spectral.selectionStart != 0 &&
      jpeg->spectral.selectionEnd != 0) {

    /* *********************************** */
    /* **** Check for initial AC scan **** */
    /* *********************************** */
    if ((jpeg->spectral.approx & 0xf0) == 0) {

      /* ************************************** */
      /* **** Check if still in an eob run **** */
      /* ************************************** */
      if (jpeg->eobRun != 0) {
        jpeg->eobRun--;
        return;
        }
      j = jpeg->spectral.selectionStart;
      while (j <= jpeg->spectral.selectionEnd) {
        code = jpeg->acht[jpeg->acTable]->Next(stream);
        if (code == 0xf0) j += 16;
        else if ((code & 0x0f) == 0) {
          if (code == 0) j = 64;
            else {
            bits = stream->Read(code >> 4);
            jpeg->eobRun = (1 << (code >> 4)) + bits - 1;
            j = 64;
            }
          }
        else {
          j += (code >> 4);
          code &= 0xf;
          bits = stream->Read(code);
          ac = (bits < (Dword)(1<<(code-1))) ? bits + (-1 << code) + 1 : bits;
          ac <<= approx;
          dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] = ac;
          j++;
          }
        }

      /* ************************************ */
      /* **** otherwise refining AC scan **** */
      /* ************************************ */
      } else {
      j = jpeg->spectral.selectionStart;
      while (j<= jpeg->spectral.selectionEnd) {
        if (jpeg->eobRun > 0) {
          while (j<=jpeg->spectral.selectionEnd) {
            if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] != 0) {
              bits = stream->Read(1);
              if (bits != 0) {
                if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] > 0)
                  dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                    (1 << approx);
                else
                  dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                    (-1 << approx);
                }
              }
            j++;
            }
          jpeg->eobRun--;
          return;
          }
        code = jpeg->acht[jpeg->acTable]->Next(stream);
        if (code & 1) {
          code >>= 4;
          bits2 = stream->Read(1);
          while (code > 0 ||
                 dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] != 0) {
            if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] != 0) {
              bits = stream->Read(1);
              if (bits != 0) {
                if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] > 0)
                  dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                    (1 << approx);
                else
                  dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                    (-1 << approx);
                }
              } else code--;
            j++;
            }
          dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] =
            (bits2 != 0) ? 1 << approx :
                          -1 << approx;
          j++;
          } else {
          code >>= 4;
          if (code == 0xf) {
            code++;
            while (code > 0) {
              if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] != 0) {
                bits = stream->Read(1);
                if (bits != 0) {
                  if (dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] > 0)
                    dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                      (1 << approx);
                  else
                    dataunit[offset][jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] +=
                      (-1 << approx);
                  }
                } else code--;
              j++;
              }
            } else if (code == 0) jpeg->eobRun = 1;
          else {
            bits = stream->Read(code);
            jpeg->eobRun = (1 << code) | bits;
            }
          }
        } /* while (j<= jpeg->spectral.selectionEnd) */
      }
    }
  }


/* **************************************************** */
/* **** Function to decode a sequential data block **** */
/* **************************************************** */
void _decodeDataBlock(JPEG* jpeg,BitStream* stream,int comp,int dx,int dy) {
  int    j;
  int    x,y;
  int    xs,ys;
  int    tx;
  int    x2,y2;
  Dword  code;
  Dword  bits;
  Int32   dc;
  Int32   ac;
  Byte*  dest;
  BLOCK  dataUnit;
  BLOCK  samples;
  /* ***************************************** */
  /* **** Set the data block to all zeros **** */
  /* ***************************************** */
  for (y=0; y<8; y++) 
    for (x=0; x<8; x++) dataUnit[y][x] = 0;

  /* *************************************************** */
  /* **** Set destination to correct component data **** */
  /* *************************************************** */
  switch (comp) {
    case  1:dest = jpeg->cb; break;
    case  2:dest = jpeg->cr; break;
    default:dest = jpeg->y; break;
    }

  /* ********************************************* */
  /* **** Get huffman code for DC coefficient **** */
  /* ********************************************* */
  code = jpeg->dcht[jpeg->dcTable]->Next(stream);
  if (code != 0) {
    bits = stream->Read(code);
    dc = (bits < (Dword)(1<<(code-1))) ? bits + (-1 << code) + 1 : bits;
    }
    else dc = 0;

  /* *************************************************** */
  /* **** De-quantize dc coefficient and add lastDC **** */
  /* *************************************************** */
  dc = dc * jpeg->qt[jpeg->components[comp].qt][0] + *(jpeg->lastDC);

  /* ************************************ */
  /* **** Set lastDC to new dc value **** */
  /* ************************************ */
  *(jpeg->lastDC) = dc;
    
  /* ************************************************ */
  /* **** Store into data unit as scaled integer **** */
  /* ************************************************ */
  dataUnit[0][0] = dc << 11;

  /* ************************************************** */
  /* **** AC coefficients then start at position 1 **** */
  /* ************************************************** */
  j = 1;

  /* ********************************************************** */
  /* **** Loop until all 64 AC coefficients have been read **** */
  /* ********************************************************** */
  while (j < 64) {

    /* ************************************************** */
    /* **** Get huffman code for next AC coefficient **** */
    /* ************************************************** */
    code = jpeg->acht[jpeg->acTable]->Next(stream);

    /* ************************************************************ */
    /* **** if code == 0xf0 then next 16 coefficients are zero **** */
    /* ************************************************************ */
    if (code == 0xf0) j += 16;

    /* ******************************************************************** */
    /* **** else if code == 0 then all remaining coefficients are zero **** */
    /* ******************************************************************** */
    else if (code == 0) j = 64;

    /* *************************************** */
    /* **** else process next coefficient **** */
    /* *************************************** */
    else {
  
      /* ********************************************************* */
      /* **** High nybble specifies runs of zero coefficients **** */
      /* ********************************************************* */
      j += (code >> 4);

      /* ********************************** */
      /* **** Clear run of zeros count **** */
      /* ********************************** */
      code &= 0xf;

      /* *********************************************** */
      /* **** Read remaining bits of AC coefficient **** */
      /* *********************************************** */
      bits = (code != 0) ? stream->Read(code) : 0;
      ac = (bits < (Dword)(1<<(code-1))) ? bits + (-1 << code) + 1 : bits;

      /* **************************************** */
      /* **** De-quantize the AC coefficient **** */
      /* **************************************** */
      ac *= jpeg->qt[jpeg->components[comp].qt][jpeg_zigzag[j]];

      /* **************************************************************** */
      /* **** Store coefficent in proper zig-zag order as scaled int **** */
      /* **************************************************************** */
      dataUnit[jpeg_zigzag[j]>>3][jpeg_zigzag[j]&7] = (ac << 11);

      /* **** Incrment to next coefficient position **** */
      j++;
      }
    }

  /* ******************************************************* */
  /* **** Perform inverse discreet cosin transformation **** */
  /* ******************************************************* */
  _mulmatrix(tdct,dataUnit,samples);
  _mulmatrix(samples,dct,dataUnit);

  /* ****************************************************************** */
  /* **** Need to descale, re-range, and bound check sample values **** */
  /* ****************************************************************** */
  for (y=0; y<8; y++) 
    for (x=0; x<8; x++) {
      /* ************************************************ */
      /* **** Remove scaling, and set range to 0-255 **** */
      /* ************************************************ */
      dataUnit[y][x] = (dataUnit[y][x] >> 11) + 128;

      /* **************************************************** */
      /* **** Check bounds, value must be >= 0 and <=255 **** */
      /* **************************************************** */
      if (dataUnit[y][x] < 0) dataUnit[y][x] = 0;
      if (dataUnit[y][x] > 255) dataUnit[y][x] = 255;
      }

  /* *************************************** */
  /* **** Get pixel span in x direction **** */
  /* *************************************** */
  xs = (jpeg->pixelsX[comp] == 16) ? 2 : 1;

  /* *************************************** */
  /* **** Get pixel span in y direction **** */
  /* *************************************** */
  ys = (jpeg->pixelsY[comp] == 16) ? 2 : 1;

  /* *************************************************** */
  /* **** Remember starting x position for each row **** */
  /* *************************************************** */
  tx = dx;

  /* ***************************************** */
  /* **** 8 vertical samples in data unit **** */
  /* ***************************************** */
  for (y=0; y<8; y++)
  
    /* ****************************************** */
    /* **** Loop for vertical pixel spanning **** */
    /* ****************************************** */
    for (y2=0; y2<ys; y2++) {

      /* *********************************************** */
      /* **** Only process if pixels still in image **** */
      /* *********************************************** */
      if (dy < (int)jpeg->height) {

        /* ******************************************* */
        /* **** Set x to starting position of row **** */
        /* ******************************************* */
        dx = tx;

        /* ******************************************* */
        /* **** 8 horizontal samples in data unit **** */
        /* ******************************************* */
        for (x=0; x<8; x++) {

          /* ******************************************** */
          /* **** Loop for horizontal pixel spanning **** */
          /* ******************************************** */
          for (x2=0; x2<xs; x2++) {

            /* ************************************************** */
            /* **** Add pixel to image data of within bounds **** */
            /* ************************************************** */
            if (dx < (int)jpeg->width) dest[dy*jpeg->width + dx] = dataUnit[y][x];

            /* ***************************** */
            /* **** Next pixel position **** */
            /* ***************************** */
            dx++;
            }
          }
        }

      /* *************************** */
      /* **** next row in image **** */
      /* *************************** */
      dy++;
      }
  }

/* *************************************************** */
/* **** Function to decode one mcu of a jpeg scan **** */
/* *************************************************** */
void _decodeMCU(JPEG* jpeg,BitStream* stream,int x,int y) {
  int i,j;
  int comp;
  int compIndex;
  int xl,yl;
  int xp,yp;
  int endx,endy;
  /* ************************************************ */
  /* **** Loop over number of components in scan **** */
  /* ************************************************ */
  for (i=0; i<jpeg->numScanComps; i++) {

    /* ************************************************** */
    /* **** Set huffman tables for current component **** */
    /* ************************************************** */
    jpeg->dcTable = jpeg->scanComps[i].dcHuffTable;
    jpeg->acTable = jpeg->scanComps[i].acHuffTable;

    /* ************************************************* */
    /* **** Set component number for this component **** */
    /* ************************************************* */
    comp=jpeg->scanComps[i].compNumber;
    compIndex = -1;
    for (j=0; j<16; j++)
      if (jpeg->compIDs[j] == comp) {
        compIndex = j;
        j = 16;
        }
    if (compIndex < 0)
      throw InvalidOpException(NULL, "Invalid component ID encountered");

    /* *************************************** */
    /* **** Set lastDC for this component **** */
    /* *************************************** */
    switch (compIndex) {
      case 0:jpeg->lastDC = &(jpeg->lastDCY); break;
      case 1:jpeg->lastDC = &(jpeg->lastDCCb); break;
      case 2:jpeg->lastDC = &(jpeg->lastDCCr); break;
      }
    /* *********************************************** */
    /* **** Determine y position in image for mcu **** */
    /* *********************************************** */
    yp = y * jpeg->mcuSizeY;

    endy = (jpeg->numScanComps == 1) ? 1 : jpeg->components[compIndex].vSampling;
    endx = (jpeg->numScanComps == 1) ? 1 : jpeg->components[compIndex].hSampling;
    /* **************************************************** */
    /* **** Loop over v sampling for current component **** */
    /* **************************************************** */
    for (yl=0; yl<endy; yl++) {

      /* ********************************************* */
      /* **** Determin x position in image of mcu **** */
      /* ********************************************* */
      xp = x * jpeg->mcuSizeX;

      /* **************************************************** */
      /* **** Loop over h sampling for current component **** */
      /* **************************************************** */
      for (xl=0; xl<endx; xl++) {

        /* ************************************************* */
        /* **** Decode data block for current component **** */
        /* ************************************************* */
        if (jpeg->frameType == JPEG_SOF0 ||
            jpeg->frameType == JPEG_SOF1)
          _decodeDataBlock(jpeg,stream,compIndex,xp,yp);
        if (jpeg->frameType == JPEG_SOF2)
          _decodeProgDataBlock(jpeg,stream,compIndex,xp,yp);


        /* ***************************************************** */
        /* **** Move to next horizontal data block position **** */
        /* ***************************************************** */
        xp +=8;
        }

      /* *************************************************** */
      /* **** Move to next vertical data block position **** */
      /* *************************************************** */
      yp += 8;
      }
    }
  }

/* ************************************************** */
/* **** Read a jpeg file into an IMAGE structure **** */
/* ************************************************** */
void Image::readJPG() {
  Byte  marker;
  Byte  temp;
  Byte  scanComps;
  Word  wTemp;
  Word  size;
  Word  index;
  int   compIndex;
  Dword counts[16];
  char  flag;
  Byte  qtFlags;
  int   i,j;
  int   x,y;
  int   x2,y2;
  int   offX,offY;
  int   sizeX,sizeY;
  int   mx,my;
  Byte  comp;
  Byte  pixels;
  Byte  pixelsX;
  Byte  pixelsY;
  Byte* pos;
  Byte* dest;
  Byte  headerFound;
  Dword pos2;
  int   bcount;
  Dword maxWidth;
  Dword maxHeight;
  Stream* fildes;
  Dword mcusX, mcusY;
  Dword*      values;
  BLOCK       samples;
  JFIF_HEADER header;
  JFIF_SOF    sof;
  JPEG        jpeg;
  JFIF_DHT_HEADER dht;
  BLOCK*      dataunit;
  BitStream*  stream;

  fildes = new MemoryStream(pathname);
  /* ****************************** */
  /* **** Clear jpeg structure **** */
  /* ****************************** */
  jpeg.fxMax = 0;
  jpeg.fyMax = 0;
  jpeg.yCoef = NULL;
  jpeg.cBCoef = NULL;
  jpeg.cRCoef = NULL;
  jpeg.y = NULL;
  jpeg.cb = NULL;
  jpeg.cr = NULL;
  jpeg.restartInt = 0;
  for (i=0; i<4; i++) {
    jpeg.dcht[i] = NULL;
    jpeg.acht[i] = NULL;
    }

  /* ******************************************* */
  /* **** check first bytes for FF D8 FF E0 **** */
  /* ******************************************* */
  data = NULL;
  headerFound = 0;
  fildes->Position(0);
  marker = fildes->ReadByte();
  if (marker != 255) throw InvalidOpException(this, "Not a valid jpg file");
  marker = fildes->ReadByte();
  if (marker != JPEG_SOI) throw InvalidOpException(this, "Not a valid jpg file");

  /* ************************** */
  /* **** Read jpeg header **** */
  /* ************************** */
  flag = ' ';

  /* ****************************************** */
  /* **** Loop through file until all read **** */
  /* ****************************************** */
  while (flag == ' ') {
    temp = 255;

    /* ******************************** */
    /* **** Skip over and FF bytes **** */
    /* ******************************** */
    bcount = 1;
    while (temp == 255 && bcount != 0) bcount=fildes->ReadByteArray(&temp,1);
    /* **************************************** */
    /* **** Data for APP0 - Start of Image **** */
    /* **************************************** */
    if (temp == JPEG_APP0) {
      size = _readSize(fildes);
      if (headerFound == 0) {
        fildes->ReadByteArray((Byte*)&header, sizeof(header));
        size -= sizeof(header);
        fildes->Position(fildes->Position() + size);
        headerFound = 1;
        } else {
        fildes->Position(fildes->Position() + size);
        }
      }

    /* ************************************** */
    /* **** Data for SOS - Start of Scan **** */
    /* ************************************** */
    else if (temp == JPEG_SOS) {
      size = _readSize(fildes);
      /* ****************************************** */
      /* **** Get number of components in scan **** */
      /* ****************************************** */
      scanComps = fildes->ReadByte();
      jpeg.numScanComps = scanComps;

      /* ************************* */
      /* **** Reset mcu sizes **** */
      /* ************************* */
      jpeg.mcuSizeX = 0;
      jpeg.mcuSizeY = 0;
      jpeg.eobRun = 0;
      jpeg.zeroRun = 0;

      /* *************************************** */
      /* **** loop over scan component info **** */
      /* *************************************** */
      for (i=0; i<scanComps; i++) {

        /* *************************** */
        /* **** Read component ID **** */
        /* *************************** */
        comp = fildes->ReadByte();
        jpeg.scanComps[i].compNumber = comp;
        compIndex = -1;
        for (j=0; j<16; j++)
          if (jpeg.compIDs[j] == comp) {
            compIndex = j;
            j = 16;
            }
        if (compIndex < 0)
          throw InvalidOpException(this, "Invalid component ID encountered");

        /* ************************************************ */
        /* **** Make sure storage for component exists **** */
        /* ************************************************ */
        switch (compIndex) {
          case 0:if (jpeg.y == NULL)
                   jpeg.y  = (Byte*)malloc(jpeg.width*jpeg.height);
                 break;
          case 1:if (jpeg.cb == NULL)
                   jpeg.cb = (Byte*)malloc(jpeg.width*jpeg.height);
                 break;
          case 2:if (jpeg.cr == NULL)
                   jpeg.cr = (Byte*)malloc(jpeg.width*jpeg.height);
                 break;
          }

        /* ******************************** */
        /* **** read huffman table ids **** */
        /* ******************************** */
        temp = fildes->ReadByte();
        jpeg.scanComps[i].dcHuffTable = temp >> 4;
        jpeg.scanComps[i].acHuffTable = temp & 0xf;

        /* ******************************************** */
        /* **** Determine pixel spans for dataunit **** */
        /* ******************************************** */
//        jpeg.pixelsX[i] = 8 * (jpeg.fxMax / jpeg.components[compIndex].hSampling);
//        jpeg.pixelsY[i] = 8 * (jpeg.fyMax / jpeg.components[compIndex].vSampling);
        jpeg.pixelsX[i] = 8 * (jpeg.fxMax / jpeg.components[compIndex].hSampling);
        jpeg.pixelsY[i] = 8 * (jpeg.fyMax / jpeg.components[compIndex].vSampling);

        /* ****************************************** */
        /* **** Keep track of largest pixel span **** */
        /* ****************************************** */
//        if (jpeg.pixelsX[i] > jpeg.mcuSizeX) jpeg.mcuSizeX = jpeg.pixelsX[i];
//        if (jpeg.pixelsY[i] > jpeg.mcuSizeY) jpeg.mcuSizeY = jpeg.pixelsY[i];
        if (jpeg.pixelsX[i] > jpeg.mcuSizeX) jpeg.mcuSizeX = jpeg.pixelsX[i];
        if (jpeg.pixelsY[i] > jpeg.mcuSizeY) jpeg.mcuSizeY = jpeg.pixelsY[i];
        }

      /* ********************************* */
      /* **** Read spectral selection **** */
      /* ********************************* */
      fildes->ReadByteArray((Byte*)&jpeg.spectral,3);

      /* ******************************** */
      /* **** Reset all starting DCs **** */
      /* ******************************** */
      jpeg.lastDCY = 0;
      jpeg.lastDCCr = 0;
      jpeg.lastDCCb = 0;

      /* *************************************************** */
      /* **** Create bit stream to read compressed data **** */
      /* *************************************************** */
      stream = new JpegBitStream(fildes);
      stream->LittleEndian(false);
      stream->DeleteStreamOnDispose(false);
      stream->FillLowToHigh(false);

      /* ******************************************* */
      /* **** Compute mcu counts to decode scan **** */
      /* ******************************************* */
      if (scanComps == 1) {
        jpeg.mcusX = (jpeg.width + jpeg.pixelsX[0]-1)/jpeg.pixelsX[0];
        jpeg.mcusY = (jpeg.height + jpeg.pixelsY[0]-1)/jpeg.pixelsY[0];
        } else {
        jpeg.mcusX = (jpeg.width + 8*jpeg.fxMax-1)/(8*jpeg.fxMax);
        jpeg.mcusY = (jpeg.height + 8*jpeg.fyMax-1)/(8*jpeg.fyMax);
        }
      jpeg.mcuCount = 0;
      /* **************************************** */
      /* **** Loop over mcus to decode image **** */
      /* **************************************** */
      for (y=0; y<(int)jpeg.mcusY; y++) {
        for (x=0; x<(int)jpeg.mcusX; x++) {
          /* ************************************** */
          /* **** Decoder for sequential files **** */
          /* ************************************** */
          _decodeMCU(&jpeg,stream,x,y);
          if (jpeg.restartInt != 0) {
            jpeg.mcuCount++;
            if (jpeg.mcuCount == jpeg.restartInt &&
                (y != (int)jpeg.mcusY-1 || x != (int)jpeg.mcusX-1)) {
              temp = fildes->ReadByte();
              if (temp != 255) {
                while (temp != 255) {
                  temp = fildes->ReadByte();
                  if (temp == 255) {
                    temp = fildes->ReadByte();
                    if (temp != 0) {
                      fildes->Position(fildes->Position() - 1);
                      temp = 255;
                      }
                    }
                  }
                temp = fildes->ReadByte();
                temp -= 0xd0;
                jpeg.restartMarker -= 0xd0;
                jpeg.skipMcus = jpeg.restartInt*((8+temp-jpeg.restartMarker)%8);
                x = jpeg.rstX;
                y = jpeg.rstY;
                while (jpeg.skipMcus > 0) {
                  x++;
                  if (x >= (int)jpeg.mcusX) {
                    x = 0;
                    y++;
                    }
                  jpeg.skipMcus--;
                  }
                fildes->Position(fildes->Position() - 1);
                }
              temp = fildes->ReadByte();
              jpeg.restartMarker = temp;
              jpeg.rstX = x;
              jpeg.rstY = y;
              stream->Flush();
              jpeg.mcuCount = 0;
              jpeg.lastDCY=0;
              jpeg.lastDCCb=0;
              jpeg.lastDCCr=0;
              }
            }
          }
        }
      }

    /* ************************************* */
    /* **** Data for EOI - End of Image **** */
    /* ************************************* */
    else if (temp == JPEG_EOI || bcount == 0) {
      if (jpeg.frameType == JPEG_SOF2) {
        for (i=0; i<jpeg.compCount; i++) {
          pixelsX = 8 * (jpeg.fxMax / jpeg.components[i].hSampling);
          pixelsY = 8 * (jpeg.fyMax / jpeg.components[i].vSampling);
          mcusX = (jpeg.width + pixelsX - 1) / pixelsX;
          mcusY = (jpeg.height + pixelsY - 1) / pixelsY;
          switch (i) {
            case 0:dataunit = jpeg.yCoef;  dest=jpeg.y; break;
            case 1:dataunit = jpeg.cBCoef; dest=jpeg.cb; break;
            case 2:dataunit = jpeg.cRCoef; dest=jpeg.cr; break;
            }
          for (my=0; my<(int)mcusY; my++) {
            offY = pixelsY * my;
            sizeY = pixelsY >> 3;
            for (mx=0; mx<(int)mcusX; mx++) {
              offX = pixelsX * mx;
              sizeX = pixelsX >> 3;
              j = my*jpeg.dusX[i] + mx;
              for (y=0; y<8; y++)
                for (x=0; x<8; x++)
                  dataunit[j][y][x] = (dataunit[j][y][x] * jpeg.qt[jpeg.components[i].qt][y*8+x]) << 11;
              _mulmatrix(tdct,dataunit[j],samples);
              _mulmatrix(samples,dct,dataunit[j]);
              for (y=0; y<8; y++)
                for (x=0; x<8; x++) {
                  dataunit[j][y][x] = (dataunit[j][y][x] >> 11) + 128;
                  if (dataunit[j][y][x] < 0) dataunit[j][y][x] = 0;
                  if (dataunit[j][y][x] > 255) dataunit[j][y][x] = 255;
                  for (y2=0;y2<sizeY;y2++)
                    for (x2=0;x2<sizeX;x2++)
                    if (offY+y*sizeY+y2 < (int)jpeg.height && offX+x*sizeX+x2<(int)jpeg.width)
                      dest[(offY+y*sizeY+y2)*(int)jpeg.width+offX+x*sizeX+x2] = dataunit[j][y][x];
                  }
              }
            }
          }
        }
      width = jpeg.width;
      height = jpeg.height;
      data = (Byte*)malloc(width * height * 3);
      if (jpeg.compCount == 1) {  
        }
      if (jpeg.compCount == 3) {  
        for (y=0; y<(int)height; y++) {
          pos = data + (y*width*3);
          pos2 = y*width;
          for (x=0; x<(int)width; x++) {
            rcsgl_yCbCrToRgb(jpeg.y[pos2],jpeg.cb[pos2],jpeg.cr[pos2],
                             pos,pos+1,pos+2);
            pos2++;
            pos += 3;
            }
          }
        free(jpeg.y);
        free(jpeg.cb);
        free(jpeg.cr);
        }
      if (jpeg.yCoef != NULL) free(jpeg.yCoef);
      if (jpeg.cBCoef != NULL) free(jpeg.cBCoef);
      if (jpeg.cRCoef != NULL) free(jpeg.cRCoef);
      for (i=0; i<4; i++) {
        if (jpeg.dcht[i] != NULL) delete(jpeg.dcht[i]);
        if (jpeg.acht[i] != NULL) delete(jpeg.acht[i]);
        }
      flag = '*';
      }

    /* **************************************** */
    /* **** Data for SOF0 - Start of Frame **** */
    /* **************************************** */
    else if (temp == JPEG_SOF0 ||
             ((temp & 0xf0) == 0xc0 && (temp & 3) != 0)) {
      /* ****************************************** */
      /* **** Set frame type in jpeg structure **** */
      /* ****************************************** */
      jpeg.frameType = temp;

      /* ********************************* */
      /* **** Read SOF data structure **** */
      /* ********************************* */
      size = _readSize(fildes);
      fildes->ReadByteArray((Byte*)&sof,sizeof(sof));
      size -= sizeof(sof);

      /* ********************************** */
      /* **** Set image specifications **** */
      /* ********************************** */
      jpeg.width = (sof.width[0]<<8) + (sof.width[1]);
      jpeg.height = (sof.height[0]<<8) + (sof.height[1]);
      jpeg.compCount = sof.components;
      jpeg.bitsPerSample = sof.bitsPerSample;

      /* ******************************************** */
      /* **** get sample info for each component **** */
      /* ******************************************** */
      for (i=0; i<jpeg.compCount; i++) {

        /* ************************** */
        /* **** Get component ID **** */
        /* ************************** */
        temp = fildes->ReadByte();
        jpeg.compIDs[i] = temp;

        /* ***************************** */
        /* **** Read sampling sizes **** */
        /* ***************************** */
        jpeg.components[i].hSampling = fildes->ReadByte();

        /* ********************************************** */
        /* **** Get quantization table for component **** */
        /* ********************************************** */
        jpeg.components[i].qt = fildes->ReadByte();

        /* *************************************************** */
        /* **** v sampling is low nybble of sampling byte **** */
        /* *************************************************** */
        jpeg.components[i].vSampling = 
          jpeg.components[i].hSampling & 0xf;

        /* ********************************* */
        /* **** h sampling is hi nybble **** */
        /* ********************************* */
        jpeg.components[i].hSampling >>= 4;

        /* ******************************************* */
        /* **** Check for maximum h sampling size **** */
        /* ******************************************* */
        if (jpeg.components[i].hSampling > jpeg.fxMax)
          jpeg.fxMax = jpeg.components[i].hSampling;

        /* ******************************************* */
        /* **** Check for maximum v sampling size **** */
        /* ******************************************* */
        if (jpeg.components[i].vSampling > jpeg.fyMax)
          jpeg.fyMax = jpeg.components[i].vSampling;

        /* **************************** */
        /* **** remove 3 from size **** */
        /* **************************** */
        size -= 3;
        }

      /* *************************************** */
      /* **** see if progressive jpeg frame **** */
      /* *************************************** */
      if (jpeg.frameType == JPEG_SOF2) {
        maxWidth = 0;
        maxHeight = 0;

        /* ******************************************* */
        /* **** Loop over each component in frame **** */
        /* ******************************************* */
        for (i=0; i<jpeg.compCount; i++) {

          /* *********************************** */
          /* **** Determine h dataunit size **** */
          /* *********************************** */
          pixels = 8 * (jpeg.fxMax / jpeg.components[i].hSampling);

          /* ***************************************** */
          /* **** Determine number of h dataunits **** */
          /* ***************************************** */
          mcusX = (jpeg.width + pixels - 1) / pixels;
          if (pixels * mcusX > maxWidth) maxWidth = pixels*mcusX;
          /* *********************************** */
          /* **** Determine v dataunit size **** */
          /* *********************************** */
          pixels = 8 * (jpeg.fyMax / jpeg.components[i].vSampling);

          /* ***************************************** */
          /* **** Determine number of v dataunits **** */
          /* ***************************************** */
          mcusY = (jpeg.height + pixels - 1) / pixels;
          if (pixels * mcusY > maxHeight) maxHeight = pixels*mcusY;
          /* ******************************************************** */
          /* **** Create coefficient storage block for component **** */
          /* ******************************************************** */
          }
        for (i=0; i<jpeg.compCount; i++) {
          mcusX = maxWidth / (8*(jpeg.fxMax/jpeg.components[i].hSampling));
          mcusY = maxHeight / (8*(jpeg.fyMax/jpeg.components[i].vSampling));
          jpeg.dusX[i] = mcusX;
          switch (i) {
            case 0:jpeg.yCoef = (BLOCK*)malloc(sizeof(BLOCK)*mcusX*mcusY);
                   for (j=0; j<(int)mcusX*(int)mcusY; j++)
                     for (y=0; y<8; y++)
                       for (x=0; x<8; x++) jpeg.yCoef[j][y][x] = 0;
                   break;
            case 1:jpeg.cBCoef = (BLOCK*)malloc(sizeof(BLOCK)*mcusX*mcusY);
                   for (j=0; j<(int)mcusX*(int)mcusY; j++)
                     for (y=0; y<8; y++)
                       for (x=0; x<8; x++) jpeg.cBCoef[j][y][x] = 0;
                   break;
            case 2:jpeg.cRCoef = (BLOCK*)malloc(sizeof(BLOCK)*mcusX*mcusY);
                   for (j=0; j<(int)mcusX*(int)mcusY; j++)
                     for (y=0; y<8; y++)
                       for (x=0; x<8; x++) jpeg.cRCoef[j][y][x] = 0;
                   break;
            }
          }
        }
      }

    /* ********************************** */
    /* **** Read quantization tables **** */
    /* ********************************** */
    else if (temp == JPEG_DQT) {
      size = _readSize(fildes);
      while (size > 0) {
        qtFlags = fildes->ReadByte();
        for (i=0; i<64; i++) {
          if (qtFlags & 0xf0) {
            temp = fildes->ReadByte();
            wTemp = temp << 8;
            temp = fildes->ReadByte();
            wTemp |= temp;
            } else {
            temp = fildes->ReadByte();
            wTemp = temp;
            }
          jpeg.qt[qtFlags & 0xf][jpeg_zigzag[i]] = wTemp;
          }
        size -= (qtFlags & 0xf0) ? 129 : 65;
        }
      }

    /* ***************************** */
    /* **** Read huffman tables **** */
    /* ***************************** */
    else if (temp == JPEG_DHT) {
      size = _readSize(fildes);
      while (size > 0) {
        fildes->ReadByteArray((Byte*)&dht, sizeof(dht));
        size -= sizeof(dht);
        j = 0;
        for (i=0; i<16; i++) j += dht.counts[i];
        values = (Dword*)malloc(sizeof(Dword) * j);
        index = 0;
        for (i=0; i<16; i++) {
          counts[i] = dht.counts[i];
          for (j=0; j<dht.counts[i]; j++) {
            temp = fildes->ReadByte();
            values[index++] = temp;
            size--;
            }
          }
        if (dht.flags &0xf0)
          jpeg.acht[dht.flags&7] = _countsValuesToHuffman(counts,values);
        else
          jpeg.dcht[dht.flags&7] = _countsValuesToHuffman(counts,values);
        free(values);
        }
      }

    /* ********************************* */
    /* **** Define Restart Interval **** */
    /* ********************************* */
    else if (temp == JPEG_DRI) {
      size = _readSize(fildes);
      temp = fildes->ReadByte();
      wTemp = temp << 8;
      temp = fildes->ReadByte();
      wTemp |= temp;
      jpeg.restartInt = wTemp;
      }

    /* ********************** */
    /* **** Read comment **** */
    /* ********************** */
    else if (temp == JPEG_COM) {
      size = _readSize(fildes);
      for (i=0; i<size; i++) {
        temp = fildes->ReadByte();
        }
      }

    /* ***************************************************** */
    /* **** Read and discard any additional APP markers **** */
    /* ***************************************************** */
    else if (temp >= JPEG_APP0 && temp <= JPEG_APPF) {
      size = _readSize(fildes);
      fildes->Position(fildes->Position() + size);
      }
    else if (temp >= 0x02 && temp <= 0xbf) {
      size = _readSize(fildes);
      fildes->Position(fildes->Position() + size);
      }
    else {
      }
    }
  }


