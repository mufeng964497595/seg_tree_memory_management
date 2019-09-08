#ifndef __SEG_TREE_MEM_MGR_CONST_H__
#define __SEG_TREE_MEM_MGR_CONST_H__

// 线段树内存管理 常量定义
namespace mem
{
const uint32_t BLOCK_SIZE = 8;	// 内存每块大小为 8 byte
const uint32_t ROOT_ID = 0;	// 线段树根节点id

// 延时标记含义
const uint8_t LAZY_EMPTY = 0;	// 无lazy标记
const uint8_t LAZY_USED = 1;	// 内存占用标记
const uint8_t LAZY_UNUSED = 2;	// 内存未使用标记

// 线段树左子树节点id
#define LEFT_NODE(node) ((node << 1) + 1)
// 线段树右子树节点id
#define RIGHT_NODE(node) ((node << 1) + 2)

}	// end namespace mem
#endif	// end #ifndef __SEG_TREE_MEM_MGR_CONST_H__
