#include "memoryPool.hpp"
#define CHECK_BIT_REVERSED(var,pos) (((uint8_t)var) & (0x80 >> pos))
#define SET_BIT_REVERSED  (var,pos) (((uint8_t)var) | (0x80 >> pos))  

RandysEngine::Pool::Bucket::Bucket(std::size_t e_block_size, std::size_t e_block_count) : 
    blockSize{e_block_size}, blockCount{e_block_count}{

    //The number of blocks we are reserving
    const auto data_size = blockSize * blockCount;
    //The number of ledger size
    const auto ledger_size = 1 + ((blockCount-1)/8);
    m_data = std::make_unique<std::byte[]>(data_size);
    assert(m_data != nullptr);
    m_ledger = std::make_unique<std::byte[]>(ledger_size);
    assert(m_ledger != nullptr);
    memset(m_data.get(),0,data_size);
    memset(m_ledger.get(),0,ledger_size);
}

bool RandysEngine::Pool::Bucket::belongs(void * ptr) const noexcept{

    bool devolver = false;

    if(ptr > m_data.get() && ptr < m_data.get()+(blockSize*blockCount))
        devolver = true;

    return (devolver);

}

std::size_t RandysEngine::Pool::Bucket::find_contiguous_blocks(std::size_t n) const noexcept{
    //for every block in the ledger
    std::size_t devolver = blockCount;
    for(std::size_t i = 0; i < (blockSize-n+1);i++){
        //We have initial 0 blocks contiguous
        std::size_t countingBlocks = 0;
        //for every contiguous block
        for(std::size_t j = 0; j < n; j++){
            //We need the byte that is in the position of the block divide by 8
            //Example: we need the 13th block which is located at the second byte,
            // 13/8 is 1, therefore the second byte
            std::byte byteToRetrieve = m_ledger.operator[](i/8);

            //Now we have to get the specific bit, we use our macro CHECK_BIT_REVERSED
            if(!CHECK_BIT_REVERSED(byteToRetrieve,(i+j)%8))
                countingBlocks++;
        }
        //We need to check if we have enough contiguous blocks
        //Se we use the flag
        if(countingBlocks == n){
            //i is the index
            devolver = i;
            break;
        }
    }
    return devolver;
};

void RandysEngine::Pool::Bucket::set_blocks_in_use(std::size_t index,std::size_t n) noexcept{
    //Before calling this function, we are 100% sure that the blocks
    //we are about to set are ***FREE***
    //don't worry about any error to much tbh
    for(std::size_t j = 0; j < n; j++){
        //We need to get the byte by the index
        //Also we need the REFERENCE not a copy, dumbass
        std::byte& byteToRetrieve = m_ledger.operator[]((index+j)/8);
        //Afterwards we have to set the specific bit of the index block
        byteToRetrieve = (std::byte)(((uint8_t)byteToRetrieve) | (0x80 >> (index+j)%8));
    }

}

void* RandysEngine::Pool::Bucket::allocate(std::size_t bytes) noexcept{
    
    void* devolver = nullptr;
    
    //Calculate the number of blocks
    const auto n = 1 + ((bytes-1)/blockSize);
    
    const auto index = find_contiguous_blocks(n);
    if(index != blockCount){
        set_blocks_in_use(index,n);
        devolver = m_data.get() + (index*blockSize);
    }

    return devolver;
}

void RandysEngine::Pool::Bucket::set_blocks_in_free(std::size_t index,std::size_t n) noexcept{
    //Before calling this function, we are 100% sure that the blocks
    //we are about to set are ***USED***
    //don't worry about any error to much tbh
    for(std::size_t j = 0; j < n; j++){
        //We need to get the byte by the index
        //Also we need the REFERENCE not a copy, dumbass
        std::byte& byteToRetrieve = m_ledger.operator[]((index+j)/8);
        //Afterwards we have to set the specific bit of the index block
        byteToRetrieve = (std::byte)(((uint8_t)byteToRetrieve) & ~(0x80 >> (index+j)%8));
    }

}

void RandysEngine::Pool::Bucket::deallocate(void * ptr, std::size_t bytes) noexcept {

    const auto p = static_cast<const std::byte *>(ptr);
    const std::size_t dist = static_cast<std::size_t>(p - m_data.get());
    //Calculate block index from pointer distance
    const auto index = dist/blockSize;
    //Calculate the required number of blocks;
    const auto n = 1 + ((bytes-1)/blockSize);
    //Update the ledger
    set_blocks_in_free(index,n);
}
