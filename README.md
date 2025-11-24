# Address Space Simulator

## Overview

This project implements a C program that simulates virtual address translation using single-level and two-level page tables in a Unix environment. The program analyzes memory address spaces and performs virtual-to-physical address translation based on different page table configurations.

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

## Installation & Usage

### 1. Compilation

```bash
gcc -o address_space address_space.c -lm
```

### 2. Usage

```bash
./address_space <page_table_type> <address_bits> <page_size_KB>
```

### Command Line Arguments

- `page_table_type`: 
  - `1` for single-level page table
  - `2` for two-level page table
- `address_bits`: Total bits in virtual address (8-63)
- `page_size_KB`: Page size in kilobytes (1, 2, 4, 8, 16, 32, 64, 128, 256, 512)

### 3. Examples

```bash
# Two-level page table with 32-bit addresses and 8KB pages
./address_space 2 32 8

# Single-level page table with 16-bit addresses and 4KB pages  
./address_space 1 16 4
```

### 4. Sample Output

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

## Implementation Details

### Program Structure

The program consists of two main components:

1. **Configuration Analysis**: Processes command-line arguments and calculates memory layout
2. **Interactive Translation**: Prompts for virtual addresses and displays translation results

### Key Functions

- `print_memory_size()`: Displays memory size in appropriate units (B, KB, MB, GB)
- `print_binary()`: Outputs binary representation with leading zeros
- `is_power_of_two()`: Validates page size argument
- `log2_int()`: Calculates base-2 logarithm for bit calculations

### Address Translation Process

1. **Input Validation**: Checks argument consistency and ranges
2. **Memory Layout Calculation**: Determines VPN bits, offset bits, and page counts
3. **Two-Level Structure**: For type 2, calculates directory and table indices
4. **Interactive Loop**: Processes user input addresses and displays translations

## Technical Details

### Memory Calculations

- **Total Memory**: `2^address_bits` bytes
- **Page Size**: `page_size_KB * 1024` bytes
- **Offset Bits**: `log2(page_size_bytes)`
- **VPN Bits**: `address_bits - offset_bits`
- **Total Pages**: `2^vpn_bits`

### Two-Level Page Table Structure

- **PTEs per Page**: `page_size_bytes / PTE_SIZE` (where PTE_SIZE = 4 bytes)
- **Page Table Index Bits**: `log2(ptes_per_page)`
- **Page Directory Index Bits**: `vpn_bits - pt_bits`
- **Pages in Page Table**: `2^pd_bits`

### Binary Format Display

All binary outputs include leading zeros to maintain proper bit width representation, ensuring accurate visualization of the address structure.

## Error Handling

The program validates:

- Command line argument ranges and consistency
- Page size as power of two within valid range
- Virtual addresses within memory bounds
- Proper page table configuration feasibility

Error messages provide clear explanations for invalid inputs:

```
Error: address space is too small for the specified page size.
Error: address exceeds memory bounds of 1024 bytes.
```

## Files Included

- `address_space.c` - Main program implementation
- `address_space_improved.c` - Alternative implementation
- `a5.pdf` - Assignment specification document

## Limitations

- Maximum address space limited to 63 bits
- Page sizes restricted to specific power-of-two values (1-512 KB)
- Single PTE size fixed at 4 bytes
- No support for physical address translation or TLB simulation

## Testing

Test the implementation by:

1. Running with various valid argument combinations
2. Testing boundary cases (minimum/maximum address bits)
3. Verifying error handling with invalid inputs
4. Comparing binary outputs with manual calculations
5. Testing both single-level and two-level page table configurations

## Notes

- The implementation follows the exact specification from the assignment document
- All calculations use unsigned 64-bit integers to handle large address spaces
- Binary output formatting ensures proper alignment and readability
- The program maintains interactive operation until user terminates input
