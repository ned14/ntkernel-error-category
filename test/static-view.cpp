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

#include "../static-views/example/static_map.hpp"
#include "ntkernel_category.hpp"

#include <iostream>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

static constexpr std::tuple<int, int, int, const char *> table[] = {
#include "../include/detail/ntkernel-table.ipp"
};

int main()
{
  using namespace ntkernel_error_category;
  int retcode = 0;

  constexpr auto table_view = boost::static_views::raw_view(table);
  constexpr auto ntstatus_to_message_map = make_static_map<2 * sizeof(table) / sizeof(table[0]), 2>(table_view);
  return retcode;
}