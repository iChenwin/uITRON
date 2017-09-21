/** 
 *  Hyper Operating System  Application Framework
 *
 * @file  memdrv.h
 * @brief %jp{メモリマップドファイル用デバイスドライバ}
 *
 * Copyright (C) 2006-2007 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


#include "memdrv_local.h"


/** 削除 */
void MemDrv_Delete(HANDLE hDriver)
{
	C_MEMDRV	*self;
	
	/* upper cast */
	self = (C_MEMDRV *)hDriver;

	/* デストラクタ呼び出し */
	MemDrv_Destructor(self);
	
	/* メモリ削除 */
	SysMem_Free(self);
}



/* end of file */
