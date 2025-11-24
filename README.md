# Address Space Simulator

A C program that simulates virtual address translation using single-level and two-level page tables in a Unix environment.

## Overview

This program analyzes memory address spaces and performs virtual-to-physical address translation based on different page table configurations. It supports both single-level linear page tables and two-level tree-structured page tables.

## Features

- **Memory Configuration Analysis**: Calculates memory size, page counts, and bit allocations
- **Address Translation**: Converts virtual addresses to page numbers and offsets
- **Binary Representation**: Displays addresses and components in binary format
- **Two-Level Page Table Support**: Handles page directory and page table indexing
- **Input Validation**: Comprehensive error checking for command-line arguments and user input

## Requirements

- Unix-like operating system
- C compiler (gcc recommended)
- Standard C libraries: `stdio.h`, `stdlib.h`, `math.h`

## Compilation

```bash
gcc -o address_space address_space.c -lm
```

## Usage

```bash
./address_space <page_table_type> <address_bits> <page_size_KB>
```

### Command Line Arguments

- `page_table_type`: 
  - `1` for single-level page table
  - `2` for two-level page table
- `address_bits`: Total bits in virtual address (8-63)
- `page_size_KB`: Page size in kilobytes (1, 2, 4, 8, 16, 32, 64, 128, 256, 512)

### Examples

```bash
# Two-level page table with 32-bit addresses and 8KB pages
./address_space 2 32 8

# Single-level page table with 16-bit addresses and 4KB pages  
./address_space 1 16 4
```

## Program Output

### Initial Configuration Output
- Memory size (in appropriate units: B, KB, MB, GB)
- Total number of pages
- Total number of page table entries (PTEs)
- Page table size in bytes
- Number of bits for VPN and page offset
- For two-level tables: PTE count per page, page counts, directory and table index bits

### Interactive Address Translation
- Prompts for decimal virtual addresses
- Outputs VPN, page offset, and indices in decimal and binary formats
- Validates addresses against memory bounds

## Sample Session

```
$ ./address_space 2 32 8
size of the memory: 4GB
total number of pages: 524288
total number of PTE (page table entries): 524288
size of page table: 2097152
number of bits for VPN: 19
number of bits for page offset: 13
number of PTE in a page of page table: 2048
number of pages in a page table: 256
number of bits for page directory index: 8
number of bits for page table index: 11

decimal virtual address: 1234567890
VPN of the address in decimal: 150704
page offset of the address in decimal: 722
page directory index in decimal: 73
page table index in decimal: 1200
the input address in binary: 0100100110010110000001011010010
VPN of the address in binary: 0100100110010110000
page offset of the address in binary: 0001011010010
page directory index in binary: 01001001
page table index in binary: 10010110000
```

## Error Handling

The program validates:
- Command line argument ranges and consistency
- Page size as power of two within valid range
- Virtual addresses within memory bounds
- Proper page table configuration feasibility

## Implementation Details

- Uses bit manipulation for address decomposition
- Supports large address spaces (up to 63-bit addresses)
- Efficient binary representation with leading zeros
- Comprehensive input validation with descriptive error messages

## Files

- `address_space.c` - Main program implementation
- `address_space_improved.c` - Alternative implementation
