# timetemplate.S
# Written 2015 by F Lundevall, Arvid Björne, Theodor Poli
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# PUSH / POP macros.                                        #
#                                                           #
# RARS and the GNU assembler (used for the DTEK-V board)    #
# use slightly different macro syntax. The file ships with  #
# the RARS-style block active, because Assignments 1-4 are  #
# done in RARS. When you port to the board in Assignment 5, #
# comment out the RARS block below and uncomment the DTEK   #
# block beneath it.                                         #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp)
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp)
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	# delay is declared global so the auto-grader's test harness (which
	# calls delay directly from a small labmain.S) can resolve the symbol.
	.globl timetemplate, tick, time2string, delay, display_string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hexasc, delay. #
#########################################################
time2string:
	PUSH	ra
	PUSH	s0
	mv	s0, a0 #Paste content of a0 in to s0 to make it easier, since a0 is altered a lot below
			#So s0 holds the memory adress
	
	srli	a0, a1, 12 #Shifts bit 12-15 (first minute digit) down to the LSB 
	jal	hexasc
	sb	a0, 0(s0)	
	
	srli 	a0, a1, 8 #Shifts bit 8-11 (Second minute digit) down to the LSB
	jal	hexasc
	sb	a0, 1(s0)
	
	li	t0, 0x3A # : in ascii
	sb	t0, 2(s0) #Store :
	
	srli 	a0, a1, 4 #Shifts bit 4-7 (first second digit) down to the LSB
	jal	hexasc
	sb	a0, 3(s0)
	
	addi	a0, a1, 0 #last byte (second second digit) is already the LSB, dont need to shift
	jal	hexasc   
	sb	a0, 4(s0)
	
	li	t1, 0x00 # Null byte
	sb	t1, 5(s0) #Store nullbyte
	
	POP s0
	POP ra
	
	jr	ra

hexasc:
	li	t0, 0x30 #Set t0 to 0 in ascii

	andi	a0, a0, 0xF #Mask out everything except the LSB. So we get 0000 0000 0000 XXXX
	
	li	t1, 10 #We want to commpare with the value 10
	bge	a0, t1, letter
	
	add	a0, a0, t0
	
	jr 	ra
	
	letter:
		addi	t0, t0, 0x7
		add	a0, a0, t0
		jr	ra
	
			
delay:
	
	blez	a0, done
	
	li	t0, 1
	sub	a0, a0, t0
	
	
	li	t1, 0
	li	a1, 15000 #4711
	inner:
		addi	t1, t1, 1
		ble	t1, a1, inner
		
	
	j	delay
	
	done:
		jr	ra
		

	
