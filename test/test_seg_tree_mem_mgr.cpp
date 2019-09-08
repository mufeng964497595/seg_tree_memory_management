#include <stdio.h>
#include <stdint.h>

#include "seg_tree_mem_mgr.h"
#include "seg_tree_mem_mgr_const.h"

using namespace mem;

int main()
{
	CBaseSegTreeMemMgr oMemMgr;
	int iRet = oMemMgr.Build(10 * BLOCK_SIZE);
	if (0 != iRet)
	{
		printf("mem mgr build fail,iRet[%d]\n", iRet);
		return -1;
	}

	oMemMgr.ShowSegTree();
	void* pddwTwo = oMemMgr.Malloc(sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	void* pdwOne = oMemMgr.Malloc(sizeof(uint32_t));
	oMemMgr.ShowSegTree();
	void* pddwThree = oMemMgr.Malloc(sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pddwTwo, sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	pddwTwo = oMemMgr.Malloc(sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	void* pdwFour = oMemMgr.Malloc(sizeof(uint32_t));
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pdwOne, sizeof(uint64_t));
	oMemMgr.ShowSegTree();
	void* pddwFive = oMemMgr.Malloc(sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pddwThree, sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pdwFour, sizeof(uint32_t));
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pddwFive, sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();
	oMemMgr.Dealloc(pddwTwo, sizeof(uint64_t) << 1);
	oMemMgr.ShowSegTree();

	return 0;
}
