# QtFinder
a wrap of **ripgrep** tool using Qt ui

QtFinder is a **[rg](https://github.com/BurntSushi/ripgrep)** and **[fd](https://github.com/BurntSushi/ripgrep)** ui front end tool.

## features
1. using rg search file contents
2. using fd search files (**fast**)
3. file preview, highlighting files
4. preview markdown

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

