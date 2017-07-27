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

using field = ntkernel_error_category::detail::field;
static constexpr field table[] = {
#include "../include/detail/ntkernel-table.ipp"
};

int main()
{
  using namespace ntkernel_error_category;
  int retcode = 0;

  static constexpr auto table_view = boost::static_views::raw_view(table);
  static constexpr auto posix_to_ntstatus_map = //
    boost::static_views::static_map::make_static_map<1, table_view.size()>(  //
      table_view, //
    &field::posix, //
    &field::ntstatus //
    );
  static constexpr auto ntstatus_to_posix_map = //
    boost::static_views::static_map::make_static_map<1, table_view.size()>(  //
      table_view, //
      &field::ntstatus, //
      &field::posix //
      );
  static constexpr auto ntstatus_to_message_map = //
    boost::static_views::static_map::make_static_map<1, table_view.size()>(  //
      table_view, //
      &field::ntstatus, //
      &field::message //
      );
  std::cout << "The following NTSTATUS codes are mapped by these POSIX codes:\n";
  {
    static constexpr int values[] = {
      posix_to_ntstatus_map[EACCES],
      posix_to_ntstatus_map[EAGAIN],
      posix_to_ntstatus_map[EBUSY],
      posix_to_ntstatus_map[ENOSYS],
      posix_to_ntstatus_map[EINVAL],
      posix_to_ntstatus_map[ENOENT],
      posix_to_ntstatus_map[ENOMEM],
      posix_to_ntstatus_map[EEXIST],
      posix_to_ntstatus_map[ENOLCK],
      posix_to_ntstatus_map[ENOSPC],
      posix_to_ntstatus_map[ENODEV],
      posix_to_ntstatus_map[EXDEV],
      posix_to_ntstatus_map[ENOTEMPTY],
      posix_to_ntstatus_map[EMFILE],
      posix_to_ntstatus_map[ECANCELED]
    };
    for(auto &v : values)
      std::cout << "  " << std::hex << v << std::dec << "(" << ntstatus_to_message_map[v] << ")" << "\n";
  }
  return retcode;
}