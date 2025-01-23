#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"

void syscall_entry (void);
void syscall_handler (struct intr_frame *);

/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);
}

/* The main system call interface */
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	// printf ("system call!\n");
	// printf("(syscall_handler) rax: %x\n", f->R.rax);
	// printf("(syscall_handler) rsp: %x\n", f->rsp);
	// printf("(syscall_handler) rip: %x\n", f->rip);
	// printf("(syscall_handler) vec_no: %x\n", f->vec_no);
	// printf("(syscall_handler) rdi: %x\n", f->R.rdi);
	// printf("(syscall_handler) rsi: %x\n", f->R.rsi);
	// printf("(syscall_handler) rdx: %x\n", f->R.rdx);

	switch(f->R.rax) {
		case SYS_EXIT:
			exit(f->R.rdi);
		case SYS_WRITE:
			f->R.rax = write(f->R.rdi, f->R.rsi, f->R.rdx);
			return;
		default:
			printf("(syscall_handler) 통과!\n");
	}


	// printf("(syscall_handler) syscall_handler 호출\n");
	// printf("(syscall_handler) 현재 쓰레드: %s\n", thread_current()->name);
	// if (f->R.rax == 10) {
	// 	f->R.rax = write(f->R.rdi, f->R.rsi, f->R.rdx);
	// 	return;
	// }
	// ready_list_print();
		

}

int
exit (int status) {
	// todo: 오류 구현하기
	// printf("(exit) f->R.rdi: %d\n", status);
	thread_exit ();
	return 0;
}

int write (int fd, void *buffer, unsigned size) {
	// todo: fd가 1이 아닌 나머지일 때 구현하기 - 테이블 만들기..?
	if (fd == 1) {
		putbuf(buffer, size);
	}
	
	return size;
}
