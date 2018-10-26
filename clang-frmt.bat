for /F %%F in ('dir /b *.c') do clang-format.exe -i -style=file %%F
for /F %%F in ('dir /b *.h') do clang-format.exe -i -style=file %%F

for /F %%F in ('dir /b example-mikroC-AVR\*.c') do clang-format.exe -i -style=file example-mikroC-AVR\%%F
for /F %%F in ('dir /b example-mikroC-AVR\*.h') do clang-format.exe -i -style=file example-mikroC-AVR\%%F

for /F %%F in ('dir /b example-mikroC-PIC\*.c') do clang-format.exe -i -style=file example-mikroC-PIC\%%F
for /F %%F in ('dir /b example-mikroC-PIC\*.h') do clang-format.exe -i -style=file example-mikroC-PIC\%%F

for /F %%F in ('dir /b example-mikroC-ARM\*.c') do clang-format.exe -i -style=file example-mikroC-ARM\%%F
for /F %%F in ('dir /b example-mikroC-ARM\*.h') do clang-format.exe -i -style=file example-mikroC-ARM\%%F
:pause

copy2atollic.bat