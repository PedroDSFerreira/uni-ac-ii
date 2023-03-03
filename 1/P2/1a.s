	.data

	.text
	.globl main
main:
do:
	li $v0,2
	syscall		# getChar()
	
	move $a0, $v0

	li $v0,3
	syscall		# putChar()
while:
	bne $a0, '\n', do
end:	
	li $a0,0
	li $v0,10
	syscall
