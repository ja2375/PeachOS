#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include <stddef.h>
#include <stdint.h>

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}
void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}


size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }

    return len;
}

void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

void println(const char* str)
{
    print(str);
    print("\n");
}

static struct paging_4gb_chunk* kernel_chunk = 0;

void kernel_main()
{
    terminal_initialize();
    println("Terminal initiated");

    // Initialize the heap
    kheap_init();
    println("Heap initialized");

    // Search and initialize the disks
    disk_search_and_init();
    print("Disk driver initialized");
    print("Read LBA0 from disk 0: ");
    char buf[512];
    read_disk_block(disk_get(0), 0, 1, &buf);
    print("READ: ");
    // TODO: Implement printf() to actually print [buf].
    print("\n");

    // Initialize the IDT
    idt_init();
    println("IDT initialized");

    //
    // START: PAGING
    //

    println("Setting up paging...");
    
    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    println("Loaded kernel chunk");

    // Switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    println("Switched page");

    // Enable paging
    enable_paging();
    println("Successfully enabled paging!");

    //
    // END: PAGING
    //

    // Enable interrupts in assembly only after
    // correctly initializing the IDT
    enable_interrupts();
    println("Enabled interrupts");
}