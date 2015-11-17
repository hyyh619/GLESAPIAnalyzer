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

// ���õ��ڴ��ṹ��
typedef struct stApiMemoryChunk
{
    stApiMemoryBlock*   pfree_mem_addr;
    stApiMemoryChunk*   pre;
    stApiMemoryChunk*   next;
} stApiMemoryChunk;

// �ڴ�ؽṹ��
typedef struct stApiMemoryPool
{
    void                *memory;
    size_t              size;
    stApiMemoryBlock*   pmem_map; 
    stApiMemoryChunk*   pfree_mem_chunk;
    stApiMemoryChunk*   pfree_mem_chunk_pool;
    size_t              mem_used_size;              // ��¼�ڴ�����Ѿ�������û����ڴ�Ĵ�С
    size_t              mem_map_pool_count;         // ��¼����Ԫ�������ʣ��ĵ�Ԫ�ĸ���������Ϊ0ʱ���ܷ��䵥Ԫ��pfree_mem_chunk
    size_t              free_mem_chunk_count;       // ��¼ pfree_mem_chunk�����еĵ�Ԫ����
    size_t              mem_map_unit_count;         // 
    size_t              mem_block_count;            // һ�� mem_unit ��СΪ MINUNITSIZE
} stApiMemoryPool, *pstMemoryPool;

/************************************************************************/
/* �����ڴ��
 * pBuf: �������ڴ�buffer��ʼ��ַ
 * sBufSize: �������ڴ�buffer��С
 * �������ɵ��ڴ��ָ��
 */
/************************************************************************/
pstMemoryPool ApiCreateMemoryPool(void* pBuf, size_t sBufSize);

/************************************************************************/
/* ��ʱû�� */
/************************************************************************/
void ApiReleaseMemoryPool(pstMemoryPool* ppMem) ;

/************************************************************************/
/* ���ڴ���з���ָ����С���ڴ� 
 * pMem: �ڴ�� ָ��
 * sMemorySize: Ҫ������ڴ��С
 * �ɹ�ʱ���ط�����ڴ���ʼ��ַ��ʧ�ܷ���NULL
 */
/************************************************************************/
void* ApiGetMemory(size_t sMemorySize, pstMemoryPool pMem) ;

/************************************************************************/
/* ���ڴ�����ͷ����뵽���ڴ�
 * pMem���ڴ��ָ��
 * ptrMemoryBlock�����뵽���ڴ���ʼ��ַ
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
    size_t mem_used_size; // ��¼�ڴ�����Ѿ�������û����ڴ�Ĵ�С
    size_t free_mem_chunk_count; // ��¼ pfree_mem_chunk�����еĵ�Ԫ����
    size_t mem_map_unit_count; // 
    size_t mem_block_count; // һ�� mem_unit ��СΪ MINUNITSIZE
}stApiMemoryPool, *pstMemoryPool;
/************************************************************************/
/* �����ڴ��
* pBuf: �������ڴ�buffer��ʼ��ַ
* sBufSize: �������ڴ�buffer��С
* �������ɵ��ڴ��ָ��
/************************************************************************/
pstMemoryPool ApiCreateMemoryPool(void* pBuf, size_t sBufSize);
/************************************************************************/
/* ��ʱû��
/************************************************************************/
void ApiReleaseMemoryPool(pstMemoryPool* ppMem) ; 
/************************************************************************/
/* ���ڴ���з���ָ����С���ڴ� 
* pMem: �ڴ�� ָ��
* sMemorySize: Ҫ������ڴ��С
* �ɹ�ʱ���ط�����ڴ���ʼ��ַ��ʧ�ܷ���NULL
/************************************************************************/
void* ApiGetMemory(size_t sMemorySize, pstMemoryPool pMem) ;
/************************************************************************/
/* ���ڴ�����ͷ����뵽���ڴ�
* pMem���ڴ��ָ��
* ptrMemoryBlock�����뵽���ڴ���ʼ��ַ
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
