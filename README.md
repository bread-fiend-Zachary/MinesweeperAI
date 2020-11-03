# MinesweeperAI
# Implemented MyAi.cpp and MyAi.hpp


# Rule Of Thumb

  Main algorithm implemented was a rule of thumb to beat any given world - I am given a starting point
in a world that is guaranteed to be safe, and with this information I checked the label(the number
of mines that are nearby when clicked) and compare it to the number of unmarked neighbors.
In my code, I checked whether the label = 0; if so, then all neighbors are safe to uncover.
If not, then this tile is pure RNG because I only have information for 1 tile which has a mine nearby.
I pick one at random and continue with the rule of thumb.

# Model Checking
  
  If the rule of thumb does not see a guaranteed safe tile nearby, then my algorithm implements model
checking. My AI stores the uncovered frontier nodes to a set and covered frontier nodes to a map, 
then applies the rule of thumb to uncovered frontier nodes. If the label we are at is equal to 
the number of unmarked neighbors, then every unmarked neighbor nearby is flagged for danger. 
Otherwise, we check for the lowest label and perform a random selection based on the lowest label
and what is inside our covered frontier nodes map. If my AI can not find a tile with the lowest 
occurance, then it performs a random selection on tiles inside the covered frontier nodes.
Else, if all else fails, and our map of covered frontier nodes is empty, then we perform a random 
selection and hope for the best, then repeat the algorithm from the rule of thumb.

# Time To Run
  It takes the algorithm ~2 minutes to run a set of 3000 worlds - 
  1000 easy, 
  1000 intermediate,
  1000 expert.
  
  World difficulty is based on the density of mines and the size of the world; the denser the mine count,
  the higher the difficulty.
