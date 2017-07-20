A portable C++ 11 STL `std::error_category` implementation for the NT kernel
error code space i.e. `NTSTATUS`. Implemented at `ntkernel_error_category::ntkernel_category()`.

Features:

- Provides a complete set of string messages for the system facility, as
according to Microsoft Windows 10, for the warning and error categories. Note
that the strings are in US English only.
- Implements mapped comparisons to error conditions of `std::system_category`
on Windows (i.e. the `GetLastError()` error code space).
- Implements mapped comparisons to error conditions of `std::generic_category`
on all platforms (i.e. the POSIX `errno` error code space).
- Comes in three implementation variants:
  - Header-only table (`NTKERNEL_ERROR_CATEGORY_INLINE`).
  - Static library table (`NTKERNEL_ERROR_CATEGORY_STATIC`).
  - Shared library table.
  
The default of `NTKERNEL_ERROR_CATEGORY_STATIC` and `NTKERNEL_ERROR_CATEGORY_INLINE`
both NOT being defined means that the shared library table is the default. As
`std::error_category` instances must be globally unique in a process
as per the C++ standard, this form - despite its inconvenience - is the
only standards conforming one. Any other setting may lead to more than
one instance of the singleton, this can cause misoperation especially during
error condition comparisons.
