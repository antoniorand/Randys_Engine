#include "memoryPool.hpp"

RandysEngine::Pool::Bucket::Bucket(std::size_t e_block_size, std::size_t e_block_count) : 
    blockSize{e_block_size}, blockCount{e_block_count}{
    
    //The number of blocks we are reserving
    const auto data_size = blockSize * blockCount;
    //The number of ledger size
    const auto ledger_size = 1 + ((blockCount-1)/8);
    m_data = std::make_unique<std::byte[]>(data_size);
    m_data = std::make_unique<std::byte[]>(ledger_size);
}

void* RandysEngine::Pool::Bucket::allocate(std::size_t bytes) noexcept{
    
    void* devolver = nullptr;
    
    //Calculate the number of blocks
    const auto n = 1 + ((bytes-1)/blockSize);
    
    /*const auto index = find_contiguous_blocks(n);
    if(index != blockCount){
        //set_blocks_in_use(index,n);
        devolver = m_data.get() + (index*blockSize);
    }*/

    return devolver;
}

void RandysEngine::Pool::Bucket::deallocate(void * ptr, std::size_t bytes) noexcept {

    const auto p = static_cast<const std::byte *>(ptr);
    const std::size_t dist = static_cast<std::size_t>(p - m_data.get());
    //Calculate block indez from pointer distance
    const auto index = dist/blockSize;
    //Calculate the required number of blocks;
    const auto n = 1 + ((bytes-1)/blockSize);
    //Update the ledger
    //set_blocks_in_free(index,n);
}