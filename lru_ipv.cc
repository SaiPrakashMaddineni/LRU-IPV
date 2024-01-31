#include "mem/cache/replacement_policies/lru_ipv.hh"							// Including the header file of the program
#include <cmath>											// This is an inclusion of the C++ standard library header <cmath>, which provides functions for mathematical operations such as power, square root, trigonometric operations, etc.
#include "base/intmath.hh"										// This line includes the intmath.hh header from the base directory of gem5. This file likely contains integer math utilities that are commonly used throughout the gem5 codebase.
#include "base/logging.hh"										// This includes the logging header from gem5's base directory. The logging utilities are used for recording messages, warnings, errors, and other types of logs.
#include "params/LRUIPVRP.hh"										// This line includes a header file that likely contains the definition of parameters (or settings) specific to the LRU-IPV replacement policy.
// Namespace for replacement policies
namespace ReplacementPolicy {

LRUIPVRP::LRUIPVRPReplData::LRUIPVRPReplData(
    int index, std::shared_ptr<std::vector<int>> SharedState)
  : index(index), SharedState(SharedState)
{
}
// LRUIPVRP constructor initializes the replacement policy parameters in a Vector format
LRUIPVRP::LRUIPVRP(const Params &p2)
  : Base(p2), numWays(p2.numWays), count(0), SharedStateInstance(nullptr)
{
    IPV = std::vector<int>({0, 0, 1, 0, 3, 0, 1, 0, 1, 7, 5, 1, 0, 0, 1, 11, 14});
}
// Invalidate a cache entry
void LRUIPVRP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    
    std::shared_ptr<LRUIPVRPReplData> vec_data =
        std::static_pointer_cast<LRUIPVRPReplData>(replacement_data);							// This line casts the replacement_data from its base class ReplacementData to the derived class LRUIPVRPReplData.
    std::vector<int>* vec = vec_data->SharedState.get();								// This line gets the index (index) from vec_data, which represents the position of the current cache entry in the shared state vector.

    int vec_index = vec_data->index;
    
    vec->at(vec_index) = numWays + 1;											// By setting the state to numWays + 1, the function effectively marks this cache entry as invalid or least recently used, pushing it beyond the normal range of valid states. 

}
// Update the state of a cache entry on access
void LRUIPVRP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    

    std::shared_ptr<LRUIPVRPReplData> vec_data =
        std::static_pointer_cast<LRUIPVRPReplData>(replacement_data);							// This line casts the replacement_data from its base class ReplacementData to the derived class LRUIPVRPReplData.
    std::vector<int>* vec = vec_data->SharedState.get();								// This line gets the index (index) from vec_data, which represents the position of the current cache entry in the shared state vector.

	
    int vec_index = vec_data->index;											// Assigning Vector to index
    int previousState = vec->at(vec_index);										// It retrieves the current state of the cache entry and determines the new state based on the IPV
    int change = IPV[previousState];


    if(previousState != change) {
        for (int i = 0; i < numWays; i++) {										// If the previous state is different from the new state (if(previousState != change)), the function iterates over all entries in the shared state vector.
            
            if (vec->at(i) >= change && vec->at(i) < previousState) {
                
                vec->at(i)++;												// For each entry, if its state is between the new state and the previous state the state of that entry is incremented (vec->at(i)++). This step promotes other cache entries closer to eviction.
            }
        }
    }
    vec->at(vec_index) = change;											// Changing vector index
    

}
//  Reset the state of a cache entry
void LRUIPVRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    
    std::shared_ptr<LRUIPVRPReplData> vec_data = std::static_pointer_cast<LRUIPVRPReplData>(replacement_data);
    std::vector<int>* vec = vec_data->SharedState.get();
    
	
    for (int i = 0; i < numWays; i++) {
        if (vec->at(i) >= IPV[numWays]) vec->at(i)++;									// This action shifts the state of those entries, making them more likely to be chosen as victims for replacement if they are not accessed again soon.
    }
	
    vec->at(vec_data->index) = IPV[numWays];										// This essentially resets the state of the targeted cache entry.
    
}
// Select a cache entry to be replaced
ReplaceableEntry* LRUIPVRP::getVictim(const ReplacementCandidates& candidates) const
{
    

    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        std::shared_ptr<LRUIPVRPReplData> vec_data =
            std::static_pointer_cast<LRUIPVRPReplData>(candidate->replacementData);
        std::vector<int>* vec = vec_data->SharedState.get();
        if (vec->at(vec_data->index) == numWays - 1) {
            
            victim = candidate;												// The condition numWays - 1 implies that the function is looking for the entry that is the least recently used, as per the LRU logic.
        }
    }
    return victim;													// The function returns the selected victim for eviction.
}


// Instantiate a new cache entry
std::shared_ptr<ReplacementData> LRUIPVRP::instantiateEntry()
{
	
    if (count % numWays == 0) {
        SharedStateInstance = new std::vector<int>(numWays);								// This line calculates the index for the new cache entry within the set.
        for (int i = 0; i < numWays; i++) SharedStateInstance->at(i) = i;						// The new entry is initialized with the calculated index (ind) and a shared pointer to the SharedStateInstance.
    }
    int ind = count % numWays;												// It uses the modulo operation to ensure that the index is within the bounds of the number of ways (slots) in the cache.
    
    LRUIPVRPReplData* Data = new LRUIPVRPReplData(ind,
                            std::shared_ptr<std::vector<int>>(SharedStateInstance));

    count++;														// This variable keeps track of the total number of cache entries created so far and ensures that the correct index is assigned to each new entry.
    
    return std::shared_ptr<ReplacementData>(Data);
}

}
