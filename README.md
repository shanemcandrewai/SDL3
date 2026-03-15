# SDL3 development

    gcc -o build/hello.exe hello.c -ISDL3-3.4.2x86_64-w64-mingw32/include -LSDL3-3.4.2/x86_64-w64-mingw32/lib -lSDL3
    gcc -o build/woodeneye-008.exe -Wall -Wpedantic -Wextra woodeneye-008.c -ISDL3-3.4.2x86_64-w64-mingw32/include -lSDL3