#include <stdio.h>
#include <string.h>
#include <SmrFramework.h>
#include "header.h"

using namespace SmrFramework;

void TestTests() {
  printf("\nInfo: Tests tests\n");
  if (Tests::IsInteger("451"))
    Good("IsInteger returned true for 451");
    else Bad("IsInteger did not return true for 451");
  if (Tests::IsInteger("-451"))
    Good("IsInteger returned true for -451");
    else Bad("IsInteger did not return true for -451");
  if (Tests::IsInteger("+451"))
    Good("IsInteger returned true for +451");
    else Bad("IsInteger did not return true for +451");
  if (!Tests::IsInteger("+45.1"))
    Good("IsInteger returned false for +45.1");
    else Bad("IsInteger did not return false for +45.1");
  if (!Tests::IsInteger("45e1"))
    Good("IsInteger returned false for 45e1");
    else Bad("IsInteger did not return false for 45e1");
  if (Tests::IsFloat("45.1"))
    Good("IsFloat returned true for 45.1");
    else Bad("IsFloat did not return true for 45.1");
  if (Tests::IsFloat("-45.1"))
    Good("IsFloat returned true for -45.1");
    else Bad("IsFloat did not return true for -45.1");
  if (Tests::IsFloat("+45.1"))
    Good("IsFloat returned true for +45.1");
    else Bad("IsFloat did not return true for +45.1");
  if (!Tests::IsFloat("+4.5.1"))
    Good("IsFloat returned false for +4.5.1");
    else Bad("IsFloat did not return false for +4.5.1");
  if (Tests::IsBinary("11010"))
    Good("IsBinary returned true for 11010");
    else Bad("IsBinary did not return true for 11010");
  if (!Tests::IsBinary("11020"))
    Good("IsBinary returned false for 11020");
    else Bad("IsBinary did not return false for 11020");
  if (Tests::IsOctal("14762"))
    Good("IsOctal returned true for 14762");
    else Bad("IsOctal did not return true for 14762");
  if (!Tests::IsOctal("14782"))
    Good("IsOctal returned false for 14782");
    else Bad("IsOctal did not return false for 14782");
  if (Tests::IsHex("14e62"))
    Good("IsHex returned true for 14e62");
    else Bad("IsHex did not return true for 14e62");
  if (Tests::IsHex("14E62"))
    Good("IsHex returned true for 14E62");
    else Bad("IsHex did not return true for 14E62");
  if (!Tests::IsHex("14G62"))
    Good("IsHex returned false for 14G62");
    else Bad("IsHex did not return false for 14G62");
  if (Tests::IsAlpha("abxyz"))
    Good("IsAlpha returned true for abxyz");
    else Bad("IsAlpha did not return true for abxyz");
  if (!Tests::IsAlpha("ab2xyz"))
    Good("IsAlpha returned false for ab2xyz");
    else Bad("IsAlpha did not return false for ab2xyz");
  if (!Tests::IsAlpha('4'))
    Good("IsAlpha returned false for 4");
    else Bad("IsAlpha did not return false for 4");
  if (Tests::IsNumeric("16576"))
    Good("IsNumeric returned true for 16576");
    else Bad("IsNumeric did not return true for 16576");
  if (!Tests::IsNumeric("16A76"))
    Good("IsNumeric returned false for 16A76");
    else Bad("IsNumeric did not return false for 16A76");
  if (Tests::IsNumeric('4'))
    Good("IsNumeric returned true for 4");
    else Bad("IsNumeric did not return true for 4");
  if (!Tests::IsNumeric('a'))
    Good("IsNumeric returned false for a");
    else Bad("IsNumeric did not return false for a");
  if (Tests::IsAlphaNumeric("abc123"))
    Good("IsAlphaNumeric returned true for abc123");
    else Bad("IsAlphaNumeric did not return true for abc123");
  if (!Tests::IsAlphaNumeric("abc!23"))
    Good("IsAlphaNumeric returned false for abc!23");
    else Bad("IsAlphaNumeric did not return false for abc!23");
  if (Tests::IsAlphaNumeric('a'))
    Good("IsAlphaNumeric returned true for a");
    else Bad("IsAlphaNumeric did not return true for a");
  if (Tests::IsAlphaNumeric('5'))
    Good("IsAlphaNumeric returned true for 5");
    else Bad("IsAlphaNumeric did not return true for 5");
  if (!Tests::IsAlphaNumeric('@'))
    Good("IsAlphaNumeric returned false for @");
    else Bad("IsAlphaNumeric did not return false for @");
  }
