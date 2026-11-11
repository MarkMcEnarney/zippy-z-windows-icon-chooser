# Building Zippy Z from source

Zippy Z is built as a native 64-bit Windows PE executable from `src/ZippyZ.c` and `src/chkstk.s`. The current source was built using **Clang 17 / LLVM LLD** targeting `x86_64-windows-msvc`, without a Windows SDK or third-party runtime dependency.

The build commands below are for a development environment with **Clang** and **LLD** installed. They were executed successfully in the authoring environment; Windows 11 GUI execution has **not** been tested here.

From the `src` folder (use a command-line shell that invokes these LLVM tools):

```sh
for lib in kernel32 user32 gdi32 advapi32 shell32; do
  lld-link /lib /machine:x64 /def:${lib}.def /out:${lib}.lib
done
clang -target x86_64-windows-msvc -O1 -fno-stack-protector -ffreestanding -fno-builtin -Wall -Wextra -c ZippyZ.c -o ZippyZ.obj
clang -target x86_64-windows-msvc -c chkstk.s -o chkstk.obj
lld-link /machine:x64 /subsystem:windows /entry:mainCRTStartup /nodefaultlib /out:../ZippyZ.exe ZippyZ.obj chkstk.obj kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib
```

These commands use POSIX shell loop syntax and work on Linux/macOS or Windows environments with Bash and the LLVM tools installed; they are **not** a script to run when installing Zippy Z. On a Windows-only command prompt, invoke each `/lib` command separately, rather than copying the `for ... do ... done` line.

After building, keep the compiled `ZippyZ.exe` alongside the `Icons` directory.

**Future engineering work:** compile and run automated Win32 GUI tests on a Windows CI runner; verify rollback with third-party ZIP programmes and test real-world antivirus behaviour. The `src` folder uses explicitly declared Win32 structures and functions; contributors should compare these declarations with the official Windows SDK when maintaining the project.
