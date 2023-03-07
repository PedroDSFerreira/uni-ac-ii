    .equ READ_CORE_TIMER,11 
    .equ RESET_CORE_TIMER,12
 
    .data
    .text
    .globl delay

delay:
    # $a0: ms
    li  $v0,RESET_CORE_TIMER    #  
    syscall                     #   resetCoreTimer()

    mul $t1, $a0, 20000
del_while:
    li  $v0,READ_CORE_TIMER     #   readCoreTimer()
    syscall                     #   counter=readCoreTimer()

    ble $v0,$t1,del_while          #   while (i<=counter) {
    
    jr  $ra  