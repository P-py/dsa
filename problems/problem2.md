### Advanced Mini-PBL: Swap Memory Manager

#### The Scenario 
You are developing a prototype of a Virtual Memory Manager. The system
has a Linked List that represents the "RAM Memory" (loaded pages).
Each node (Page) contains an ID, an access timestamp, and the next pointer.

The Problem (The Replacement Challenge):
RAM memory is full (limit of $N$ nodes). A new process requests the loading
of a new page. To make room for this new page, you must implement the simplified LRU (Least Recently Used) algorithm:
1. Find the page with the lowest timestamp (the oldest unused page) in the list.
2. Remove it from the list (freeing the space with free()).
3. Insert the new page at the beginning of the list.

#### Questions
1. What is the best data structure to be used on this case? Why?

>R: A singly-linked list. The three required operations map directly onto its natural strengths: insertion at the head is O(1) (just a pointer re-link, no shifting), removal of an arbitrary node is O(1) once the target and its predecessor are known (another pointer re-link plus a free()), and the LRU search is a straightforward O(n) linear scan. A fixed-size array would require shifting elements on every removal, making each replacement O(n) in data movement rather than pointer work. A doubly-linked list would also work but adds unnecessary memory overhead (a back-pointer per node) since we only traverse in one direction.

2. Develop the C implementation of the solution involving only the operations of the chosen data structure.

>R: See `problem2.c`. The core function `replaceLruPage` performs a single O(n) traversal, tracking the minimum-timestamp node (`lru`) and its predecessor (`lruPrev`) simultaneously. After the walk, it unlinks `lru` with a single pointer re-assignment and calls `free()` on it, then delegates to `insertAtBeginning` to prepend the new page in O(1).

3. The implemented solution is suitable for large quantities. For example, if the List had 1 million pages, would the solution be efficient? Why?

>R: No, it would not be efficient at scale. Every replacement requires a full O(n) scan to find the minimum timestamp, so with 1 million pages each eviction inspects all 1 000 000 nodes. If replacements happen frequently this becomes a bottleneck. An efficient production LRU cache couples the linked list with a min-heap (or a sorted set): the heap always exposes the minimum-timestamp page in O(1), and removal plus re-insertion cost only O(log n), bringing each replacement from O(n) down to O(log n) — roughly 20 operations instead of 1 000 000 for a million-page list.