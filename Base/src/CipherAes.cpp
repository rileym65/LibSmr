#include <string.h>

#include "SmrFramework.h"

using namespace SmrFramework;

  static constexpr Byte Aes_sbox[256] = {
      0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
      0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
      0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
      0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
      0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
      0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
      0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
      0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
      0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
      0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
      0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
      0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
      0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
      0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
      0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
      0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
  static constexpr Byte Aes_rsbox[256] = {
      0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
      0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
      0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
      0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
      0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
      0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
      0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
      0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
      0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
      0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
      0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
      0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
      0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
      0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
      0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
      0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
  static constexpr Byte Aes_rCon[255] = {
      0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
      0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
      0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
      0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
      0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
      0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
      0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
      0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
      0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
      0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
      0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
      0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
      0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
      0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
      0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
      0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

CipherAes::CipherAes() {
  int i, x, y;
  objectType = (char*)"CipherAes";
  mode = AES_MODE_ECB;
  for (y=0; y<4; y++)
    for (x=0; x<4; x++) {
      initVector[y][x] = 0;
      state[y][x] = 0;
      }
  for (i=0; i<240; i++) Aes_roundKey[i] = 0;
  for (i=0; i<32; i++) Aes_key[i] = 0;
  counter = 0;
  }

CipherAes::CipherAes(const char* cipherKey) : Cipher() {
  int i, x, y;
  objectType = (char*)"CipherAes";
  mode = AES_MODE_ECB;
  for (y=0; y<4; y++)
    for (x=0; x<4; x++) {
      initVector[y][x] = 0;
      state[y][x] = 0;
      }
  for (i=0; i<240; i++) Aes_roundKey[i] = 0;
  for (i=0; i<32; i++) Aes_key[i] = 0;
  Init(cipherKey);
  counter = 0;
  }

CipherAes::CipherAes(const char* cipherKey, UInt32 size) : Cipher() {
  int i, x, y;
  objectType = (char*)"CipherAes";
  mode = AES_MODE_ECB;
  for (y=0; y<4; y++)
    for (x=0; x<4; x++) {
      initVector[y][x] = 0;
      state[y][x] = 0;
      }
  for (i=0; i<240; i++) Aes_roundKey[i] = 0;
  for (i=0; i<32; i++) Aes_key[i] = 0;
  Init(cipherKey, size);
  counter = 0;
  }

CipherAes::~CipherAes() {
  }

Byte CipherAes::AesXtime(Byte x) {
  return (Byte)((x << 1) ^ (((x >> 7) & 1) * 0x1b));
  }

Byte CipherAes::AesMultiply(Byte x, Byte y) {
  return (Byte)(((y & 1) * x) ^ ((y >> 1 & 1) * AesXtime(x)) ^
               ((y >> 2 & 1) * AesXtime(AesXtime(x))) ^
               ((y >> 3 & 1) * AesXtime(AesXtime(AesXtime(x)))) ^
               ((y >> 4 & 1) * AesXtime(AesXtime(AesXtime(AesXtime(x))))));
  }

        void CipherAes::AesKeyExpansion()
        {
            Int32 i, j;
            Byte k;
            Byte temp[4];
            for (i=0; i<4; i++) temp[i] = 0;
            for (i = 0; i < Aes_Nk; i++)
            {
                Aes_roundKey[i * 4] = Aes_key[i * 4];
                Aes_roundKey[i * 4 + 1] = Aes_key[i * 4 + 1];
                Aes_roundKey[i * 4 + 2] = Aes_key[i * 4 + 2];
                Aes_roundKey[i * 4 + 3] = Aes_key[i * 4 + 3];
            }
            while (i < (Aes_Nb * (Aes_Nr + 1)))
            {
                for (j = 0; j < 4; j++)
                {
                    temp[j] = Aes_roundKey[(i - 1) * 4 + j];
                }
                if (i % Aes_Nk == 0)
                {
                    k = temp[0];
                    temp[0] = temp[1];
                    temp[1] = temp[2];
                    temp[2] = temp[3];
                    temp[3] = k;
                    temp[0] = Aes_sbox[temp[0]];
                    temp[1] = Aes_sbox[temp[1]];
                    temp[2] = Aes_sbox[temp[2]];
                    temp[3] = Aes_sbox[temp[3]];
                    temp[0] = (Byte)(temp[0] ^ Aes_rCon[i / Aes_Nk]);
                }
                else if (Aes_Nk > 6 && i % Aes_Nk == 4)
                {
                    temp[0] = Aes_sbox[temp[0]];
                    temp[1] = Aes_sbox[temp[1]];
                    temp[2] = Aes_sbox[temp[2]];
                    temp[3] = Aes_sbox[temp[3]];
                }
                Aes_roundKey[i * 4 + 0] = (Byte)(Aes_roundKey[(i - Aes_Nk) * 4 + 0] ^ temp[0]);
                Aes_roundKey[i * 4 + 1] = (Byte)(Aes_roundKey[(i - Aes_Nk) * 4 + 1] ^ temp[1]);
                Aes_roundKey[i * 4 + 2] = (Byte)(Aes_roundKey[(i - Aes_Nk) * 4 + 2] ^ temp[2]);
                Aes_roundKey[i * 4 + 3] = (Byte)(Aes_roundKey[(i - Aes_Nk) * 4 + 3] ^ temp[3]);
                i++;
            }
        }

void CipherAes::Init(const char* cipherKey) {
  Int32 i;
  Int32 x, y;
  Int32 klen;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      state[y][x] = 0;
  Aes_Nk = strlen(cipherKey) * 8 / 32;
  Aes_Nr = Aes_Nk + 6;
  for (x = 0; x < 32; x++) Aes_key[x] = 0;
  x = 0;
  klen = strlen(cipherKey);
  if (klen > 32) klen = 32;
  for (i = 0; i < klen; i++)
    Aes_key[x++] = (Byte)cipherKey[i];
  AesKeyExpansion();
  }

void CipherAes::Init(const char* cipherKey, UInt32 size) {
  Int32 i;
  Int32 x, y;
  Int32 klen;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      state[y][x] = 0;
  Aes_Nk = size * 8 / 32;
  Aes_Nr = Aes_Nk + 6;
  for (x = 0; x < 32; x++) Aes_key[x] = 0;
  x = 0;
  klen = size;
  if (klen > 32) klen = 32;
  for (i = 0; i < klen; i++)
    Aes_key[x++] = (Byte)cipherKey[i];
  AesKeyExpansion();
  }

void CipherAes::AesAddRoundKey(Int32 round) {
  Int32 i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      state[i][j] ^= Aes_roundKey[round * Aes_Nb * 4 + i * Aes_Nb + j];
  }

void CipherAes::AesSubBytes() {
  Int32 i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      state[j][i] = Aes_sbox[state[j][i]];
  }

void CipherAes::AesInvSubBytes() {
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      state[j][i] = Aes_rsbox[state[j][i]];
  }

void CipherAes::AesShiftRows() {
  Byte temp;

  // Rotate first row 1 columns to left	
  temp = state[0][1];
  state[0][1] = state[1][1];
  state[1][1] = state[2][1];
  state[2][1] = state[3][1];
  state[3][1] = temp;

  // Rotate second row 2 columns to left	
  temp = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;

  temp = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // Rotate third row 3 columns to left
  temp = state[0][3];
  state[0][3] = state[3][3];
  state[3][3] = state[2][3];
  state[2][3] = state[1][3];
  state[1][3] = temp;
  }

void CipherAes::AesInvShiftRows() {
  Byte temp;

  // Rotate first row 1 columns to right	
  temp = state[3][1];
  state[3][1] = state[2][1];
  state[2][1] = state[1][1];
  state[1][1] = state[0][1];
  state[0][1] = temp;

  // Rotate second row 2 columns to right	
  temp = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;

  temp = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = state[0][3];
  state[0][3] = state[1][3];
  state[1][3] = state[2][3];
  state[2][3] = state[3][3];
  state[3][3] = temp;
  }

void CipherAes::AesMixColumns() {
  Int32 i;
  Byte Tmp, Tm, t;
  for (i = 0; i < 4; i++) {
    t = state[i][0];
    Tmp = (Byte)(state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3]);
    Tm = (Byte)(state[i][0] ^ state[i][1]);
    Tm = AesXtime(Tm);
    state[i][0] = (Byte)(state[i][0] ^ (Tm ^ Tmp));
    Tm = (Byte)(state[i][1] ^ state[i][2]);
    Tm = AesXtime(Tm);
    state[i][1] = (Byte)(state[i][1] ^ (Tm ^ Tmp));
    Tm = (Byte)(state[i][2] ^ state[i][3]);
    Tm = AesXtime(Tm);
    state[i][2] = (Byte)(state[i][2] ^ (Tm ^ Tmp));
    Tm = (Byte)(state[i][3] ^ t);
    Tm = AesXtime(Tm);
    state[i][3] = (Byte)(state[i][3] ^ (Tm ^ Tmp));
    }
  }

void CipherAes::AesInvMixColumns() {
  Int32 i;
  Byte a, b, c, d;
  for (i = 0; i < 4; i++) {

    a = state[i][0];
    b = state[i][1];
    c = state[i][2];
    d = state[i][3];

    state[i][0] = (Byte)(AesMultiply(a, 0x0e) ^ AesMultiply(b, 0x0b) ^
                         AesMultiply(c, 0x0d) ^ AesMultiply(d, 0x09));
    state[i][1] = (Byte)(AesMultiply(a, 0x09) ^ AesMultiply(b, 0x0e) ^
                         AesMultiply(c, 0x0b) ^ AesMultiply(d, 0x0d));
    state[i][2] = (Byte)(AesMultiply(a, 0x0d) ^ AesMultiply(b, 0x09) ^
                         AesMultiply(c, 0x0e) ^ AesMultiply(d, 0x0b));
    state[i][3] = (Byte)(AesMultiply(a, 0x0b) ^ AesMultiply(b, 0x0d) ^
                         AesMultiply(c, 0x09) ^ AesMultiply(d, 0x0e));
    }
  }

void CipherAes::_encrypt() {
  int round;
  AesAddRoundKey(0);
  for (round = 1; round < Aes_Nr; round++) {
    AesSubBytes();
    AesShiftRows();
    AesMixColumns();
    AesAddRoundKey(round);
    }
  AesSubBytes();
  AesShiftRows();
  AesAddRoundKey(Aes_Nr);
  }

void CipherAes::_decrypt() {
  int round;
  AesAddRoundKey(Aes_Nr);
  for (round = Aes_Nr - 1; round > 0; round--) {
    AesInvShiftRows();
    AesInvSubBytes();
    AesAddRoundKey(round);
    AesInvMixColumns();
    }
  AesInvShiftRows();
  AesInvSubBytes();
  AesAddRoundKey(0);
  }

char* CipherAes::Encrypt(char* inp, int len, char* dest, Int32* length) {
  int i, j, pos;
  UInt64 count;
  String result;
  pos = 0;
  count = counter;
  if (mode == AES_MODE_CBC ||
      mode == AES_MODE_CFB ||
      mode == AES_MODE_OFB) {
    for (i=0; i<4; i++)
      for (j=0; j<4; j++)
        state[i][j] = initVector[i][j];
    }
  while (len > 0) {
    if (mode == AES_MODE_CTR) {
          for (i=0; i<2; i++)
            for (j=0; j<4; j++)
              state[i][j] = initVector[i][j];
          state[3][3] = (count & 0xff);
          state[3][2] = (count >> 8) & 0xff;
          state[3][1] = (count >> 16) & 0xff;
          state[3][0] = (count >> 24) & 0xff;
          state[2][3] = (count >> 32) & 0xff;
          state[2][2] = (count >> 40) & 0xff;
          state[2][1] = (count >> 48) & 0xff;
          state[2][0] = (count >> 56) & 0xff;
          count++;
      }
    else {
      for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {
          if (mode == AES_MODE_CBC) {
            state[i][j] ^= (Byte)((len > 0) ? *inp++ : 0);
            if (len > 0) len--;
            }
          else if (mode == AES_MODE_ECB) {
            state[i][j] = (Byte)((len > 0) ? *inp++ : 0);
            if (len > 0) len--;
            }
          }
       }

    _encrypt();

    result = "";
    for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
        if (mode == AES_MODE_OFB || mode == AES_MODE_CTR) {
          if (len > 0) dest[pos++] = state[i][j] ^ *inp++;
          if (len > 0) len--;
          }
        else if (mode == AES_MODE_CFB) {
          if (len > 0) {
            dest[pos] = state[i][j] ^ *inp++;
            state[i][j] = dest[pos++];
            len--;
            }
          }
        else
          dest[pos++] = state[i][j];
        }
    }
  *length = pos;
  return dest;
  }

char* CipherAes::Decrypt(char* inp, int len, char* dest, Int32* length) {
  Int32 i, j, pos;
  String result;
  if (mode == AES_MODE_OFB ||
      mode == AES_MODE_CTR)
    return Encrypt(inp, len, dest, length);
  pos = 0;
  if (mode == AES_MODE_CBC ||
      mode == AES_MODE_CFB) {
    for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
        if (mode == AES_MODE_CFB) state[i][j] = initVector[i][j];
        comb[i][j] = initVector[i][j];
        }
    }
  while (len > 0) {
    for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
        if (mode == AES_MODE_ECB) state[i][j] = (len > 0) ? (Byte)*inp++ : 0;
        if (mode == AES_MODE_CBC) {
          state[i][j] = (len > 0) ? (Byte)*inp++ : 0;
          enc[i][j] = state[i][j];
          }
        if (mode != AES_MODE_CFB) if (len > 0) len--;
        }

    if (mode == AES_MODE_CFB) _encrypt();
      else _decrypt();

    result = "";
    for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
        if (mode == AES_MODE_CBC) {
          dest[pos++] = state[i][j] ^ comb[i][j];
          comb[i][j] = enc[i][j];
          }
        else if (mode == AES_MODE_CFB) {
          if (len > 0) {
            dest[pos++] = state[i][j] ^ *inp;
            state[i][j] = *inp++;
            len--;
            }
          }
        else
          dest[pos++] = state[i][j];
        }
    }
  *length = pos;
  return dest;
  }

void CipherAes::IV(const Byte* iv) {
  int x,y;
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      initVector[y][x] = (Byte)iv[y * 4 + x];
  }

UInt64 CipherAes::Counter() {
  return counter;
  }

UInt64 CipherAes::Counter(UInt64 i) {
  counter = i;
  return counter;
  }

Byte CipherAes::Mode() {
  return mode;
  }

Byte CipherAes::Mode(const Byte b) {
  if (b == AES_MODE_ECB) mode = b;
  if (b == AES_MODE_CBC) mode = b;
  if (b == AES_MODE_CFB) mode = b;
  if (b == AES_MODE_OFB) mode = b;
  if (b == AES_MODE_CTR) mode = b;
  return b;
  }


