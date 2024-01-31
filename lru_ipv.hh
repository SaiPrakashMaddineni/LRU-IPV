#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_LRU_IPV_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_LRU_IPV_RP_HH__				// They prevent multiple inclusions of this header file in a single compilation unit.

#include <cstdint>									// They are standard library headers for fixed-width integer types
#include <memory>									// They are standard library headers for smart pointers
#include <vector>									// They are standard library headers for dynamic array handling

#include "mem/cache/replacement_policies/base.hh"					// This includes the base class for replacement policies in gem5.

struct LRUIPVRPParams;									// Taking the Parameters from the python program.

namespace ReplacementPolicy {
class LRUIPVRP : public Base								// This declares a new class LRUIPVRP which inherits from the Base replacement policy class.
{
  private:
    std::vector<int> IPV;								// A vector to store the Insertion Policy Vector (IPV) values.
    int numWays;									// An integer to store the number of ways in the cache set
    int count;										// A counter, possibly used to track the number of cache accesses or replacements.
    std::vector<int>* SharedStateInstance;						// A pointer to a vector, likely shared across multiple instances or representing some global state.


  protected:
    /** LRUIPV-specific implementation of replacement data. is a nested structure within LRUIPVRP, inheriting from ReplacementData. It represents the per-entry replacement data for the LRU-IPV policy.*/
    struct LRUIPVRPReplData : ReplacementData
    {
        /** Tick on which the entry was last touched.
         *Tick lastTouchTick;
         * Default constructor. Invalidate data.
         */
        int index;									//  This represents the index of the cache entry.
        std::shared_ptr<std::vector<int>> SharedState;					//  A shared pointer to a vector, possibly for shared state across cache entries.
        LRUIPVRPReplData(int index, std::shared_ptr<std::vector<int>> SharedState); 		// A constructor for LRUIPVRPReplData.
    };

  public:
    typedef LRUIPVRPParams Params;							// A typedef for easier reference to the parameters structure.
    LRUIPVRP(const Params &p);								// The constructor for the LRUIPVRP class.
    ~LRUIPVRP() = default;								// The default destructor.

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Sets its last touch tick as the starting tick.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data) const override;
                                                              

    /**
     * Touch an entry to update its replacement data.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const override;
                                                                     

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Sets its last touch tick as the current tick.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const override;
                                                                     

    /**
     * Find replacement victim using LRU timestamps.
     *
     * @param candidates Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

} // namespace ReplacementPolicy

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_LRU_IPV_RP_HH__
