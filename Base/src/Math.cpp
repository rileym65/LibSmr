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
#include "SmrFramework.h"

namespace SmrFramework {

  Int8 Math::Abs(Int8 n) {
    return (n >= 0) ? n : -n;
    }

  Int16 Math::Abs(Int16 n) {
    return (n >= 0) ? n : -n;
    }

  Int32 Math::Abs(Int32 n) {
    return (n >= 0) ? n : -n;
    }

  Int64 Math::Abs(Int64 n) {
    return (n >= 0) ? n : -n;
    }

  Single Math::Abs(Single n) {
    return (n >= 0) ? n : -n;
    }

  Double Math::Abs(Double n) {
    return (n >= 0) ? n : -n;
    }

  double Math::ACos(double n) {
    n *= n;
    return ATan(Sqrt((1-n) / n));
    }

  double Math::ACosH(double n) {
    return Log(Sqrt(n * n - 1) + n);
    }

  double Math::ASin(double n) {
    n *= n;
    return ATan(Sqrt(n/(1-n)));
    }

  double Math::ASinH(double n) {
    return Log(Sqrt(n * n + 1) + n);
    }

  double Math::ATan(double n) {
    double sum;
    double k;
    double last;
    double sgn;
    double pwr;
    int    cnt;
    k = 3;
    pwr = n;
    sum = n;
    last = 99999;
    cnt = 1000;
    n *=n;
    if (n <= 1) {
      sgn = 1;
      while (last != sum && cnt-- > 0) {
        last = sum;
        pwr *= n;
        sgn *= -1;
        sum += (sgn * pwr/k);
        k += 2;
        }
      } else {
      sum = (3.14159265358979323846 / 2) - (1 / sum);
      sgn = -1;
      while (last != sum && cnt-- > 0) {
        last = sum;
        pwr *= n;
        sgn *= -1;
        sum += (sgn / (k*pwr));
        k += 2;
        }
      }
    return sum;
    }

  double Math::ATanH(double n) {
    return Log((1 + n) / (1 - n)) / 2;
    }

  Int32 Math::Ceiling(Single n) {
    if (n <= 0) return (Int32)n;
    return ((n - (Int32)n)  == 0) ? (Int32)n : (Int32)n + 1;
    }

  Int32 Math::Ceiling(Double n) {
    if (n < 0) return (Int32)n;
    return ((n - (Int32)n)  == 0) ? (Int32)n : (Int32)n + 1;
    }

  double Math::Cos(double angle) {
    double sum;
    double last;
    double k;
    double sgn;
    double pwr;
    double fct;
    double fctCount;
    sum = 1;
    last = 0;
    k = 1;
    sgn = 1;
    pwr = 1;
    fct = 1;
    fctCount = 1;
    angle *= angle;
    while (last != sum) {
      last = sum;
      sgn *= -1;
      pwr *= angle;
      fct *= fctCount++;
      fct *= fctCount++;
      sum += ( (sgn * pwr) / fct);
      k++;
      }
    return sum;
    }

  Int8 Math::Aug(Int8 n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  Int16 Math::Aug(Int16 n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  Int32 Math::Aug(Int32 n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  Int64 Math::Aug(Int64 n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  Single Math::Aug(Single n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  Double Math::Aug(Double n) {
    if (n > 0) return n+1;
    if (n < 0) return n-1;
    return 0;
    }

  double Math::CosH(double angle) {
    return (Exp(angle) + Exp(-angle)) / 2;
    }

  double Math::DegToRad(double deg) {
    return deg * 1.74532925199433E-002;
    }

  Int8 Math::Dim(Int8 n) {
    if (n > 0) return n-1;
    if (n < 0) return n+1;
    return 0;
    }

  Int16 Math::Dim(Int16 n) {
    if (n > 0) return n-1;
    if (n < 0) return n+1;
    return 0;
    }

  Int32 Math::Dim(Int32 n) {
    if (n > 0) return n-1;
    if (n < 0) return n+1;
    return 0;
    }

  Int64 Math::Dim(Int64 n) {
    if (n > 0) return n-1;
    if (n < 0) return n+1;
    return 0;
    }

  Single Math::Dim(Single n) {
    if (n > 0) {
      n -= 1;
      if (n < 0) n = 0;
      return n;
      }
    if (n < 0) {
      n += 1;
      if (n > 0) n = 0;
      return n;
      }
    return 0;
    }

  Double Math::Dim(Double n) {
    if (n > 0) {
      n -= 1;
      if (n < 0) n = 0;
      return n;
      }
    if (n < 0) {
      n += 1;
      if (n > 0) n = 0;
      return n;
      }
    return 0;
    }

  double Math::Exp(double n) {
    double sum;
    double last;
    double fct;
    double fctCount;
    double pwr;
    last = 0;
    pwr = n;
    fct = 1;
    fctCount = 2;
    sum = 1 + n;
    while (last != sum) {
      last = sum;
      pwr *= n;
      fct *= fctCount++;
      sum += (pwr / fct);
      }
    return sum;
    }

  double Math::Exp10(double n) {
    return Exp(n * 2.30258509299404568402);
    }

  Int32 Math::Floor(Single n) {
    if (n >= 0) return (Int32)n;
    return ((n - (Int32)n)  == 0) ? (Int32)n : (Int32)n - 1;
    }

  Int32 Math::Floor(Double n) {
    if (n >= 0) return (Int32)n;
    return ((n - (Int32)n)  == 0) ? (Int32)n : (Int32)n - 1;
    }

  double Math::Log(double n) {
    double sum;
    double last;
    double k;
    double pwr;
    int    cnt;
    n = (n-1)/(n+1);
    sum = 0;
    pwr = n;
    k = 1;
    last = 1;
    cnt = 20;
    n *= n;
    while (last != sum) {
      last = sum;
      sum += pwr / k;
      pwr *= n;
      k+=2;
      cnt--;
      }
    return sum * 2;
    }

  double Math::Log10(double n) {
    return Log(n) / 2.30258509299404568402;
    }

  Int8 Math::Max(Int8 a, Int8 b) {
    return (a >= b) ? a : b;
    }

  Int16 Math::Max(Int16 a, Int16 b) {
    return (a >= b) ? a : b;
    }

  Int32 Math::Max(Int32 a, Int32 b) {
    return (a >= b) ? a : b;
    }

  Int64 Math::Max(Int64 a, Int64 b) {
    return (a >= b) ? a : b;
    }

  UInt8 Math::Max(UInt8 a, UInt8 b) {
    return (a >= b) ? a : b;
    }

  UInt16 Math::Max(UInt16 a, UInt16 b) {
    return (a >= b) ? a : b;
    }

  UInt32 Math::Max(UInt32 a, UInt32 b) {
    return (a >= b) ? a : b;
    }

  UInt64 Math::Max(UInt64 a, UInt64 b) {
    return (a >= b) ? a : b;
    }

  Single Math::Max(Single a, Single b) {
    return (a >= b) ? a : b;
    }

  Double Math::Max(Double a, Double b) {
    return (a >= b) ? a : b;
    }

  Int8 Math::Min(Int8 a, Int8 b) {
    return (a <= b) ? a : b;
    }

  Int16 Math::Min(Int16 a, Int16 b) {
    return (a <= b) ? a : b;
    }

  Int32 Math::Min(Int32 a, Int32 b) {
    return (a <= b) ? a : b;
    }

  Int64 Math::Min(Int64 a, Int64 b) {
    return (a <= b) ? a : b;
    }

  UInt8 Math::Min(UInt8 a, UInt8 b) {
    return (a <= b) ? a : b;
    }

  UInt16 Math::Min(UInt16 a, UInt16 b) {
    return (a <= b) ? a : b;
    }

  UInt32 Math::Min(UInt32 a, UInt32 b) {
    return (a <= b) ? a : b;
    }

  UInt64 Math::Min(UInt64 a, UInt64 b) {
    return (a <= b) ? a : b;
    }

  Single Math::Min(Single a, Single b) {
    return (a <= b) ? a : b;
    }

  Double Math::Min(Double a, Double b) {
    return (a <= b) ? a : b;
    }

  double Math::Pow(double x, double y) {
    return Exp(Log(x) * y);
    }

  double Math::RadToDeg(double rad) {
    return rad * 5.72957795130823E+001;
    }

  double Math::Ratio(double start, double end, double index) {
    double range;
    range = end - start;
    if (range < 0) range = -range;
    return (index - start) / range;
    }

  double Math::Root(double n, double rt) {
    return Pow(n, (1 / rt));
    }

  Int32 Math::Round(Single n) {
    if (n >= 0) return (Int32)(n+0.5);
    return (Int32)(n-0.5);
    }

  Int32 Math::Round(Double n) {
    if (n >= 0) return (Int32)(n+0.5);
    return (Int32)(n-0.5);
    }

  double Math::Sign(double n) {
    if (n > 0) return 1.0;
    if (n < 0) return -1.0;
    return 0;
    }

  double Math::Sin(double angle) {
    double sum;
    double last;
    double k;
    double sgn;
    double pwr;
    double fct;
    double fctCount;
    sum = angle;
    last = 0;
    k = 1;
    sgn = 1;
    pwr = angle;
    fct = 1;
    fctCount = 2;
    angle *= angle;
    while (last != sum) {
      last = sum;
      sgn *= -1;
      pwr *= angle;
      fct *= fctCount++;
      fct *= fctCount++;
      sum += ( (sgn * pwr) / fct);
      k++;
      }
    return sum;
    }

  double Math::SinH(double angle) {
    return (Exp(angle) - Exp(-angle)) / 2;
    }

  double Math::Sqr(double n) {
    return n * n;
    }

  double Math::Sqrt(double n) {
    return Pow(n, 0.5);
    }

  double Math::Tan(double n) {
    return Sin(n) / Cos(n);
    }

  double Math::TanH(double angle) {
    return SinH(angle) / CosH(angle);
    }

  Int32 Math::Trunc(Single n) {
    return (Int32)n;
    }

  Int32 Math::Trunc(Double n) {
    return (Int32)n;
    }

  }
