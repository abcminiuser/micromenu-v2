for /F %%F in ('dir /b *.c') do clang-format.exe -i -style=file %%F
for /F %%F in ('dir /b *.h') do clang-format.exe -i -style=file %%F
:pause