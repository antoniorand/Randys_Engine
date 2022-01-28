#pragma once

#include <cstddef>
#include <memory>

//source https://www.codeproject.com/Articles/15527/C-Memory-Pool
namespace Randys_Engine{
    
    struct IMemoryBlock{};


    typedef struct SMemoryChunk{

        std::unique_ptr<std::byte> Data;
        std::unique_ptr<SMemoryChunk> next;
        std::size_t DataSize;
        std::size_t UsedSize;
        bool IsAllocationChunk;

    } SMemoryChunk;

    
    typedef struct CMemoryPool{

        CMemoryPool(
            const std::size_t& sInitialMemoryPoolSize,
            const std::size_t& sMemoryChunkSize,
            const std::size_t& sMinimalMemorySizeToAllocate,
            bool bSetMemoryData);

        bool AllocateMemory(const std::size_t& sMemorySize)

        private:
            std::unique_ptr<std::byte> FirstChunk;
            std::unique_ptr<std::byte> LastChunk;
            std::unique_ptr<std::byte> CursorChunk;

            std::size_t TotalMemoryPoolSize{0};
            std::size_t UsedMemoryPoolSize{0};
            std::size_t FreeMemoryPoolSize{0};
            std::size_t MemoryChunkSize;
            
            unsigned int MemoryChunkCount{0};
            unsigned int ObjectCount{0};
            const unsigned int MinimalMemoryToAllocate;
            bool SetMemoryData

    } CMemoryPool;

    struct MemoryPool{};
    
}