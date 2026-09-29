  # hexmain.S
  # Written 2015-09-04 by F Lundevall, Arvid Björne, Theodor Poli
  # Copyright abandonded - this file is in the public domain.

	.text
	# Declare hexasc global too so it can be called from another file
	# (e.g. timetemplate.S, or the auto-grader's test harness).
	.globl hex2asc, hexasc

hex2asc:
	li	a0, 36	# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
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
	
