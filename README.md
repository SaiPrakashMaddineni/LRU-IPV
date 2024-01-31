The least-recently-used (LRU) replacement policy is moti-
vated by the observation that, if a block has not been used
recently, then it is unlikely to be used again in the near fu-
ture. This intuition is reasonable, but leaves a considerable
amount of room for improvement.

There has been much previous work on last-level cache
replacement. From this work, we have ample evidence that
we can improve significantly over LRU, but at a cost. Even
the LRU policy itself is expensive: in a 16-way set associa-
tive cache, LRU requires four bits per cache block. Other
proposed algorithms improve on this cost.

last-level cache replacement al-
gorithm with very low overhead that delivers high perfor-
mance. The algorithm uses less than one bit per cache block;
on a 16-way set associative cache, it uses 15 bits per set or
less than 0.94 bits per block. It extends tree-based Pseu-
doLRU, a policy with performance similar to LRU. We
enhance it to deliver performance comparable with state-of-
the-art replacement algorithms.
