/** 
 *  Hyper Operating System V4 Advance
 *
 * @file  get_mpf.c
 * @brief %jp{固定長メモリプール資源の獲得(ポーリング)}%en{Acquired Fixed-Sized Memory Block(Poling)}
 *
 * Copyright (C) 1998-2006 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */



#include "core/core.h"
#include "object/mpfobj.h"



#if _KERNEL_SPT_PGET_MPF


#if _KERNEL_SPT_TGET_MPF && (_KERNEL_OPT_CODE_SIZE <= _KERNEL_OPT_SPEED)	/* %jp{tget_mpfありで、サイズ優先なら} */

/** %jp{固定長メモリプールの獲得(ポーリング)}%en{Acquired Fixed-Sized Memory Block(Poling)}
 * @param  mpfid	%jp{メモリブロック獲得対象の固定長メモリプールのID番号}%en{ID number of the fixed-sized memory pool from which resource is acquired}
 * @param  p_blkf	%jp{固定長メモリプール生成情報を入れたパケットへのポインタ}%en{Start address of the acquired memory block}
 * @retval E_OK     %jp{正常終了}%en{Normal completion}
 * @retval E_ID     %jp{不正ID番号(mpfidが不正あるいは使用できない)}%en{Invalid ID number(mpfid is invalid or unusable)}
 * @retval E_NOEXS  %jp{オブジェクト未生成(対象固定長メモリプールが未登録)}%en{Non-existant object(specified fixed-sized memory pool is not registerd)}
 * @retval E_PAR    %jp{パラメータエラー(p_blk, tmoutが不正)}%en{Forced release from waiting(accept rel_wai while waiting)}
 * @retval E_RLWAI  %jp{待ち状態の強制解除(待ち状態の間にrel_waiを受付)}%en{Forced release from waiting(accept rel_wai while waiting)}
 * @retval E_DLT    %jp{待ちオブジェクトの削除(待ち状態の間に固定長メモリプールが削除)}%en{Waiting object deleted(fixed-sized memory pool is deleted waiting)}
 */
ER pget_mpf(ID mpfid, VP *p_blk)
{
	/* %jp{tget_mpfで代替する} */
	return tget_mpf(mpfid, p_blk, TMO_POL);
}

#else


/** %jp{固定長メモリプールの獲得(ポーリング)}%en{Acquired Fixed-Sized Memory Block(Poling)}
 * @param  mpfid	%jp{メモリブロック獲得対象の固定長メモリプールのID番号}%en{ID number of the fixed-sized memory pool from which resource is acquired}
 * @param  p_blkf	%jp{固定長メモリプール生成情報を入れたパケットへのポインタ}%en{Start address of the acquired memory block}
 * @retval E_OK     %jp{正常終了}%en{Normal completion}
 * @retval E_ID     %jp{不正ID番号(mpfidが不正あるいは使用できない)}%en{Invalid ID number(mpfid is invalid or unusable)}
 * @retval E_NOEXS  %jp{オブジェクト未生成(対象固定長メモリプールが未登録)}%en{Non-existant object(specified fixed-sized memory pool is not registerd)}
 * @retval E_PAR    %jp{パラメータエラー(p_blk, tmoutが不正)}%en{Forced release from waiting(accept rel_wai while waiting)}
 * @retval E_RLWAI  %jp{待ち状態の強制解除(待ち状態の間にrel_waiを受付)}%en{Forced release from waiting(accept rel_wai while waiting)}
 * @retval E_DLT    %jp{待ちオブジェクトの削除(待ち状態の間に固定長メモリプールが削除)}%en{Waiting object deleted(fixed-sized memory pool is deleted waiting)}
 */
ER pget_mpf(ID mpfid, VP *p_blk)
{
	_KERNEL_T_MPFCB_PTR  mpfcb;
	_KERNEL_MPF_T_BLKHDL blkhdl;
	VP                   blkptr;
	ER                   ercd;
	
	
	/* %jp{ID のチェック} */
#if _KERNEL_SPT_PGET_MPF_E_ID
	if ( !_KERNEL_MPF_CHECK_MPFID(mpfid) )
	{
		return E_ID;	/* %jp{不正ID番号}%en{Invalid ID number} */
	}
#endif
	
	_KERNEL_ENTER_SVC();		/* %jp{サービスコールに入る}%en{enter service-call} */
	
	/* %jp{オブジェクト存在チェック} */
#if _KERNEL_SPT_PGET_MPF_E_NOEXS
	if ( !_KERNEL_MPF_CHECK_EXS(mpfid) )
	{
		_KERNEL_LEAVE_SVC();	/* %jp{サービスコールから出る}%en{leave service-call} */
		return E_NOEXS;			/* %jp{オブジェクト未生成}%en{Non-existant object} */
	}
#endif

	/* %jp{コントロールブロック取得} */
	mpfcb = _KERNEL_MPF_ID2MPFCB(mpfid);
	
	/* %jp{固定長メモリプールカウンタ取得} */
	blkhdl = _KERNEL_MPF_GET_FREBLK(mpfcb);
	
	if ( blkhdl != _KERNEL_MPF_BLKHDL_NULL )
	{
		/* %jp{固定長メモリプール資源が獲得できれば成功} */
		blkptr = _KERNEL_MPF_BLKHDL2PTR(mpfcb, blkhdl);
		*p_blk = blkptr;
		_KERNEL_MPF_SET_FREBLK(mpfcb, *(_KERNEL_MPF_T_BLKHDL *)blkptr);
		_KERNEL_MPF_SET_FBLKCNT(mpfcb, _KERNEL_MPF_GET_FBLKCNT(mpfhdl) - 1);
		ercd = E_OK;
	}
	else
	{
		ercd = E_TMOUT;
	}
	
	_KERNEL_LEAVE_SVC();	/* %jp{オブジェクト未生成}%en{Non-existant object} */
	
	return ercd;
}

#endif


#else	/* _KERNEL_SPT_PGET_MPF */


#if _KERNEL_SPT_PGET_MPF_E_NOSPT

/** %jp{固定長メモリプールの獲得(ポーリング)}%en{Acquired Fixed-Sized Memory Block(Poling)}
 * @param  mpfid	%jp{メモリブロック獲得対象の固定長メモリプールのID番号}%en{ID number of the fixed-sized memory pool from which resource is acquired}
 * @param  p_blkf	%jp{固定長メモリプール生成情報を入れたパケットへのポインタ}%en{Start address of the acquired memory block}
 * @retval E_NOSPT  %jp{未サポート機能}%en{Unsupported function}
 */
ER pget_mpf(ID mpfid)
{
	return E_NOSPT;
}

#endif


#endif	/* _KERNEL_SPT_PGET_MPF */



/* end of file */
