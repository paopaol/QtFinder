# QtFinder
a wrap of **ripgrep** tool using Qt ui

QtFinder is a **[fd](https://github.com/BurntSushi/ripgrep)** ui front end tool.

## features
  using fd search files (**fast**)

  Support fuzzy search, keyword separation by space

  Support for regular expressions

## build

 * linux

```sh
    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_PREFIX_PATH=/opt/Qt5.11.1/5.11.1/gcc_64
    cmake --build build
```

 * windows
```sh
    cmake -H\. -Bbuild -G"Visual Studio 14 2015 Win64" -DCMAKE_PREFIX_PATH=C:/Qt/Qt5.9.0/5.9/msvc2015_x64
    cmake --build build
```

