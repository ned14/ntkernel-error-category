/* NT kernel error code category testing
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

#include "ntkernel_category.hpp"

#include <iostream>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#define CHECK(expr)                                                                                                                                                                                                                                                                                                            \
  if(!(expr))                                                                                                                                                                                                                                                                                                                  \
  {                                                                                                                                                                                                                                                                                                                            \
    std::cerr << #expr " failed at line " << __LINE__ << std::endl;                                                                                                                                                                                                                                                            \
    retcode = 1;                                                                                                                                                                                                                                                                                                               \
  }

int main()
{
  using namespace ntkernel_error_category;
  int retcode = 0;
  {
    std::error_code ec(static_cast<int>(0xc000003a), ntkernel_category());  // STATUS_OBJECT_PATH_NOT_FOUND
    CHECK(!strcmp(ec.message().c_str(), "{Path Not Found}\nThe path %hs does not exist."));
    // This is mapped to generic_category
    CHECK(ec.default_error_condition().category() == std::generic_category());
    CHECK(ec == std::errc::no_such_file_or_directory);
#ifdef _WIN32
    // Check we can compare to win32 error codes
    std::error_code winec(ERROR_PATH_NOT_FOUND, std::system_category());
    // Compare via mapping winec to generic_category and comparing generic_categories
    CHECK(ec == winec.default_error_condition());
    // Compare via mapping ec to generic_category and comparing generic_categories
    CHECK(ec.default_error_condition() == winec);
#endif
  }
  {
    std::error_code ec(static_cast<int>(0xc000003b), ntkernel_category());  // STATUS_OBJECT_PATH_SYNTAX_BAD
    CHECK(!strcmp(ec.message().c_str(), "Object Path Component was not a directory object."));
    // This has no mapping to generic_category
    CHECK(ec.default_error_condition().category() == ntkernel_category());
#ifdef _WIN32
    // Check we can compare to win32 error codes
    std::error_code winec(ERROR_BAD_PATHNAME, std::system_category());
    // Compare via ntkernel_category's special support for system_category
    CHECK(ec == winec.default_error_condition());
    // Compare via ntkernel_category's special support for system_category
    CHECK(ec.default_error_condition() == winec);
#endif
  }
  return retcode;
}