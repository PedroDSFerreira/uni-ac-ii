	.data

	.text
	.globl main
main:
do:
	li $v0,1
	syscall		# inkey()
	
	move $a0, $v0

if: 
	beq $a0, $0, else

	li $v0, 3
	syscall		# putChar()
	
	j while

else:
	la $a0, '.'
	li $v0, 3
	syscall 	# putChar('.')


while:
	bne $a0, '\n', do
end:	
	li $a0,0
	li $v0,10
	syscall
