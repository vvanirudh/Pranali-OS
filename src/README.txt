1.


The booting code is found in src/guestos.c The snippets are given below. 


void boot(void) {
    // to install system calls
    install_signals();
    set_defaults();
}


void install_signals(void){
    signal(SIGINT, &sim_signal_handler);
            signal(SIGABRT, &sim_signal_handler);
            signal(SIGFPE, &sim_signal_handler);
            signal(SIGUSR2, &sim_signal_handler);
}


void set_defaults(void) {
    char param_value[LINE_MAX+1];
    get_param("INSTR_SLICE", param_value);
    instr_slice = atoi(param_value);
    int heads, tracks, sectors;
    char command[1000];
    get_param("NUM_HEADS",param_value);
    heads=atoi(param_value);
    get_param("NUM_TRACKS",param_value);
    tracks=atoi(param_value);
    get_param("NUM_SECTORS",param_value);
    sectors=atoi(param_value);
    sprintf(command,"(dd if=/dev/zero of=Sim_disk bs=%dx%dx%db count=1) 2> /dev/zero",heads,tracks,sectors);
    system(command);
}

-----------------------------------------------------------------------------------------------------------------------
2.

We have changed the specification of the configuration file as follows:


INSTR_SLICE=20
NUM_HEADS=4
NUM_TRACKS=160
NUM_SECTORS=36

-----------------------------------------------------------------------------------------------------------------------

3.


*(a) The system call handler in GuestOs is implemented in the syscall_do() function in src/libm2kernel/syscall.c . This function is called when the opcode of the decoded instruction is same as op_int_imm8 (i.e. if inum==0x80) 


(b) 327 (this can be find in src/libm2kernel/syscall.dat )


*(c) When the system call number <= 325, the guestOS passes it onto the host OS (as can be seen in the following snippet)


if (syscode > 325) {
            retval = handle_guest_syscalls();
} else // Pass system call to host os
{


Each of such syscalls is handled by functions which pass them onto the host OS for execution with a help of a switch statement. The parameters of the syscall are passed onto host OS in registers or in memory.


(d) If a parameter is the address of some location in the user program then the address is translated by MMU and after retrieving the parameter, the parameter is copied into the memory of guestOS and the real system call is made. In the code, we can see that the read system call gets the parameters from isa_regs (the registers) in which they are stored after retrieving them with the help of MMU.


guest_fd = isa_regs->ebx;
pbuf = isa_regs->ecx;
count = isa_regs->edx;

-----------------------------------------------------------------------------------------------------------------------


5.


(a) 
(i) The library function written can be seen in src/syscall.c in the handle_guest_syscalls() function.


(ii) The test programs used are test1.c and test.c. We can see the difference in speed between both the programs due to the change in instruction slice.

-----------------------------------------------------------------------------------------------------------------------

4.


(a)The PCBs are stored in struct kernel_t in the following fields which is present in src/libm2skernel/m2skernel.h file
fields in struct kernel_t
struct ctx_t *context_list_head, *context_list_tail;
struct ctx_t *running_list_head, *running_list_tail;
struct ctx_t *suspended_list_head, *suspended_list_tail;
struct ctx_t *zombie_list_head, *zombie_list_tail;
struct ctx_t *finished_list_head, *finished_list_tail;
struct ctx_t *alloc_list_head, *alloc_list_tail;


(b) the field that contains the instruction-slice of a process is int instr_slice in src/libm2skernel/context.c


(c) Design of scheduler of guest OS:
The guest OS scheduler is capable of scheduling different processes. It is not time-driven and executes one-by-one instruction from each user process in a round-robin way.
Related code to scheduler of guest OS:

void ke_run(void) function in src/libm2skernel/m2skernel.c
for (ctx = ke->running_list_head; ctx; ctx = ctx->running_next) {
int i;
for ( i = 0 ; i < ctx->instr_slice ; ++i) {
ctx_execute_inst(ctx);
}
}
