    .data
    .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area 
    .equ TRISE, 0x6100   # TRISE address is 0xBF886100 
    .equ PORTE, 0x6110   # PORTE address is 0xBF886110 
    .equ LATE,  0x6120   # LATE  address is 0xBF886120

    .equ TRISD, 0x60C0   # TRISD address is 0xBF8860C0
    .equ PORTD, 0x60D0   # PORTD address is 0xBF8860D0
    .equ LATD,  0x60E0   # LATD  address is 0xBF8860E0
    .text
    .globl main

main:
    lui   $t1, SFR_BASE_HI # $t1=0xBF880000

    # configure port RE0 as output

    # READ  (Mem_addr = 0xBF880000 + 0x6100)
    lw  $t2, TRISE($t1) 
    # MODIFY bit0=0 (0 means OUTPUT))
    andi  $t2, $t2, 0xFFFE
    # WRITE (Write TRISE register)
    sw  $t2, TRISE($t1) 


    # configure port RD8 as input

    # READ  (Mem_addr = 0xBF880000 + 0x60C0)
    lw  $t2, TRISD($t1) 
    # MODIFY (bit8=1 (1 means INPUT))
    andi  $t2, $t2, 0x0010 
    # WRITE (Write TRISD register)
    sw  $t2, TRISD($t1)

while:
    # RE0 = RD8

    lw $t2, PORTD($t1)      # READ PORTD
    andi $t2, $t2, 0x0010   # get bit8 value
    srl $t2, $t2, 4         # bit8 -> bit0

    xor $t2, $t2, 0x0001   # invert bit0

    lw $t3, LATE($t1)      # READ LATE
    andi $t3, $t3, 0xFFFE   # clear bit0
    or $t3, $t3, $t2        # bit0=RD8(bit0)
    sw $t3, LATE($t1)      # WRITE to LATE

    j while