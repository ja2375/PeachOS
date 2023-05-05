I am using macOS to build the project and everything seems to work fine. I was able to build the GCC cross compiler (even 
thought I gave it a different name, rather than i686-elf i set i386-elf. I figured out that is not technically correct 
after building it so feel free to change it in the makefile), install nasm, qemu, gdb and all the dependencies with the 
brew package manager.

I am in lecture 41 right now, but the code that is not working for me is the one on lecture 39.

I would appreciate if you could have a look at the code i have written, which is basically yours but whith some additions 
of my own, and let me know if you find anything that could potentially be wrong in the logic to read from disk, which is 
the one that seems to be broken.
I have tried using gdb to print the buf var that i try to read from disk, but it always says it's 512 zeroes, which i 
believe it's not what i am expecting. I have also tried printing its value with the print function, and it also prints 
nothing.

Thanks in advance!

Javi
