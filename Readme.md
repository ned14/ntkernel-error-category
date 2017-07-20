A portable C++ 11 STL `std::error_category` implementation for the NT kernel
error code space i.e. `NTSTATUS`. Features:

- Provides a complete set of string messages for the system facility (as
according to Microsoft Windows 10). Note that the strings are in US English
only.
- Implements comparisons to `std::system_category` on Windows (i.e. the
`GetLastError()` error code space).
- Implements comparisons to `std::generic_category` on all platforms
(i.e. the POSIX `errno` error code space).
- Comes in three implementation variants:
  - Compile-time table (requires C++ 14, note this is hard on the
  compiler, but it has zero runtime overhead).
  - Header-only table.
  - Static library table.
