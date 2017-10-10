// BSD 3-Clause License
// 
// Copyright (c) 2017, Adrien Guinet (adrien@guinet.me)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <system_error>
#include <limits>

#include "sec_random.h"

namespace {

void __throw_system_error(int ev, const char* what)
{
   throw std::system_error{std::error_code{ev, std::system_category()}, what};
}

} // anonymous

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>

SecRandom::SecRandom()
{
  if (!CryptAcquireContext(&HProv_, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    __throw_system_error(GetLastError(), "error initializing CryptAcquireContext");
  }
}

SecRandom::~SecRandom()
{
  CryptReleaseContext(HProv_, 0);
}

SecRandom::result_type SecRandom::operator()()
{
  result_type Ret;
  if (!CryptGenRandom(HProv_, sizeof(result_type), reinterpret_cast<BYTE*>(&Ret))) {
    __throw_system_error(GetLastError(), "unable to generate a random number");
  }
  return Ret;
}

#elif defined(__unix__)

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Inspired by http://llvm.org/svn/llvm-project/libcxx/trunk/src/random.cpp
SecRandom::SecRandom()
{
  fd_ = open("/dev/urandom", O_RDONLY);
  if (fd_ < 0) {
    __throw_system_error(errno, "failed to open /dev/urandom");
  }
}

SecRandom::~SecRandom()
{
  close(fd_);
}

SecRandom::result_type SecRandom::operator()()
{
  result_type r;
  size_t n = sizeof(r);
  char* p = reinterpret_cast<char*>(&r);
  while (n > 0)
  {
    ssize_t s = read(fd_, p, n);
    if (s == 0)
      __throw_system_error(ENODATA, "random_device got EOF");
    if (s == -1) 
    {   
      if (errno != EINTR)
        __throw_system_error(errno, "random_device got an unexpected error");
      continue;
    }   
    n -= static_cast<size_t>(s);
    p += static_cast<size_t>(s);
  }
  return r;
}

#elif defined(__APPLE__)
#include <cstdlib>
#include <cstdint>

SecRandom::result_type SecRandom::operator()()
{
  return arc4random();
}

#endif
