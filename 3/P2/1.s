    .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area 
    .equ TRISE, 0x6100   # TRISE address is 0xBF886100 
    .equ PORTE, 0x6110   # PORTE address is 0xBF886110 
    .equ LATE,  0x6120   # LATE  address is 0xBF886120
    .equ READ_CORE_TIMER,11 
    .equ RESET_CORE_TIMER,12
 
    .data
string:     .asciiz "\n"
    .text
    .globl main

main:
    lui   $s0, SFR_BASE_HI # $s0=0xBF880000

    # configure port RE4-1 as output

    # READ  (Mem_addr = 0xBF880000 + 0x6100)
    lw  $t2, TRISE($s0) 
    # MODIFY bit4-1=0 (0 means OUTPUT))
    andi  $t2, $t2, 0xFFE1
    # WRITE (Write TRISE register)
    sw  $t2, TRISE($s0) 

    li $s1, 0           # counter

while:

    lw $t3, LATE($s0)      # READ LATE
    andi $t3, $t3, 0xFFE1   # clear bit4-1
    or $t3, $t3, $s1        # OR with counter
    sw $t3, LATE($s0)      # WRITE to LATE

    li $a0, 1000             # 1000ms = 1Hz

    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    jal delay
    lw $ra, 0($sp)
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addi $sp, $sp, 12

    addi $s1, $s1, 1        # increment counter

    j while



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