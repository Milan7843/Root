# Root
Root is a game creation framework for 2D games, using Visual Studio c++.

# Install guide
- Clone this repository near your own project.
- In your project properties:
  - In VC++ directories, add the absolute path to `Root\x64\Debug` to the Library Directories. E.g. `C:\Git\Root\x64\Debug`
  - In C/C++, add the absolute path to `Root\Root\src` to the Additional Include Directories. E.g. `C:\Git\Root\Root\src`
  - In Linker, add `Root.lib;` to the Additional Dependencies.
