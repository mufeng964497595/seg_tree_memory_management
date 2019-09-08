#ifndef __SEG_TREE_MEM_MGR_H__
#define __SEG_TREE_MEM_MGR_H__

/**
 * 描述：使用线段树实现内存管理
 * 作者：huangweifeng
 * 时间：2019.08.22
 */

#include <stdint.h>
#include <vector>
#include <stdlib.h>

#include "seg_tree_mem_mgr_const.h"

namespace mem
{
// 线段树内存管理节点类
class CSegTreeMemMgrNode
{
public:
	CSegTreeMemMgrNode(void* pStartAddr = NULL, void* pEndAddr = NULL
						, uint32_t dwMaxMemBytes = 0, uint32_t dwLeftMemBytes = 0
						, uint32_t dwRightMemBytes = 0)
		:
			m_pStartAddr(pStartAddr), m_pEndAddr(pEndAddr), m_dwMaxMemBytes(dwMaxMemBytes)
			, m_dwLeftMemBytes(dwLeftMemBytes), m_dwRightMemBytes(dwRightMemBytes), m_cLazy(LAZY_EMPTY)
		{}
	~CSegTreeMemMgrNode()
	{
		m_pStartAddr = NULL;
		m_pEndAddr = NULL;
	}

public:
	void* m_pStartAddr;			// 该节点的起始地址（闭区间）
	void* m_pEndAddr;			// 该节点的终止地址（开区间）
	uint32_t m_dwMaxMemBytes;	// 该节点下最大的连续可用的地址空间字节数
	uint32_t m_dwLeftMemBytes;	// 以起始地址为起点的连续可用的地址空间字节数
	uint32_t m_dwRightMemBytes;	// 以终止地址为终点的连续可用的地址空间字节数
	
	uint8_t m_cLazy;			// 延时标记
};	// end class CSegTreeMemMgrNode

// 线段树内存管理基类
class CBaseSegTreeMemMgr
{
public:
	CBaseSegTreeMemMgr()
	{
		m_pMem = NULL;
	}
	virtual ~CBaseSegTreeMemMgr()	// 使用virtual，便于被继承
	{
		if (m_pMem) free(m_pMem);
	}

public:
	/**
	 * 创建内存管理线段树
	 * 参数：dwMemSize 用户申请的内存大小，单位byte
	 * 返回值：错误码，0表示成功，非0表示失败
	 */
	int Build(uint32_t dwMemSize);

	/**
	 * 申请内存
	 * 参数：dwSize 要申请的内存大小，单位byte
	 * 返回值：分配的对象指针，分配失败返回NULL
	 */
	void* Malloc(uint32_t dwSize);

	/**
	 * 回收对象内存
	 * 参数：pObj 待释放内存的对象
	 */
	template <typename T>
	void Dealloc(T* pObj);

	/**
	 * 回收对象内存
	 * 参数：pObj 待释放内存的对象
	 * 参数：dwSize 待释放对象的大小，单位byte
	 */
	template <typename T>
	void Dealloc(T* pObj, uint32_t dwSize);

	/**
	 * 打印线段树，用于debug
	 */
	void ShowSegTree();
protected:
	/**
	 * 调整用户申请的bytes为切割好的内存块大小的倍数
	 * 参数：dwBytes 用户申请的字节数
	 * 返回值：调整后的字节数
	 */
	uint32_t RoundUp(uint32_t dwBytes);

	/**
	 * 递归创建内存管理线段树
	 * 参数：dwNodeId 当前构造的节点的id
	 * 参数：dwStart 当前构造的节点管理的起始区间块
	 * 参数：dwEnd 当前构造的节点管理的终止区间块
	 */
	void Build(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd);

	/**
	 * 递归查询分配内存，分配地址最低的空间
	 * 参数：dwSize 要分配的内存大小，单位byte
	 * 参数：dwNodeId 当前查询的线段树节点id
	 * 参数：dwStart 当前查询的线段树节点管理的内存块起始id
	 * 参数：dwEnd 当前查询的线段树节点管理的内存块终止id
	 * 返回值：分配的对象指针，分配失败返回NULL
	 */
	void* Malloc(uint32_t dwSize, uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd);

	/**
	 * 维持当前节点的数据
	 * 参数：dwNodeId 当前构造的节点的id
	 */
	void Maintain(uint32_t dwNodeId);

	/**
	 * 更新内存块状态
	 * 参数：dwNodeId 当前处理的线段树节点id
	 * 参数：dwStart 当前处理的线段树节点管理的起始内存块id
	 * 参数：dwEnd 当前处理的线段树节点管理的终止内存块id
	 * 参数：dwLeft 要更新的内存块起始区间id
	 * 参数：dwRight 要更新的内存块终止区间id
	 * 参数：cMemStatus 要更新的内存块状态
	 */
	void UpdateMemStatus(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd, uint32_t dwLeft, uint32_t dwRight, uint8_t cMemStatus);

	/**
	 * 下放lazy标记
	 * 参数：dwNodeId 当前处理的线段树节点id
	 * 参数：dwStart 当前处理的线段树节点管理的内存块起始id
	 * 参数：dwEnd 当前处理的线段树节点管理的内存块终止id
	 */
	void PushDown(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd);

	/**
	 * 打印线段树，用于debug
	 * 参数：dwNodeId 当前处理的线段树节点id
	 * 参数：dwStart 当前处理的线段树节点管理的起始内存块id
	 * 参数：dwEnd 当前处理的线段树节点管理的终止内存块id
	 */
	void ShowSegTree(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd);

protected:
	std::vector<CSegTreeMemMgrNode> m_vSegTree;		// 内存管理线段树
	void* m_pMem;		// 管理的内存
	uint32_t m_dwBlockNum;	// 管理的内存块个数
};	// end class CBaseSegTreeMemMgr

/**
 * 创建内存管理线段树
 * 参数：dwMemSize 用户申请的内存大小，单位byte
 * 返回值：错误码，0表示成功，非0表示失败
 */
int CBaseSegTreeMemMgr::Build(uint32_t dwMemSize)
{
	if (0 == dwMemSize)
	{
		printf("error: build size is 0\n");
		return -1;
	}

	if (m_pMem)
	{
		// 释放空间，以便重新分配
		free(m_pMem);
		std::vector<CSegTreeMemMgrNode>().swap(m_vSegTree);
	}

	uint32_t dwSize = RoundUp(dwMemSize);
	m_pMem = malloc(dwSize);
	if (!m_pMem)
	{
		printf("error: malloc size[%u] fail\n", dwSize);
		return -1;
	}
	// 将内存分块
	m_dwBlockNum = dwSize / BLOCK_SIZE;
	m_vSegTree.resize(m_dwBlockNum << 2);	// 提前分配空间，尚未进行空间优化，需要分配4倍空间
	// 递归建树
	Build(ROOT_ID, 0, m_dwBlockNum - 1);

	printf("build success\n");
	return 0;
}

/**
 * 调整用户申请的bytes为切割好的内存块大小的倍数
 * 参数：dwBytes 用户申请的字节数
 * 返回值：调整后的字节数
 */
uint32_t CBaseSegTreeMemMgr::RoundUp(uint32_t dwBytes)
{
	return (dwBytes + BLOCK_SIZE - 1) & (~(BLOCK_SIZE - 1));
}

/**
 * 递归创建内存管理线段树
 * 参数：dwNodeId 当前构造的节点的id
 * 参数：dwStart 当前构造的节点管理的起始区间块
 * 参数：dwEnd 当前构造的节点管理的终止区间块
 */
void CBaseSegTreeMemMgr::Build(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd)
{
	if (dwStart == dwEnd)
	{
		// // 只剩一个块，不保存数据，节省空间
		void* pStartAddr = (void*)((char*)m_pMem + dwStart * BLOCK_SIZE);
		void* pEndAddr = (void*)((char*)pStartAddr + BLOCK_SIZE);
		m_vSegTree[dwNodeId] = CSegTreeMemMgrNode(pStartAddr, pEndAddr, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
		return;
	}
//	if (dwStart + 1 == dwEnd)
//	{
//		// 该节点管理两个块，作为叶子节点
//
//	}

	uint32_t dwMid = (dwStart + dwEnd) >> 1;
	// 构造左子树
	Build(LEFT_NODE(dwNodeId), dwStart, dwMid);
	// 构造右子树
	Build(RIGHT_NODE(dwNodeId), dwMid + 1, dwEnd);
	// 维持当前节点的数据
	Maintain(dwNodeId);
}

/**
 * 维持当前节点的数据
 * 参数：dwNodeId 当前构造的节点的id
 */
void CBaseSegTreeMemMgr::Maintain(uint32_t dwNodeId)
{
	uint32_t dwLeftId = LEFT_NODE(dwNodeId);
	uint32_t dwRightId = RIGHT_NODE(dwNodeId);
	void* pStartAddr = m_vSegTree[dwLeftId].m_pStartAddr;
	void* pEndAddr = m_vSegTree[dwRightId].m_pEndAddr;
	// 最大连续可用空间可能在左子树、右子树，或两颗子树的交汇处
	uint32_t dwMaxMemBytes = std::max(std::max(m_vSegTree[dwLeftId].m_dwMaxMemBytes, m_vSegTree[dwRightId].m_dwMaxMemBytes)
		, m_vSegTree[dwLeftId].m_dwRightMemBytes + m_vSegTree[dwRightId].m_dwLeftMemBytes);
	// 以起始地址为起点的最大连续可用空间可能跨越整个左子树
	uint32_t dwLeftMemBytes = m_vSegTree[dwLeftId].m_dwLeftMemBytes;
	if ((char*)m_vSegTree[dwLeftId].m_pStartAddr + dwLeftMemBytes == (char*)m_vSegTree[dwLeftId].m_pEndAddr)
	{
		dwLeftMemBytes += m_vSegTree[dwRightId].m_dwLeftMemBytes;
	}
	// 以终止地址为终点的最大连续可用空间可能跨越整个右子树
	uint32_t dwRightMemBytes = m_vSegTree[dwRightId].m_dwRightMemBytes;
	if ((char*)m_vSegTree[dwRightId].m_pStartAddr + dwRightMemBytes == (char*)m_vSegTree[dwRightId].m_pEndAddr)
	{
		dwRightMemBytes += m_vSegTree[dwLeftId].m_dwRightMemBytes;
	}
	
	m_vSegTree[dwNodeId] = CSegTreeMemMgrNode(pStartAddr, pEndAddr, dwMaxMemBytes, dwLeftMemBytes, dwRightMemBytes);
}

/**
 * 分配内存
 * 参数：dwSize 要分配的内存大小，单位byte
 * 返回值：分配的对象指针，分配失败返回NULL
 */
void* CBaseSegTreeMemMgr::Malloc(uint32_t dwSize)
{
	if (0 == dwSize) return NULL;
	// 分配内存
	uint32_t dwReqSize = RoundUp(dwSize);
	printf("malloc input_size[%u], round_up_size[%u]\n", dwSize, dwReqSize);
	void* pMem = Malloc(dwReqSize, ROOT_ID, 0, m_dwBlockNum - 1);
	if (!pMem) return NULL;

	// 更新内存块使用情况
	uint32_t dwLeft = ((char*)pMem - (char*)m_pMem) / BLOCK_SIZE;
	uint32_t dwRight = dwLeft + dwReqSize / BLOCK_SIZE - 1;
	UpdateMemStatus(ROOT_ID, 0, m_dwBlockNum - 1, dwLeft, dwRight, LAZY_USED);

	return pMem;
}

/**
 * 递归查询分配内存，分配地址最低的空间，首次适应算法（First Fit）
 * 参数：dwSize 要分配的内存大小，单位byte
 * 参数：dwNodeId 当前查询的线段树节点id
 * 参数：dwStart 当前查询的线段树节点管理的内存块起始id
 * 参数：dwEnd 当前查询的线段树节点管理的内存块终止id
 * 返回值：分配的对象指针，分配失败返回NULL
 */
void* CBaseSegTreeMemMgr::Malloc(uint32_t dwSize, uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd)
{
	if (m_vSegTree[dwNodeId].m_dwMaxMemBytes < dwSize)
	{
		printf("error: max free bytes is %u,require[%u], Malloc fail\n", m_vSegTree[dwNodeId].m_dwMaxMemBytes, dwSize);
		return NULL;
	}

	if (m_vSegTree[dwNodeId].m_dwLeftMemBytes >= dwSize)
	{
		// 该节点左侧起点有足够的空间，分配之
		return m_vSegTree[dwNodeId].m_pStartAddr;
	}

	// 下放lazy标记
	PushDown(dwNodeId, dwStart, dwEnd);
	// 可分配的内存可能在左子树、左右子树交汇 或 右子树
	uint32_t dwMid = (dwStart + dwEnd) >> 1;
	void* pMem = NULL;
	if (m_vSegTree[LEFT_NODE(dwNodeId)].m_dwMaxMemBytes >= dwSize)
	{
		// 左子树
		pMem = Malloc(dwSize, LEFT_NODE(dwNodeId), dwStart, dwMid);
	}
	else if (m_vSegTree[LEFT_NODE(dwNodeId)].m_dwRightMemBytes + m_vSegTree[RIGHT_NODE(dwNodeId)].m_dwLeftMemBytes >= dwSize)
	{
		// 左右子树交汇
		pMem = (void*)((char*)m_vSegTree[LEFT_NODE(dwNodeId)].m_pEndAddr - m_vSegTree[LEFT_NODE(dwNodeId)].m_dwRightMemBytes);
	}
	else
	{
		// 右子树
		pMem = Malloc(dwSize, RIGHT_NODE(dwNodeId), dwMid + 1, dwEnd);
	}

	return pMem;
}

/**
 * 更新内存块状态
 * 参数：dwNodeId 当前处理的线段树节点id
 * 参数：dwStart 当前处理的线段树节点管理的起始内存块id
 * 参数：dwEnd 当前处理的线段树节点管理的终止内存块id
 * 参数：dwLeft 要更新的内存块起始区间id
 * 参数：dwRight 要更新的内存块终止区间id
 * 参数：cMemStatus 要更新的内存块状态
 */
void CBaseSegTreeMemMgr::UpdateMemStatus(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd, uint32_t dwLeft, uint32_t dwRight, uint8_t cMemStatus)
{
	if (dwStart > dwRight || dwEnd < dwLeft) return;

	if (dwStart >= dwLeft && dwEnd <= dwRight)
	{
		// 整个节点都更新
		if (LAZY_USED == cMemStatus)
		{
			m_vSegTree[dwNodeId].m_dwMaxMemBytes = m_vSegTree[dwNodeId].m_dwLeftMemBytes = m_vSegTree[dwNodeId].m_dwRightMemBytes = 0;
		}
		else if (LAZY_UNUSED == cMemStatus)
		{
			m_vSegTree[dwNodeId].m_dwMaxMemBytes = m_vSegTree[dwNodeId].m_dwLeftMemBytes = m_vSegTree[dwNodeId].m_dwRightMemBytes = (dwEnd - dwStart + 1) * BLOCK_SIZE;
		}
		m_vSegTree[dwNodeId].m_cLazy = cMemStatus;

		return;
	}

	// 下放lazy标记
	PushDown(dwNodeId, dwStart, dwEnd);
	// 更新左右子树
	uint32_t dwMid = (dwStart + dwEnd) >> 1;
	UpdateMemStatus(LEFT_NODE(dwNodeId), dwStart, dwMid, dwLeft, dwRight, cMemStatus);
	UpdateMemStatus(RIGHT_NODE(dwNodeId), dwMid + 1, dwEnd, dwLeft, dwRight, cMemStatus);
	// 维护当前节点
	Maintain(dwNodeId);
}

/**
 * 下放lazy标记
 * 参数：dwNodeId 当前处理的线段树节点id
 * 参数：dwStart 当前处理的线段树节点管理的内存块起始id
 * 参数：dwEnd 当前处理的线段树节点管理的内存块终止id
 */
void CBaseSegTreeMemMgr::PushDown(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd)
{
	if (dwStart == dwEnd) return;

	CSegTreeMemMgrNode& oNode = m_vSegTree[dwNodeId];
	if (LAZY_EMPTY == oNode.m_cLazy) return;

	// 更新左右子树节点信息
	CSegTreeMemMgrNode& oLeftNode = m_vSegTree[LEFT_NODE(dwNodeId)];
	CSegTreeMemMgrNode& oRightNode = m_vSegTree[RIGHT_NODE(dwNodeId)];
	if (LAZY_USED == oNode.m_cLazy)
	{
		// 整个节点都被占用了，可用空间清零
		oLeftNode.m_dwMaxMemBytes = oLeftNode.m_dwLeftMemBytes = oLeftNode.m_dwRightMemBytes = 0;
		oRightNode.m_dwMaxMemBytes = oRightNode.m_dwLeftMemBytes = oRightNode.m_dwRightMemBytes = 0;
	}
	else if (LAZY_UNUSED == oNode.m_cLazy)
	{
		// 整个节点都未使用，可用空间填满
		uint32_t dwMid = (dwStart + dwEnd) >> 1;
		oLeftNode.m_dwMaxMemBytes = oLeftNode.m_dwLeftMemBytes = oLeftNode.m_dwRightMemBytes = (dwMid - dwStart + 1) * BLOCK_SIZE;
		oRightNode.m_dwMaxMemBytes = oRightNode.m_dwLeftMemBytes = oRightNode.m_dwRightMemBytes = (dwEnd - dwMid) * BLOCK_SIZE;
	}
	// 清除该节点的lazy标记
	oNode.m_cLazy = LAZY_EMPTY;
	// 左右子节点打上lazy标记
	oLeftNode.m_cLazy = oRightNode.m_cLazy = oNode.m_cLazy;
}

/**
 * 回收对象内存
 * 参数：pObj 待释放内存的对象
 */
template <typename T>
void CBaseSegTreeMemMgr::Dealloc(T* pObj)
{
	if (!pObj) return;
	Dealloc(pObj, sizeof(T));
}

/**
 * 回收对象内存
 * 参数：pObj 待释放内存的对象
 * 参数：dwSize 待释放对象的大小，单位byte
 */
template <typename T>
void CBaseSegTreeMemMgr::Dealloc(T* pObj, uint32_t dwSize)
{
	if (!pObj) return;

	dwSize = RoundUp(dwSize);
	int iLeft = ((char*)pObj - (char*)m_pMem) / BLOCK_SIZE;
	int iRight = iLeft + dwSize / BLOCK_SIZE - 1;
	if (iLeft < 0 || (uint32_t)iLeft >= m_dwBlockNum || iRight < 0 || (uint32_t)iRight >= m_dwBlockNum)
	{
		printf("error: dealloc [%d, %d],but seg_tree is [%u, %u]\n", iLeft, iRight, 0, m_dwBlockNum - 1);
		return;
	}

	UpdateMemStatus(ROOT_ID, 0, m_dwBlockNum - 1, iLeft, iRight, LAZY_UNUSED);
}

/**
 * 打印线段树，用于debug
 */
void CBaseSegTreeMemMgr::ShowSegTree()
{
	printf("======================================\n");
	ShowSegTree(ROOT_ID, 0, m_dwBlockNum - 1);
	printf("======================================\n");
}

/**
 * 打印线段树，用于debug
 * 参数：dwNodeId 当前处理的线段树节点id
 * 参数：dwStart 当前处理的线段树节点管理的起始内存块id
 * 参数：dwEnd 当前处理的线段树节点管理的终止内存块id
 */
void CBaseSegTreeMemMgr::ShowSegTree(uint32_t dwNodeId, uint32_t dwStart, uint32_t dwEnd)
{
	CSegTreeMemMgrNode& oNode = m_vSegTree[dwNodeId];
	printf("nodeid[%u],startid[%u],endid[%u],startaddr[%p],endaddr[%p],max[%u],leftmax[%u],rightmax[%u],lazy[%d]\n"
		, dwNodeId, dwStart, dwEnd, oNode.m_pStartAddr, oNode.m_pEndAddr, oNode.m_dwMaxMemBytes, oNode.m_dwLeftMemBytes, oNode.m_dwRightMemBytes, oNode.m_cLazy);

	if (dwStart == dwEnd) return;

	uint32_t dwMid = (dwStart + dwEnd) >> 1;
	ShowSegTree(LEFT_NODE(dwNodeId), dwStart, dwMid);
	ShowSegTree(RIGHT_NODE(dwNodeId), dwMid + 1, dwEnd);
}

}	// end namespace mem

#endif	// end #ifndef __SEG_TREE_MEM_MGR_H__
