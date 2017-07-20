/* Generate mapping tables of NT kernel error codes
(C) 2017 Niall Douglas <http://www.nedproductions.biz/> (5 commits)
File Created: July 2017


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
(See accompanying file Licence.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifdef _WIN32
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <system_error>
#include <utility>

/* NTSTATUS (LONG) bit format:
//           27                   16 15                            0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code (bits 31, 30)
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag (bit 29)
//
//      R - is a reserved bit (bit 28)
//
//      Facility - is the facility code (bits 27-16)
//
//      Code - is the facility's status code (bits 15-0)

So anything with bit 31 set is an error, they will be prefixed with 0x8 or 0xC.

Highest system facility warning appears to be 0x80000030L
Highest system facility error appears to be 0xC000A2A4L
*/

// clang-format off
static constexpr std::pair<int, const char *> posixmap[] = {
  { 1,"EPERM" },
  { 2,"ENOENT" },
  { 3,"ESRCH" },
  { 4,"EINTR" },
  { 5,"EIO" },
  { 6,"ENXIO" },
  { 7,"E2BIG" },
  { 8,"ENOEXEC" },
  { 9,"EBADF" },
  { 10,"ECHILD" },
  { 11,"EAGAIN" },
  { 12,"ENOMEM" },
  { 13,"EACCES" },
  { 14,"EFAULT" },
  { 16,"EBUSY" },
  { 17,"EEXIST" },
  { 18,"EXDEV" },
  { 19,"ENODEV" },
  { 20,"ENOTDIR" },
  { 21,"EISDIR" },
  { 22,"EINVAL" },
  { 23,"ENFILE" },
  { 24,"EMFILE" },
  { 25,"ENOTTY" },
  { 27,"EFBIG" },
  { 28,"ENOSPC" },
  { 29,"ESPIPE" },
  { 30,"EROFS" },
  { 31,"EMLINK" },
  { 32,"EPIPE" },
  { 33,"EDOM" },
  { 34,"ERANGE" },
  { 36,"EDEADLK" },
  { 38,"ENAMETOOLONG" },
  { 39,"ENOLCK" },
  { 40,"ENOSYS" },
  { 41,"ENOTEMPTY" },
  { 42,"EILSEQ" },
  { 80,"STRUNCATE" },
  { 100,"EADDRINUSE" },
  { 101,"EADDRNOTAVAIL" },
  { 102,"EAFNOSUPPORT" },
  { 103,"EALREADY" },
  { 104,"EBADMSG" },
  { 105,"ECANCELED" },
  { 106,"ECONNABORTED" },
  { 107,"ECONNREFUSED" },
  { 108,"ECONNRESET" },
  { 109,"EDESTADDRREQ" },
  { 110,"EHOSTUNREACH" },
  { 111,"EIDRM" },
  { 112,"EINPROGRESS" },
  { 113,"EISCONN" },
  { 114,"ELOOP" },
  { 115,"EMSGSIZE" },
  { 116,"ENETDOWN" },
  { 117,"ENETRESET" },
  { 118,"ENETUNREACH" },
  { 119,"ENOBUFS" },
  { 120,"ENODATA" },
  { 121,"ENOLINK" },
  { 122,"ENOMSG" },
  { 123,"ENOPROTOOPT" },
  { 124,"ENOSR" },
  { 125,"ENOSTR" },
  { 126,"ENOTCONN" },
  { 127,"ENOTRECOVERABLE" },
  { 128,"ENOTSOCK" },
  { 129,"ENOTSUP" },
  { 130,"EOPNOTSUPP" },
  { 131,"EOTHER" },
  { 132,"EOVERFLOW" },
  { 133,"EOWNERDEAD" },
  { 134,"EPROTO" },
  { 135,"EPROTONOSUPPORT" },
  { 136,"EPROTOTYPE" },
  { 137,"ETIME" },
  { 138,"ETIMEDOUT" },
  { 139,"ETXTBSY" },
  { 140,"EWOULDBLOCK" },
};
// clang-format on

static constexpr std::pair<NTSTATUS, NTSTATUS> inputs[] = {
//
{0x80000001L, 0x8000ffffL},  //
{0xC0000001L, 0xC000ffffL}   //
};

typedef NTSTATUS(NTAPI *RtlUnicodeToUTF8N_t)(_Out_ PCHAR UTF8StringDestination, _In_ ULONG UTF8StringMaxByteCount, _Out_ PULONG UTF8StringActualByteCount, _In_ PCWCH UnicodeStringSource, _In_ ULONG UnicodeStringByteCount);
static RtlUnicodeToUTF8N_t RtlUnicodeToUTF8N;

static inline DWORD win32_error_from_nt_status(NTSTATUS ntstatus)
{
  DWORD br;
  OVERLAPPED o;

  o.Internal = ntstatus;
  o.InternalHigh = 0;
  o.Offset = 0;
  o.OffsetHigh = 0;
  o.hEvent = 0;
  GetOverlappedResult(NULL, &o, &br, FALSE);
  return GetLastError();
}

int main()
{
  std::ofstream oh("../include/detail/ntkernel-table.ipp");
  char txt[32768];
  wchar_t buffer[32768];
  int errc = 0;
  HMODULE ntdll = GetModuleHandle(L"NTDLL.DLL");
  RtlUnicodeToUTF8N = (RtlUnicodeToUTF8N_t) GetProcAddress(ntdll, "RtlUnicodeToUTF8N");
  oh << "// NTSTATUS code, Win32 error code equivalent, POSIX error code equivalent, NTSTATUS descriptive string in UTF8\n";
  for(auto &input : inputs)
  {
    for(NTSTATUS code = input.first; code < input.second; code++)
    {
      DWORD written = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, ntdll, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer) / sizeof(buffer[0]), nullptr);
      if(!written)
      {
        // std::cout << "NTSTATUS code 0x" << std::hex << code << std::dec << " does not have string!" << std::endl;
      }
      else
      {
        buffer[written] = 0;
        NTSTATUS ntstat;
        ULONG len = 0;
        ntstat = RtlUnicodeToUTF8N(txt, sizeof(txt), &len, buffer, (written + 1) * sizeof(wchar_t));
        if(0 == ntstat)
        {
          DWORD win32code = win32_error_from_nt_status(code);
          if(win32code == code)
            win32code = 0;
          std::error_code ec(win32code, std::system_category());
          int _errc = ec.default_error_condition().value();
          const char *errc = "0";
          if(_errc != win32code)
          {
            for(auto &p : posixmap)
            {
              if(p.first == _errc)
                errc = p.second;
            }
          }
          // Format is:
          // ntstatus,win32code,std::errc,"ntstatusstring"
          oh << std::hex << "{ 0x" << code << ",0x" << win32code << "," << errc << ",\"";
          --len;
          if(txt[len - 1] == 10)
            --len;
          for(size_t n = 0; n < len; n++)
          {
            if(10 == txt[n])
              oh << '\\' << 'n';
            else if(13 != txt[n])
            {
              oh << txt[n];
              if(txt[n] == '\\')
                oh << '\\';
            }
          }
          oh << "\"},\n" << std::dec;
        }
        else
        {
          std::cerr << "WARNING: RtlUnicodeToUTF8N() failed with " << ntstat << std::endl;
        }
      }
    }
  }
  // Delete the last comma
  oh.seekp(-3, std::ios::cur);
  oh << '\n';
}

#else
int main()
{
  std::cerr << "This program can only work on Windows" << std::endl;
  return 1;
}
#endif