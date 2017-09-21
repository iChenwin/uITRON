# ---------------------------------------------------------------------------
#  Hyper Operating System V4 Advance
#
#                                     Copyright (C) 2007 by Project HOS
#                                     http://sourceforge.jp/projects/hos/
# ---------------------------------------------------------------------------


				.text


# ------------------------------------------------
#  割り込み待ち
#  void _kernel_wai_int(void)
# ------------------------------------------------
				.align	4
				.frame	__kernel_wai_int, 0
				.globl	__kernel_wai_int
__kernel_wai_int:
				halt
				jr		__kernel_wai_int


# end of file
