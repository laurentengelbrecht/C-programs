#include "memalloc.h"



mem_opt_t   mem_opt        = {DEFAULT_INIT_SIZE_RATIO,
                              DEFAULT_MAX_HEAP_NODE_SIZE};

list_t      heap_list      = {NULL, NULL, 0},
            mmap_list      = {NULL, NULL, 0};

free_list_t free_heap_list = {NULL, NULL, 0, 0, 0};


void *memalloc(unsigned int size)
{
        void *addr;

        if(size == 0)
                return EBAD_SIZE;

        if (size <= mem_opt.max_heap_node_size)
        {
                addr = heap_alloc(size);

        } else {
                addr = mmap_alloc(size);
        }

        return addr;
}


void *clear_alloc  (unsigned int size)
{
        void *addr;

        if (size == 0)
                return EBAD_SIZE;

        /* Simplicity is our best friend. */
        addr = memalloc(size);
        mem_clear_64(addr, size);

        return addr;
}


int   memfree      (void *to_free)
{
      bool is_last;
      node_t *real_addr = (node_t*) to_free - 1;

      if (real_addr->size <= mem_opt.max_heap_node_size)
      {
              if (check_if_node(&heap_list, real_addr))
              {
                      is_last = (real_addr == heap_list.head);

                      unlink_node(&heap_list, real_addr);

                      if (is_last)
                      { /* If our block is the last in the heap, we may free it
                           from memory. */
                              sbrk( - (real_addr->size + sizeof(node_t)));
                      } else {

                              link_node_in_order(&free_heap_list, real_addr);
                      }

              } else {
                      return EBAD_ADDR;
              }

      } else if (real_addr->size > mem_opt.max_heap_node_size) {

              if (check_if_node(&mmap_list, real_addr))
              {
                      unlink_node(&mmap_list, real_addr);
                      /* Remember we use the base address of the node. */
                      munmap(to_free, real_addr->size + sizeof(node_t));
              } else {
                      return EBAD_ADDR;
              }
      }

      return 0;
}


void *rememalloc   (void *to_realloc, unsigned int size)
{
        node_t *real_addr = (node_t*) to_realloc - 1;
        node_t *new_node;
        void   *new_alloc;

        if (real_addr->size >= size)
        { /* If the node already has a sufficiently large size to fit the
            requested size, the given address is returned. */
                return to_realloc;
        }

        if (real_addr == heap_list.head && size < mem_opt.max_heap_node_size)
        { /* If the node is last in the heap, we may simply expand it's size. */
                        sbrk(size - real_addr->size);
                        real_addr->size = size;

                        return to_realloc;
        }

        if (check_if_node(&heap_list, real_addr) ||
            check_if_node(&mmap_list, real_addr)) {

            new_alloc = memalloc(size);
            mem_copy_64(new_alloc, real_addr->data, real_addr->size);

            memfree(to_realloc);

        } else {
                return NULL;
        }

        return new_alloc;
}


node_t *heap_alloc (unsigned int size)
{
        node_t *node;

        if (size <= free_heap_list.largest_block_size)
        { /* Node recycling. */
                node = get_node_by_size(&free_heap_list, size);
                unlink_free_node(&free_heap_list, node);
                link_node(&heap_list, node);
        } else { /* Allocating a new node. */
                node = sbrk(sizeof(node_t) + (size * mem_opt.size_ratio));
                if (node == BRK_FAIL)
                        return BRK_FAIL;

                node->size = size * mem_opt.size_ratio;
                link_node(&heap_list, node);
        }

        /* Returns the address of the data. */
        node++;
        return node;
}


node_t *mmap_alloc (unsigned int size)
{ /* There is no recycling when it deals with memory mapped nodes, they are
     allocated or removed, which is the point of memory mapping, in order to
     reduce internal fragmentation. */
        node_t *node;
        int min_page_size   = sysconf(_SC_PAGESIZE);
        int real_alloc_size = (((sizeof(node_t) + size) * mem_opt.size_ratio) /
                                 min_page_size) * min_page_size;

        real_alloc_size    += (!real_alloc_size) ? (real_alloc_size) :
                                min_page_size;

        node = mmap(NULL, real_alloc_size, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, real_alloc_size);

        if (node == (node_t*) -1)
                return MMAP_FAIL;

        node->size = real_alloc_size - sizeof(node_t);
        link_node(&mmap_list, node);

        node++;
        return node;
}


void  mem_clear_64 (void *to_clear, unsigned int size)
{
        uint64_t *qword_ptr;
        char     *byte_ptr;

        unsigned int quotient;
        unsigned int remainder;

        if (size < 128)
        { /* We don't mind making a 64-bit copy if the content is small due to
             quotient/ remainder calculation time. */
                byte_ptr = (char*) to_clear;

                do {
                        *++byte_ptr = NULL_BYTE;

                } while(size--);

        } else {
                qword_ptr = (uint64_t*) to_clear;

                quotient  = size / QWAD_WORD_SIZE;
                remainder = size % QWAD_WORD_SIZE;

                byte_ptr = (char*) (uint64_t) to_clear + (size - remainder);

                do {
                        *++qword_ptr = NULL_QWORD;

                } while(quotient--);


                do {
                        *++byte_ptr = NULL_BYTE;

                } while(remainder--);
        }

        return;
}


void  mem_copy_64  (void *dst, void *src, unsigned int size)
{
        uint64_t *qword_dst_ptr,
                 *qword_src_ptr;

        char     *byte_dst_ptr,
                 *byte_src_ptr;

        unsigned int quotient;
        unsigned int remainder;

        if (size <= 128)
        {
                byte_dst_ptr = (char*) dst;
                byte_src_ptr = (char*) src;

                do {
                        *++byte_dst_ptr = *++byte_src_ptr;

                } while(size--);

        } else {
                qword_dst_ptr = (uint64_t*) dst;
                qword_src_ptr = (uint64_t*) src;

                quotient  = size / QWAD_WORD_SIZE;
                remainder = size % QWAD_WORD_SIZE;

                byte_dst_ptr = (char*) (uint64_t) dst + (size - remainder);
                byte_src_ptr = (char*) (uint64_t) src + (size - remainder);

                do {
                        *++qword_dst_ptr = *++qword_src_ptr;

                } while(quotient--);


                do {
                        *++byte_dst_ptr = *++byte_src_ptr;

                } while(remainder--);
        }
}
