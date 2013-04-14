API Documentation
=================
The API documentation is available in the source code repository under
./Developer_Docs/ModularizedAI/html/index.html


Coding style guidelines
=======================
When modifying or adding to the code in ModularizedTacticalAI, plese adhere to
the following rules in order to keep the place tidy.

- Do indent code blocks by 4 spaces
- Do not use tabs; they are interpreted differently on each platform, messing up the indention
- Do not #include compilation units (cpp files). Doing so will lead to you facing the firing squad.
- Do try to limit the number of include files to what is really needed. If a lot is needed, consider using the Pimpl
  idiom.
- Do not use leading underscores or double underscores. They are reserved for compiler implementation.
- Do document your code using Doxygen-style comments, and update the doxygen documentation by running doxygen in the
  ModularizedTacticalAI directory.
- Do use identifier names that reflect what you are doing.
- Do use CamelCase for classes, and all_lowercase_characters() for function names and attributes
- Do use one trailing underscore for class_members_
- Do not event think about adding a global variable, goto, macros (unless absolutely needed) or other BS.
- Do follow the C++ specifications, as in ISO 14882
- Do, if you feel that these guidelines are an impediment for your work, discuss alternatives at bears-pit.com


