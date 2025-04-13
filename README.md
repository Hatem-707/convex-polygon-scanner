# Convex polygons void set  

## Our goal of this project will be to provide a dataset of the boundary of sets that doesn't have convex polygons  

## Performance evolution accross the project duration  

week 5:  
**Baseline performance**: Used convex hulls and random points generator to find sets.
Maximum possible set size is 12 at around 1 minutes on 14 threads on a ryzen 5800h.

week 6:   
**Memory optimization**: Reduced memory usage from 2 to 3 GBs to being N/A in ubuntu system monitor.
**Performance improvement**: Added new breakpoints to the algorithm that gave us 31x the performance that scales with target set size.
Maximum set size is 13 with average time of generation of 20 seconds.

week 7:  
**Major breakthrough**: Implemented iterative construction algorithm. We managed to trivialize the probabilistic factor of the problem growth.
In the previous optimization the algorithm a 16 point set would have taken **years to decades**. We traded the certainty for an algorithm that terminates in around 20 seconds, on the same hardware, and has a success rate of around 28%.
