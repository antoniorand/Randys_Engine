#include "mem_Pool.hpp"


Randys_Engine::CMemoryPool::CMemoryPool(
    const std::size_t& sInitialMemoryPoolSize,
    const std::size_t& sMemoryChunkSize,
    const std::size_t& sMinimalMemorySizeToAllocate,
    bool bSetMemoryData) 
    : MemoryChunkSize{sMemoryChunkSize},
      MinimalMemoryToAllocate{sMinimalMemorySizeToAllocate},
      MemoryChunkCount{0},
      SetMemoryData{bSetMemoryData}
    {
        Randys_Engine::CMemoryPool::AllocateMemory(sInitialMemoryPoolSize);

    }
