/** 
 *  Hyper Operating System  Application Framework
 *
 * @file  system.h
 * @brief %jp{システム用API定義}
 *
 * Copyright (C) 2006 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


#include "sysapi_local.h"


/* システムメモリの割り当て */
void *SysMem_Alloc(MEMSIZE Size)
{
	return SysMemHeap_Alloc(&SysMem_SysMemHeap, Size);
}


/* end of file */
