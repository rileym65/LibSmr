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
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  UInt32 _primes[] = {
    2,3,5,7,11,13,17,19,23,29, 31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113, 127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
    283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,
    419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,
    547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,
    661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,
    811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,
    947,953,967,971,977,983,991,997,0
    };

  Fraction::Fraction(Double n) {
    UInt32 next;
    UInt32 count;
    Fraction temp(0,0,1);
    negative = (n < 0);
    if (negative) n = -n;
    temp = Fraction(n,0,1);
    whole = (UInt32)n;
    n -= whole;
    next = 2;
    count = 0;
    while (n != 0 && count < 24) {
      n *= 2;
      if (n >= 1.0) {
        temp = temp + Fraction(1,next);
        n -= (Int32)n;
        }
      next <<= 1;
      count++;
      }
    numerator = temp.Numerator();
    denominator = temp.Denominator();
    whole = temp.Whole();
    }

  Fraction::Fraction(Int32 n, Int32 d) {
    objectType = (char*)"Fraction";
    if (d == 0 || n == 0) {
      d = 1;
      n = 0;
      }
    negative = false;
    if (n < 0) {
      n = -n;
      negative = true;
      }
    if (d < 0) {
      d = -d;
      negative = true;
      }
    numerator = n;
    denominator = d;
    whole = 0;
    reduce();
    }

  Fraction::Fraction(Int32 w, Int32 n, Int32 d) {
    objectType = (char*)"Fraction";
    if (d == 0 || n == 0) {
      d = 1;
      n = 0;
      }
    negative = false;
    if (w < 0) {
      w = -w;
      negative = true;
      }
    if (n < 0) {
      n = -n;
      negative = true;
      }
    if (d < 0) {
      d = -d;
      negative = true;
      }
    numerator = n;
    denominator = d;
    whole = w;
    reduce();
    }
  
  Fraction::~Fraction() {
    }
  
  UInt32 Fraction::leastCommonDenominator(UInt32 d1, UInt32 d2) {
    UInt32 lcd;
    UInt32 prime;
    if (d1 == d2) return d1;
    if (d1 % d2 == 0) return d1;
    if (d2 % d1 == 0) return d2;
    lcd = 1;
    prime = 0;
    while (_primes[prime] != 0 && (d1 != 1 || d2 != 1)) {
      while ((d1 % _primes[prime] == 0) || (d2 % _primes[prime] == 0)) {
        lcd *= _primes[prime];
        if (d1 % _primes[prime] == 0) d1 /= _primes[prime];
        if (d2 % _primes[prime] == 0) d2 /= _primes[prime];
        }
      prime++;
      }
    if (d1 == 1 && d2 == 1) return lcd;
    prime = 1009;
    while ((prime <= d1) || (prime <= d2)) {
      if ((d1 % prime == 0) || (d2 % prime == 0)) {
        lcd *= prime;
        if (d1 % prime == 0) d1 /= prime;
        if (d2 % prime == 0) d2 /= prime;
        }
      prime += 2;
      }
    return lcd;
    }

  void   Fraction::reduce() {
    UInt32 prime;
    if (numerator >= denominator) {
      whole += (numerator / denominator);
      numerator = numerator % denominator;
      }
    if (numerator == 0 || numerator == 1) return;
    if (denominator % numerator == 0) {
      denominator = denominator / numerator;
      numerator = 1;
      return;
      }
    prime = 0;
    while (_primes[prime] != 0) {
      if ((numerator % _primes[prime] == 0) &&
          (denominator % _primes[prime] == 0)) {
        numerator /= _primes[prime];
        denominator /= _primes[prime];
        if (numerator == 1) return;
        }
      prime++;
      if (_primes[prime] != 0 && _primes[prime] > denominator) return;
      }
    prime = 1009;
    while (numerator != 1 && prime < denominator) {
      if ((numerator % prime == 0) &&
          (denominator % prime == 0)) {
        numerator /= prime;
        denominator /= prime;
        }
      prime += 2;
      }
    }

  Fraction Fraction::operator+ (const Fraction& a) {
    UInt32 n;
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    n = n1 + n2;
    return Fraction(0, n, d);
    }

  Fraction Fraction::operator- (const Fraction& a) {
    UInt32 n;
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    n = n1 - n2;
    return Fraction(0, n, d);
    }

  Fraction Fraction::operator* (const Fraction& a) {
    UInt32 n;
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (whole != 0) n1 += (whole * d1);
    if (a.whole != 0) n2 += (a.whole * d2);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    n = n1 * n2;
    d = d1 * d2;
    return Fraction(0, n, d);
    }

  Fraction Fraction::operator/ (const Fraction& a) {
    UInt32 n;
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (whole != 0) n1 += (whole * d1);
    if (a.whole != 0) n2 += (a.whole * d2);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    n = n1 * d2;
    d = d1 * n2;
    return Fraction(0, n, d);
    }

  bool Fraction::operator== (const Fraction& a) {
    if (negative != a.negative) return false;
    if (numerator != a.numerator) return false;
    if (denominator != a.denominator) return false;
    return true;
    }

  bool Fraction::operator!= (const Fraction& a) {
    if (negative != a.negative) return true;
    if (numerator != a.numerator) return true;
    if (denominator != a.denominator) return true;
    return false;
    }

  bool Fraction::operator> (const Fraction& a) {
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    return (n1 > n2);
    }

  bool Fraction::operator>= (const Fraction& a) {
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    return (n1 >= n2);
    }

  bool Fraction::operator< (const Fraction& a) {
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    return (n1 < n2);
    }

  bool Fraction::operator<= (const Fraction& a) {
    UInt32 d;
    UInt32 n1, n2;
    UInt32 d1, d2;
    n1 = numerator;
    n2 = a.numerator;
    d1 = denominator;
    d2 = a.denominator;
    if (d1 == d2) {
      d = d1;
      }
    else {
      d = leastCommonDenominator(d1, d2);
      n1 *= (d / d1);
      n2 *= (d / d2);
      }
    if (whole != 0) n1 += (whole * d);
    if (a.whole != 0) n2 += (a.whole * d);
    if (negative) n1 = -n1;
    if (a.negative) n2 = -n2;
    return (n1 <= n2);
    }

  UInt32 Fraction::Denominator() {
    return denominator;
    }

  UInt32 Fraction::Denominator(UInt32 d) {
    denominator = d;
    return denominator;
    }
  
  bool   Fraction::Negative() {
    return negative;
    }

  bool   Fraction::Negative(bool b) {
    negative = b;
    return negative;
    }

  UInt32 Fraction::Numerator() {
    return numerator;
    }

  UInt32 Fraction::Numerator(UInt32 n) {
    numerator = n;
    return numerator;
    }
  
  String Fraction::ToString() {
    char buffer[256];
    if (whole == 0) {
      if (negative) sprintf(buffer,"-%d/%d", numerator, denominator);
       else sprintf(buffer,"%d/%d", numerator, denominator);
      }
    else {
      if (negative) sprintf(buffer,"-%d %d/%d", whole, numerator, denominator);
        else sprintf(buffer,"%d %d/%d", whole, numerator, denominator);
      }
    return String(buffer);
    }

  UInt32 Fraction::Whole() {
    return whole;
    }

  UInt32 Fraction::Whole(UInt32 w) {
    whole = w;
    return whole;
    }

  }

