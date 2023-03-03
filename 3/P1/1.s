    .data
    .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area 
    .equ TRISE, 0x6100   # TRISE address is 0xBF886100 
    .equ PORTE, 0x6110   # PORTE address is 0xBF886110 
    .equ LATE,  0x6120   # LATE  address is 0xBF886120

    .equ TRISB, 0x6040   # TRISB address is 0xBF886040
    .equ PORTB, 0x6050   # PORTB address is 0xBF886050
    .equ LATB,  0x6060   # LATB  address is 0xBF886060
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


    # configure port RB0 as input

    # READ  (Mem_addr = 0xBF880000 + 0x6040)
    lw  $t2, TRISB($t1) 
    # MODIFY (bit0=1 (1 means INPUT))
    andi  $t2, $t2, 0x0001 
    # WRITE (Write TRISB register)
    sw  $t2, TRISB($t1)

while:
    # RE0 = RB0

    lw $t2, PORTB($t1)      # READ PORTB
    andi $t2, $t2, 0x0001   # get bit0 value

    lw $t3, PORTE($t1)      # READ PORTE
    andi $t3, $t3, 0xFFFE   # clear bit0
    or $t3, $t3, $t2        # bit0=RB0
    sw $t3, PORTE($t1)      # WRITE to PORTE

    j while