#ifndef _MEMORYPOOL_H
#define _MEMORYPOOL_H

#include <stdlib.h>
#include <GLES3/gl3.h>

#define USE_DOUBLE_LINK         1

#if USE_DOUBLE_LINK
#define MINUNITSIZE             64
#define ADDR_ALIGN              8
#define SIZE_ALIGN              MINUNITSIZE

struct stApiMemoryChunk;
typedef struct stApiMemoryBlock
{
    size_t          count;
    size_t          start;
    stApiMemoryChunk*   pmem_chunk;
} stApiMemoryBlock;

// 可用的内存块结构体
typedef struct stApiMemoryChunk
{
    stApiMemoryBlock*   pfree_mem_addr;
    stApiMemoryChunk*   pre;
    stApiMemoryChunk*   next;
} stApiMemoryChunk;

// 内存池结构体
typedef struct stApiMemoryPool
{
    void                *memory;
    size_t              size;
    stApiMemoryBlock*   pmem_map; 
    stApiMemoryChunk*   pfree_mem_chunk;
    stApiMemoryChunk*   pfree_mem_chunk_pool;
    size_t              mem_used_size;              // 记录内存池中已经分配给用户的内存的大小
    size_t              mem_map_pool_count;         // 记录链表单元缓冲池中剩余的单元的个数，个数为0时不能分配单元给pfree_mem_chunk
    size_t              free_mem_chunk_count;       // 记录 pfree_mem_chunk链表中的单元个数
    size_t              mem_map_unit_count;         // 
    size_t              mem_block_count;            // 一个 mem_unit 大小为 MINUNITSIZE
} stApiMemoryPool, *pstMemoryPool;

/************************************************************************/
/* 生成内存池
 * pBuf: 给定的内存buffer起始地址
 * sBufSize: 给定的内存buffer大小
 * 返回生成的内存池指针
 */
/************************************************************************/
pstMemoryPool ApiCreateMemoryPool(void* pBuf, size_t sBufSize);

/************************************************************************/
/* 暂时没用 */
/************************************************************************/
void ApiReleaseMemoryPool(pstMemoryPool* ppMem) ;

/************************************************************************/
/* 从内存池中分配指定大小的内存 
 * pMem: 内存池 指针
 * sMemorySize: 要分配的内存大小
 * 成功时返回分配的内存起始地址，失败返回NULL
 */
/************************************************************************/
void* ApiGetMemory(size_t sMemorySize, pstMemoryPool pMem) ;

/************************************************************************/
/* 从内存池中释放申请到的内存
 * pMem：内存池指针
 * ptrMemoryBlock：申请到的内存起始地址
 */
/************************************************************************/
void ApiFreeMemory(void *ptrMemoryBlock, pstMemoryPool pMem) ;

#else

#define _MEMORYPOOL_H
#include <stdlib.h>
#define MINUNITSIZE 64
#define ADDR_ALIGN 8
#define SIZE_ALIGN MINUNITSIZE
#define MAXCHUNKSIZE 1024*1024*64
struct stApiMemoryChunk;
typedef struct stApiMemoryBlock
{
    size_t count;
    size_t start;
    stApiMemoryChunk* pmem_chunk;
}stApiMemoryBlock;
typedef struct stApiMemoryChunk
{
    size_t chunk_size;
    stApiMemoryBlock* pfree_mem_addr;
}stApiMemoryChunk;
typedef struct max_heap
{
    stApiMemoryChunk *heap;  
    size_t maxSize;   
    size_t currentSize;  
}max_heap;
typedef struct stApiMemoryPool
{
    void *memory;
    size_t size;
    stApiMemoryBlock* pmem_map; 
    max_heap heap;
    size_t mem_used_size; // 记录内存池中已经分配给用户的内存的大小
    size_t free_mem_chunk_count; // 记录 pfree_mem_chunk链表中的单元个数
    size_t mem_map_unit_count; // 
    size_t mem_block_count; // 一个 mem_unit 大小为 MINUNITSIZE
}stApiMemoryPool, *pstMemoryPool;
/************************************************************************/
/* 生成内存池
* pBuf: 给定的内存buffer起始地址
* sBufSize: 给定的内存buffer大小
* 返回生成的内存池指针
/************************************************************************/
pstMemoryPool ApiCreateMemoryPool(void* pBuf, size_t sBufSize);
/************************************************************************/
/* 暂时没用
/************************************************************************/
void ApiReleaseMemoryPool(pstMemoryPool* ppMem) ; 
/************************************************************************/
/* 从内存池中分配指定大小的内存 
* pMem: 内存池 指针
* sMemorySize: 要分配的内存大小
* 成功时返回分配的内存起始地址，失败返回NULL
/************************************************************************/
void* ApiGetMemory(size_t sMemorySize, pstMemoryPool pMem) ;
/************************************************************************/
/* 从内存池中释放申请到的内存
* pMem：内存池指针
* ptrMemoryBlock：申请到的内存起始地址
/************************************************************************/
void ApiFreeMemory(void *ptrMemoryBlock, pstMemoryPool pMem) ;

#endif

#define TCG_TEX_MEM_CACHE_SIZE          (20*1024*1024)

GLvoid      ApiCreateEventMemPool();
GLvoid      ApiDestroyEventMemPool();
GLvoid*     ApiAllocateEventMemory(GLuint size, GLboolean *pbUsePool);
GLvoid      ApiReleaseEventMemory(GLvoid *p, GLboolean bUsePool);

GLvoid*     ApiAllocateTexMemory(GLuint size, GLboolean *pbUsePool);
GLvoid      ApiReleaseTexMemory(GLvoid *p, GLboolean bUsePool);

GLvoid      ApiCreateTexMemPool();
GLvoid      ApiDestroyTexMemPool();
GLvoid*     ApiAllocateTexMemory(GLuint size);
GLvoid      ApiReleaseTexMemory(GLvoid *p);

#endif //_MEMORYPOOL_H
