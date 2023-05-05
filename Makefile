all:
	nasm -f bin -o bin/boot.bin src/boot/boot.asm

clean:
	rm -rf bin/boot.bin