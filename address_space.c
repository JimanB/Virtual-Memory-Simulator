#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PTE_SIZE 4 // Standard size for a Page Table Entry (PTE) in bytes

// Prints memory size in the most appropriate units (B, KB, MB, GB)
void print_memory_size(unsigned long long bytes) {
    if (bytes >= (1ULL << 30))
        printf("size of the memory: %lluGB\n", bytes >> 30);
    else if (bytes >= (1ULL << 20))
        printf("size of the memory: %lluMB\n", bytes >> 20);
    else if (bytes >= (1ULL << 10))
        printf("size of the memory: %lluKB\n", bytes >> 10);
    else
        printf("size of the memory: %lluB\n", bytes);
}

// Prints the binary representation of a number with leading zeros
void print_binary(unsigned long long value, int bits) {
    for (int i = bits - 1; i >= 0; i--)
        printf("%d", (int)((value >> i) & 1));
}

// Checks if a number is a power of two
int is_power_of_two(int n) {
    return (n > 0 && (n & (n - 1)) == 0);
}

// Returns the base-2 logarithm of a number as an integer
int log2_int(unsigned long long x) {
    return x == 0 ? 0 : (int)(log2(x));
}

int main(int argc, char *argv[]) {
    // Expect exactly 3 command-line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <page_table_type 1|2> <address_bits 8-63> <page_size_KB>\n", argv[0]);
        return 1;
    }

    // Parse and validate command-line arguments
    int type = atoi(argv[1]);              // 1 = single-level, 2 = two-level page table
    int address_bits = atoi(argv[2]);      // Total number of bits in the virtual address
    int page_size_kb = atoi(argv[3]);      // Page size in kilobytes

    if (type != 1 && type != 2) {
        fprintf(stderr, "Error: page table type must be 1 or 2.\n");
        return 1;
    }
    if (address_bits < 8 || address_bits > 63) {
        fprintf(stderr, "Error: address bits must be between 8 and 63.\n");
        return 1;
    }
    if (!is_power_of_two(page_size_kb) || page_size_kb < 1 || page_size_kb > 512) {
        fprintf(stderr, "Error: page size must be a power of two between 1 and 512 KB.\n");
        return 1;
    }

    // Calculate total memory size and page size in bytes
    unsigned long long mem_size = 1ULL << address_bits;
    unsigned long long page_size_bytes = page_size_kb * 1024ULL;

    // Sanity check: page size must fit into the memory address space
    if (mem_size < page_size_bytes) {
        fprintf(stderr, "Error: address space is too small for the specified page size.\n");
        return 1;
    }

    // Calculate bit divisions
    int offset_bits = log2_int(page_size_bytes);       // Bits used for page offset
    int vpn_bits = address_bits - offset_bits;         // Bits used for Virtual Page Number (VPN)
    unsigned long long total_pages = 1ULL << vpn_bits; // Total number of virtual pages
    unsigned long long page_table_size = total_pages * PTE_SIZE; // Total size of the page table in bytes

    // Print out the main memory configuration
    print_memory_size(mem_size);
    printf("total number of pages: %llu\n", total_pages);
    printf("total number of PTE (page table entries): %llu\n", total_pages);
    printf("size of page table: %llu\n", page_table_size);
    printf("number of bits for VPN: %d\n", vpn_bits);
    printf("number of bits for page offset: %d\n", offset_bits);

    // Variables for two-level page table structure
    int ptes_per_page = 0, pages_in_table = 0, pd_bits = 0, pt_bits = 0;

    if (type == 2) {
        // Compute the layout for two-level page tables
        ptes_per_page = page_size_bytes / PTE_SIZE;   // Number of PTEs that can fit in one page
        pt_bits = log2_int(ptes_per_page);            // Bits for indexing into a page table
        pd_bits = vpn_bits - pt_bits;                 // Remaining bits used for the page directory
        pages_in_table = 1 << pd_bits;

        // Display the two-level page table breakdown
        printf("number of PTE in a page of page table: %d\n", ptes_per_page);
        printf("number of pages in a page table: %d\n", pages_in_table);
        printf("number of bits for page directory index: %d\n", pd_bits);
        printf("number of bits for page table index: %d\n", pt_bits);
    }

    // Start virtual address translation loop
    while (1) {
        unsigned long long vaddr;
        printf("decimal virtual address: ");

        // Exit if input fails (e.g., EOF or non-numeric)
        if (scanf("%llu", &vaddr) != 1) break;

        // Reject address if it exceeds virtual memory space
        if (vaddr >= mem_size) {
            printf("Error: address exceeds memory bounds of %llu bytes.\n", mem_size);
            continue;
        }

        // Extract offset and VPN
        unsigned long long offset = vaddr & ((1ULL << offset_bits) - 1);
        unsigned long long vpn = vaddr >> offset_bits;

        // Output VPN and offset in decimal
        printf("VPN of the address in decimal: %llu\n", vpn);
        printf("page offset of the address in decimal: %llu\n", offset);

        if (type == 2) {
            // Extract page directory and page table indices
            unsigned long long pt_index = vpn & ((1ULL << pt_bits) - 1);
            unsigned long long pd_index = vpn >> pt_bits;

            // Output directory and table indices in decimal
            printf("page directory index in decimal: %llu\n", pd_index);
            printf("page table index in decimal: %llu\n", pt_index);

            // Output full binary breakdown
            printf("the input address in binary: ");
            print_binary(vaddr, address_bits);
            printf("\nVPN of the address in binary: ");
            print_binary(vpn, vpn_bits);
            printf("\npage offset of the address in binary: ");
            print_binary(offset, offset_bits);
            printf("\npage directory index in binary: ");
            print_binary(pd_index, pd_bits);
            printf("\npage table index in binary: ");
            print_binary(pt_index, pt_bits);
            printf("\n");
        } else {
            // Single-level page table: only VPN and offset
            printf("the input address in binary: ");
            print_binary(vaddr, address_bits);
            printf("\nVPN of the address in binary: ");
            print_binary(vpn, vpn_bits);
            printf("\npage offset of the address in binary: ");
            print_binary(offset, offset_bits);
            printf("\n");
        }
    }

    return 0;
}
