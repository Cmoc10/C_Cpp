#define DMALLOC_DISABLE 1
#include "dmalloc.hh"
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <cstdint> 

// Global structure to track comprehensive memory statistics
static struct {
    unsigned long long nactive = 0;         // # active allocations
    unsigned long long active_size = 0;     // # bytes in active allocations
    unsigned long long ntotal = 0;          // # total allocations
    unsigned long long total_size = 0;      // # bytes in total allocations
    unsigned long long nfail = 0;           // # failed allocation attempts
    unsigned long long fail_size = 0;       // # bytes in failed alloc attempts
    uintptr_t heap_min = UINTPTR_MAX;       // smallest allocated addr
    uintptr_t heap_max = 0;                 // largest allocated addr
} memory_stats;

// Metadata structure to store detailed allocation information
struct AllocMetadata {
    size_t size;               // Size of the allocated block
    const char* file;          // File where allocation occurred
    long line;                 // Line number of allocation
    bool is_active;            // Whether the block is currently allocated
};

// Unordered map to track allocations with their metadata
static std::unordered_map<uintptr_t, AllocMetadata> allocation_map;
constexpr uint32_t GUARD_VALUE = 0xDEADBEEF;

/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and 
 *      return a pointer to it 
 * 
 * @arg size_t sz : the amount of memory requested 
 * @arg const char *file : a string containing the filename from which dmalloc was called 
 * @arg long line : the line number from which dmalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    // Handle size 0 and potential integer overflow
    if (sz == 0 || sz > SIZE_MAX - sizeof(AllocMetadata) - 2 * sizeof(uint32_t)) {
        memory_stats.nfail++;
        memory_stats.fail_size += sz;
        return nullptr;
    }

    // Allocate memory with space for metadata, guard values, and payload
    size_t total_size = sz + sizeof(AllocMetadata) + 2 * sizeof(uint32_t);
    void* raw_ptr = base_malloc(total_size);

    // Check allocation success
    if (!raw_ptr) {
        memory_stats.nfail++;
        memory_stats.fail_size += sz;
        return nullptr;
    }

    // Initialize metadata
    AllocMetadata* metadata = reinterpret_cast<AllocMetadata*>(raw_ptr);
    *metadata = {sz, file, line, true};

    // Adjust payload pointer directly after metadata (same way as the first version)
    void* payload_ptr = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(raw_ptr) + sizeof(AllocMetadata));

    // Place the pre-guard value immediately before the payload
    uint32_t* pre_guard = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(payload_ptr) - sizeof(uint32_t));
    *pre_guard = GUARD_VALUE;

    // Place the post-guard value immediately after the payload
    uint32_t* post_guard = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(payload_ptr) + sz);
    *post_guard = GUARD_VALUE;

    // Update global memory statistics
    memory_stats.nactive++;
    memory_stats.active_size += sz;
    memory_stats.ntotal++;
    memory_stats.total_size += sz;

    // Update heap address range
    uintptr_t ptr_addr = reinterpret_cast<uintptr_t>(payload_ptr);
    memory_stats.heap_min = std::min(memory_stats.heap_min, ptr_addr);
    memory_stats.heap_max = std::max(memory_stats.heap_max, ptr_addr + sz);

    // Track allocation in map
    allocation_map[ptr_addr] = *metadata;

    return payload_ptr;
}


/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should 
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing. 
 * 
 * @arg void *ptr : a pointer to the heap 
 * @arg const char *file : a string containing the filename from which dfree was called 
 * @arg long line : the line number from which dfree was called 
 */
void dfree(void* ptr, const char* file, long line) {
    // Do nothing for null pointer
    if (!ptr) return;

    // Check if pointer is within the heap range
    uintptr_t ptr_addr = reinterpret_cast<uintptr_t>(ptr);
    if (ptr_addr < memory_stats.heap_min || ptr_addr > memory_stats.heap_max) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not in heap\n", file, line, ptr);
        abort();
    }

    // Find the metadata for the exact allocation
    auto it = allocation_map.find(ptr_addr);

    if (it == allocation_map.end()) {
        // Check if the pointer is inside an existing allocation
        for (const auto& entry : allocation_map) {
            const uintptr_t start_addr = entry.first;
            const size_t size = entry.second.size;
            if (ptr_addr > start_addr && ptr_addr < start_addr + size) {
                // Pointer is inside an allocated block
                fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n", file, line, ptr);
                fprintf(stderr, "%s:%ld: %p is %zu bytes inside a %zu byte region allocated here\n",
                        entry.second.file, entry.second.line, ptr,
                        ptr_addr - start_addr, size);
                abort();
            }
        }

        // Pointer is not found in any allocated block
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n", file, line, ptr);
        abort();
    }

    // Get metadata and size
    AllocMetadata& metadata = it->second;

    // Check for double free
    if (!metadata.is_active) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, double free\n", file, line, ptr);
        abort();
    }

    // Check for wild frees (if the pointer has been altered)
    if (ptr != reinterpret_cast<void*>(ptr_addr)) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, wild free detected\n", file, line, ptr);
        abort();
    }

    // Check pre-guard
    uint32_t* pre_guard = reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(ptr) - sizeof(uint32_t)
    );
    if (*pre_guard != GUARD_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: detected write before allocated block %p\n", file, line, ptr);
        abort();
    }

    // Check post-guard
    uint32_t* post_guard = reinterpret_cast<uint32_t*>(
        reinterpret_cast<char*>(ptr) + metadata.size
    );
    if (*post_guard != GUARD_VALUE) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: detected wild write during free of pointer %p\n", file, line, ptr);
        abort();
    }

    // Mark the allocation as freed
    metadata.is_active = false;

    // Update memory statistics
    memory_stats.nactive--;
    memory_stats.active_size -= metadata.size;

    // Free the entire block (including metadata and guards)
    void* metadata_ptr = reinterpret_cast<char*>(pre_guard) - sizeof(AllocMetadata);
    base_free(metadata_ptr);
}


/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb` 
 *      number of elements with wach element being `sz` bytes. The memory should be initialized 
 *      to zero  
 * 
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called 
 * @arg long line : the line number from which dcalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Protect against potential integer overflow
    if (nmemb != 0 && sz > SIZE_MAX / nmemb) {
        memory_stats.nfail++;
        memory_stats.fail_size += (nmemb * sz);
        return nullptr;
    }

    // Allocate memory
    void* ptr = dmalloc(nmemb * sz, file, line);
    
    // Zero out the memory if allocation succeeded
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics  
 * 
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    // Ensure stats is not null
    if (!stats) return;

    // Copy current memory statistics to the provided struct
    stats->nactive = memory_stats.nactive;
    stats->active_size = memory_stats.active_size;
    stats->ntotal = memory_stats.ntotal;
    stats->total_size = memory_stats.total_size;
    stats->nfail = memory_stats.nfail;
    stats->fail_size = memory_stats.fail_size;
    stats->heap_min = memory_stats.heap_min;
    stats->heap_max = memory_stats.heap_max;
}

/**
 * print_statistics()
 *      print the current memory statistics to stdout       
 */
void print_statistics() {
    dmalloc_stats stats;
    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**  
 * print_leak_report()
 *      Print a report of all currently-active allocated blocks of dynamic
 *      memory.
 */
void print_leak_report() {
    // Iterate through all allocations and print active blocks
    for (const auto& entry : allocation_map) {
        const AllocMetadata& metadata = entry.second;
        if (metadata.is_active) {
            printf("LEAK CHECK: %s:%ld: allocated object %p with size %zu\n", 
                   metadata.file, metadata.line, 
                   reinterpret_cast<void*>(entry.first), metadata.size);
        }
    }
}