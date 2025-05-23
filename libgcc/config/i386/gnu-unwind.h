/* DWARF2 EH unwinding support for GNU Hurd: x86.
   Copyright (C) 2020-2025 Free Software Foundation, Inc.
   Contributed by Samuel Thibault <samuel.thibault@gnu.org>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

/* Do code reading to identify a signal frame, and set the frame
   state data appropriately.  See unwind-dw2.c for the structs. */

#ifndef inhibit_libc

#include <signal.h>

#ifdef __x86_64__

#define MD_FALLBACK_FRAME_STATE_FOR x86_gnu_fallback_frame_state

static _Unwind_Reason_Code
x86_gnu_fallback_frame_state
(struct _Unwind_Context *context, _Unwind_FrameState *fs)
{
  static const unsigned char gnu_sigtramp_code[] =
  {
    /* rpc_wait_trampoline: */
    0x48, 0xc7, 0xc0, 0xe7, 0xff, 0xff, 0xff,    /* mov    $-25,%rax */
    0x0f, 0x05,                                  /* syscall */
    0x49, 0x89, 0x04, 0x24,                      /* mov    %rax,(%r12) */
    0x48, 0x89, 0xdc,                            /* mov    %rbx,%rsp */

    /* trampoline: */
    0x5f,                                        /* pop    %rdi */
    0x5e,                                        /* pop    %rsi */
    0x5a,                                        /* pop    %rdx */
    0x48, 0x83, 0xc4, 0x08,                      /* add    $0x8,%rsp */
    0x41, 0xff, 0xd5,                            /* call   *%r13 */

    /* RA HERE */
    0x48, 0x8b, 0x7c, 0x24, 0x10,                /* mov    0x10(%rsp),%rdi */
    0xc3,                                        /* ret */

    /* firewall: */
    0xf4,                                        /* hlt */
  };

  const size_t gnu_sigtramp_len = sizeof gnu_sigtramp_code;
  const size_t gnu_sigtramp_tail = 7; /* length of tail after RA */

  struct stack_contents {
    void *sigreturn_addr;
    void *sigreturn_returns_here;
    struct sigcontext *return_scp;
  } *stack_contents;
  struct sigcontext *scp;
  unsigned long usp;

  unsigned char *adjusted_pc = (unsigned char*)(context->ra) +
    gnu_sigtramp_tail - gnu_sigtramp_len;
  if (memcmp (adjusted_pc, gnu_sigtramp_code, gnu_sigtramp_len))
    return _URC_END_OF_STACK;

  stack_contents = context->cfa;

  scp = stack_contents->return_scp;
  usp = scp->sc_ursp;

  fs->regs.reg[0].loc.offset = (unsigned long)&scp->sc_rax - usp;
  fs->regs.reg[1].loc.offset = (unsigned long)&scp->sc_rdx - usp;
  fs->regs.reg[2].loc.offset = (unsigned long)&scp->sc_rcx - usp;
  fs->regs.reg[3].loc.offset = (unsigned long)&scp->sc_rbx - usp;
  fs->regs.reg[4].loc.offset = (unsigned long)&scp->sc_rsi - usp;
  fs->regs.reg[5].loc.offset = (unsigned long)&scp->sc_rdi - usp;
  fs->regs.reg[6].loc.offset = (unsigned long)&scp->sc_rbp - usp;
  fs->regs.reg[8].loc.offset = (unsigned long)&scp->sc_r8 - usp;
  fs->regs.reg[9].loc.offset = (unsigned long)&scp->sc_r9 - usp;
  fs->regs.reg[10].loc.offset = (unsigned long)&scp->sc_r10 - usp;
  fs->regs.reg[11].loc.offset = (unsigned long)&scp->sc_r11 - usp;
  fs->regs.reg[12].loc.offset = (unsigned long)&scp->sc_r12 - usp;
  fs->regs.reg[13].loc.offset = (unsigned long)&scp->sc_r13 - usp;
  fs->regs.reg[14].loc.offset = (unsigned long)&scp->sc_r14 - usp;
  fs->regs.reg[15].loc.offset = (unsigned long)&scp->sc_r15 - usp;
  fs->regs.reg[16].loc.offset = (unsigned long)&scp->sc_rip - usp;

  /* Register 7 is rsp  */
  fs->regs.cfa_how = CFA_REG_OFFSET;
  fs->regs.cfa_reg = 7;
  fs->regs.cfa_offset = usp - (unsigned long) context->cfa;

  fs->regs.how[0] = REG_SAVED_OFFSET;
  fs->regs.how[1] = REG_SAVED_OFFSET;
  fs->regs.how[2] = REG_SAVED_OFFSET;
  fs->regs.how[3] = REG_SAVED_OFFSET;
  fs->regs.how[4] = REG_SAVED_OFFSET;
  fs->regs.how[5] = REG_SAVED_OFFSET;
  fs->regs.how[6] = REG_SAVED_OFFSET;
  fs->regs.how[8] = REG_SAVED_OFFSET;
  fs->regs.how[9] = REG_SAVED_OFFSET;
  fs->regs.how[10] = REG_SAVED_OFFSET;
  fs->regs.how[11] = REG_SAVED_OFFSET;
  fs->regs.how[12] = REG_SAVED_OFFSET;
  fs->regs.how[13] = REG_SAVED_OFFSET;
  fs->regs.how[14] = REG_SAVED_OFFSET;
  fs->regs.how[15] = REG_SAVED_OFFSET;
  fs->regs.how[16] = REG_SAVED_OFFSET;

  fs->retaddr_column = 16;
  fs->signal_frame = 1;

  return _URC_NO_REASON;
}

#else /* ifdef __x86_64__  */

#define MD_FALLBACK_FRAME_STATE_FOR x86_gnu_fallback_frame_state

static _Unwind_Reason_Code
x86_gnu_fallback_frame_state
(struct _Unwind_Context *context, _Unwind_FrameState *fs)
{
  struct handler_args {
    int signo;
    union
      {
	struct
	  {
	    long int sigcode;
	    struct sigcontext *scp;
	  } legacy;
	struct
	  {
	    siginfo_t *siginfop;
	    ucontext_t *uctxp;
	  } posix;
      };
  } *handler_args;
  long int sigcode;
  unsigned long usp;

/*
 * i386 sigtramp frame we are looking for follows.
 * (see glibc/sysdeps/mach/hurd/i386/trampoline.c assembly)
 *
 * rpc_wait_trampoline:
 *   0:	b8 e7 ff ff ff       	mov    $-25,%eax       mach_msg_trap
 *   5:	9a 00 00 00 00 07 00 	lcall  $7,$0
 *  12:	89 01                	movl   %eax, (%ecx)
 *  14:	89 dc                	movl   %ebx, %esp      switch to signal stack
 *
 * trampoline:
 *  16:	ff d2                	call   *%edx           call the handler function
 * RA HERE
 *  18:	83 c4 0c             	addl   $12, %esp       pop its args
 *  21:	c3                   	ret                    return to sigreturn
 *
 * firewall:
 *  22:	f4                   	hlt
 */

  if (!(   *(unsigned int   *)(context->ra     ) == 0xc30cc483
        && *(unsigned char  *)(context->ra +  4) ==       0xf4

        && *(unsigned int   *)(context->ra -  4) == 0xd2ffdc89
        && *(unsigned int   *)(context->ra -  8) == 0x01890007
        && *(unsigned int   *)(context->ra - 12) == 0x00000000
        && *(unsigned int   *)(context->ra - 16) == 0x9affffff
        && *(unsigned short *)(context->ra - 18) ==     0xe7b8))
    return _URC_END_OF_STACK;

  handler_args = context->cfa;
  sigcode = handler_args->legacy.sigcode;
  if (sigcode >= -16 && sigcode < 4096)
    {
      /* This cannot be a SIGINFO pointer, assume legacy.  */
      struct sigcontext *scp = handler_args->legacy.scp;
      usp = scp->sc_uesp;

      fs->regs.reg[0].loc.offset = (unsigned long)&scp->sc_eax - usp;
      fs->regs.reg[1].loc.offset = (unsigned long)&scp->sc_ecx - usp;
      fs->regs.reg[2].loc.offset = (unsigned long)&scp->sc_edx - usp;
      fs->regs.reg[3].loc.offset = (unsigned long)&scp->sc_ebx - usp;
      fs->regs.reg[5].loc.offset = (unsigned long)&scp->sc_ebp - usp;
      fs->regs.reg[6].loc.offset = (unsigned long)&scp->sc_esi - usp;
      fs->regs.reg[7].loc.offset = (unsigned long)&scp->sc_edi - usp;
      fs->regs.reg[8].loc.offset = (unsigned long)&scp->sc_eip - usp;
    }
  else
    {
      /* This is not a valid sigcode, assume SIGINFO.  */
      ucontext_t *uctxp = handler_args->posix.uctxp;
      gregset_t *gregset = &uctxp->uc_mcontext.gregs;
      usp = (*gregset)[REG_UESP];

      fs->regs.reg[0].loc.offset = (unsigned long)&(*gregset)[REG_EAX] - usp;
      fs->regs.reg[1].loc.offset = (unsigned long)&(*gregset)[REG_ECX] - usp;
      fs->regs.reg[2].loc.offset = (unsigned long)&(*gregset)[REG_EDX] - usp;
      fs->regs.reg[3].loc.offset = (unsigned long)&(*gregset)[REG_EBX] - usp;
      fs->regs.reg[5].loc.offset = (unsigned long)&(*gregset)[REG_EBP] - usp;
      fs->regs.reg[6].loc.offset = (unsigned long)&(*gregset)[REG_ESI] - usp;
      fs->regs.reg[7].loc.offset = (unsigned long)&(*gregset)[REG_EDI] - usp;
      fs->regs.reg[8].loc.offset = (unsigned long)&(*gregset)[REG_EIP] - usp;
    }

  fs->regs.cfa_how = CFA_REG_OFFSET;
  fs->regs.cfa_reg = 4;
  fs->regs.cfa_offset = usp - (unsigned long) context->cfa;

  fs->regs.how[0] = REG_SAVED_OFFSET;
  fs->regs.how[1] = REG_SAVED_OFFSET;
  fs->regs.how[2] = REG_SAVED_OFFSET;
  fs->regs.how[3] = REG_SAVED_OFFSET;
  fs->regs.how[5] = REG_SAVED_OFFSET;
  fs->regs.how[6] = REG_SAVED_OFFSET;
  fs->regs.how[7] = REG_SAVED_OFFSET;
  fs->regs.how[8] = REG_SAVED_OFFSET;

  fs->retaddr_column = 8;
  fs->signal_frame = 1;

  return _URC_NO_REASON;
}

#endif /* ifdef __x86_64__  */

#endif /* ifndef inhibit_libc */
