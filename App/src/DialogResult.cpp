#include <SmrFramework.h>
#include <SmrAppFramework.h>

namespace SmrFramework {

  DialogResult::DialogResult() {
    result = DialogResult::CANCEL;
    }
  
  DialogResult::DialogResult(Byte r) {
    result = r;
    }
  
  DialogResult::~DialogResult() {
    }
  
  Byte DialogResult::Result() {
    return result;
    }

  }
