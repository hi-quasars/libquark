
/* *********************
 *  a block-based encryption library
 *  
 */

#include <iostream>
#include <cstring>
#include <cerrno>

#include "QEncry.h"

namespace Quark {
    QuarkAppendFile* QuarkAppendFile::NewReadFile(const char*) {
      // open
      
      // check
      return NULL;
    }
    QuarkAppendFile* QuarkAppendFile::NewWriteFile(const char*) {
      return NULL;
    }
    QuarkAppendFile* QuarkAppendFile::ImportFromPlainFile(const char*, int rw) {
      return NULL;
    }
    
    int QuarkAppendFile::Read1Block() {
      
    }

    int QuarkAppendFile::Write1Block() {
      
    }

}  // namespace Quark
