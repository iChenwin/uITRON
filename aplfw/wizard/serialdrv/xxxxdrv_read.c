/** 
 * $PROJECT_NAME$
 *
 * @file  xxxxdrv_read.c
 * @brief %jp{$OBJNAME_JP$ 読込み}%en{$OBJNAME_EN$  read}
 *
 * $COPYRIGHT$
 */


#include "xxxxdrv_local.h"


/** %jp{読込み} */
FILE_SIZE XxxxDrv_Read(C_DRVOBJ *pDrvObj, C_FILEOBJ *pFileObj, void *pBuf, FILE_SIZE Size)
{
	C_XXXXDRV		*self;
	C_SYNCFILE		*pFile;
	unsigned char	*pubBuf;
	FILE_ERR		ErrCode;
	FILE_SIZE		i;
	int				c;
	
	/* upper cast */
	self  = (C_XXXXDRV *)pDrvObj;
	pFile = (C_SYNCFILE *)pFileObj;


	pubBuf = (unsigned char *)pBuf;

	/* 読込み処理開始 */
	if ( (ErrCode = SyncDrv_StartProcess(&self->SyncDrv, pFile, SYNCDRV_FACTOR_READ)) != FILE_ERR_OK )
	{
		return (FILE_SIZE)ErrCode;
	}

	/* 読込みシグナルを一旦クリア */
	SyncFile_ClearSignal(pFile, SYNCDRV_FACTOR_READ);
	
	for ( i = 0; i < Size; i++ )
	{
		/* 読み出し */
		while ( (c = StreamBuf_RecvChar(&self->StmBufRecv)) < 0 )
		{
			/* ブロッキングモードでなければ抜ける */
			if ( SyncFile_GetSyncMode(pFile, SYNCDRV_FACTOR_READ) != FILE_SYNCMODE_BLOCKING )
			{
				SyncDrv_EndProcess(&self->SyncDrv, SYNCDRV_FACTOR_READ, i);
				return i;
			}
			
			/* 受信を待つ */
			SyncFile_WaitSignal(pFile, SYNCDRV_FACTOR_READ);
			
			/* 読込みシグナルをクリアしてリトライ */
			SyncFile_ClearSignal(pFile, SYNCDRV_FACTOR_READ);
		}
		
		/* 読み出せた文字を格納 */	
		*pubBuf++ = (unsigned char)c;
	}
	
	/* 読み出し処理完了 */
	SyncDrv_EndProcess(&self->SyncDrv, SYNCDRV_FACTOR_READ, (VPARAM)i);

	return i;
}


/* end of file */
