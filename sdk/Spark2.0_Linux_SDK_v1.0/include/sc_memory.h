#ifndef SC_MEMORY_API_H
#define SC_MEMORY_API_H

#include "sc_type.h"

#define DEFAULT_MAX_WINDOWS 5
#define DEFAULT_MAX_TOKENS  9120

namespace SparkChain {

/**
 * @brief base momory api
 * 
 */
class SPARKCHAIN_API Memory {
public:
    virtual ~Memory();

    /**
     * @brief 
     * 
     * @param type 
     * @param maxCache : memory cache capacity
     * @return Memory* : inst
     */
    static Memory* WindowsMemory(int maxCache = DEFAULT_MAX_WINDOWS);

    /**
     * @brief 
     * 
     * @param maxCache 
     * @return Memory* 
     */
    static Memory* TokensMemory(int maxTokens = DEFAULT_MAX_TOKENS);

};

}

#endif