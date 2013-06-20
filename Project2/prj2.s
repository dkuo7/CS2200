!$zero should be translated to the register which is hardwired to 0
!$sp should be translated to your stack pointer register
!$r# should be translated to your choice of non specific use registers 
!	such that the ones with the same name are the same register
!	and those witth different names are different
!.fill xxx means to put xxx at that address
!name: meant this line is labeled 'name'
!using name in an offset later means the address of that line
! the number at the beginning of each line is the address of that line in hex


! for your convenience 

main:
0	addi $sp, $zero, initsp ! initialize the stack pointer
4	lw $sp, 0($sp)
8	ei 			!Don't forget to enable interrupts...

		
C	addi $r1, $zero, 100	!load itteration count
10	addi $r0, $zero, loop
14 loop:			!this and the next line are the same address since labels dont take up a spot
14	lw $r3, $r1, 0		!should be an unaligned access 3/4 of the time
18	addi $r1, $r1, -1
1C	beq $zero, $r1, +4
20	jalr $r0, $zero		!remember this means the target is r0
	!beq target	
24	halt			!shold have caused 75 errors
	

28 initsp: .fill 0xA00000
		
		
2C bad_access: .fill 0
30 mem_inthandler:
	!FIX ME
	!should record a misaligned access with maximum interruptable time
	!that is do all the overhead
	!and then increment address 2C
	! Fun Tip: If your offsets to sw/lw are atleast 9 bits you can say
	!	   sw/lw $r#, $zero, 2C 
	!	   (access at an offset of 2C from 0 will come out to 2C)
	!	   to save instructions which means less work for you
34	addi $sp, $sp, -4	
38	sw $k0, 0($sp)
3C	ei
40	addi $sp, $sp, -4
44	sw $r1, 0($sp)
48	lw $r1, $zero, 2C
4C	addi $r1, $r1, 1
50	sw $r1, $zero, 2C
54	lw $r1, 0($sp)
58	addi $sp, $sp, 4
5C	di
60	lw $k0, 0($sp)
64	addi $sp, $sp, 4
68	rti

	
	
	
	
	
	
