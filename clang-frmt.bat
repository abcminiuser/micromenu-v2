for /F %%F in ('dir /b *.c') do clang-format.exe -i -style=file %%F
for /F %%F in ('dir /b *.h') do clang-format.exe -i -style=file %%F
for /F %%F in ('dir /b example-mikroC-AVR\*.c') do clang-format.exe -i -style=file example-mikroC-AVR\%%F
for /F %%F in ('dir /b example-mikroC-AVR\*.h') do clang-format.exe -i -style=file example-mikroC-AVR\%%F
:pause