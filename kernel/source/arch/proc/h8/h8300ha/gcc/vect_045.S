/* ------------------------------------------------------------------------ */
/* Hyper Operating System V4 Advance                                        */
/*                                                                          */
/*                                  Copyright (C) 1998-2006 by Project HOS  */
/*                                  http://sourceforge.jp/projects/hos/     */
/* ------------------------------------------------------------------------ */

				.h8300h


				.global	__kernel_vector_045
				.global	__kernel_int_hdr


				.text

__kernel_vector_045:
				push.l	er0
				mov.b	#45, r0l
				jmp		__kernel_int_hdr


				.end


/* end of file */
