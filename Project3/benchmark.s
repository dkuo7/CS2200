Multiply numbers by 4 for a byte addressed memory
this includes the instruction numbering in the left margin and
all offsets to memory or the stack pointer

If and only if two registers have the same name 
then they are assembled as the same register

the style is as in the book
$sp is the stack pointer
$ra is the return address register
$at is just another register 
(if there were an assembler it would be the reserved for assembler)
$v# are return value registers
$a# are argument registers
$zero is the register which is hard wired to be 0

main:                            Imm (assuming 8 bit, please sign extend)
00        ldadr $sp, initsp      0x29   ! initialize the stack pointer
01        lw $sp, 0($sp)                ! finish initialization
                
02        ldadr $a0, BASE        0x08   !load base for pow
03        lw $a0, 0($a0)
04        ldadr $a1, EXP         0x07   !load power for pow
05        lw $a1, 0($a1)
06        ldadr $at, POW         0x07   !load address of pow
07        jalr $at, $ra                 !run pow
08        ldadr $a0, ANS         0x04   !load base for pow
09        sw $v0, 0($a0)
                
0A        halt        
        
0B        BASE: .fill 2
0C        EXP: .fill 16
0D        ANS: .fill 0                    ! should come out to 65536
        
POW: 
0E  addi $sp, $sp, 2                      ! push 2 slots onto the stack
0F  sw $ra, -1($sp)                       ! save RA to stack
10  sw $a0, -2($sp)                       ! save arg 0 to stack
11  beq $zero, $a1, RET1        0x0B      ! if the power is 0 return 1
12  beq $zero, $a0, RET0        0x0D      ! if the base is 0 return 0
13  addi $a1, $a1, -1                     ! decrement the power
14  addi $at, $zero, POW        0x0E      ! load the address of POW
15  jalr $at, $ra                         ! recursively call POW
16  add $a0, $v0, $zero                   ! store return value in arg 1
17  lw $a1, -2($sp)                       ! load the base into arg 0
18  addi $at, $zero, MULT       0x23      ! load the address of MULT
19  jalr $at, $ra                         ! multiply arg 0 (base) and arg 1 (running product)
1A  lw $ra, -1($sp)                       ! load RA from the stack
1B  addi $sp, $sp, -2                     ! pop the RA and arg 0 off the stack
1C  jalr $ra, $zero                       ! return
1D RET1: addi $v0, $zero, 1               ! return a value of 1
1E  addi $sp, $sp, -2
1F  jalr $ra, $zero
20 RET0: add $v0, $zero, $zero            ! return a value of 0
21  addi $sp, $sp, -2
22  jalr $ra, $zero                
        
23 MULT: add $v0, $zero, $zero            ! zero out return value
24 AGAIN: add $v0,$v0, $a0                ! multiply loop
25  nand $a2, $zero, $zero
26  add $a1, $a1, $a2
27  beq $a1, $zero, DONE        0x01      ! finished multiplying
28  beq $zero, $zero, AGAIN     0xFB (-5) ! loop again
29 DONE: jalr $ra, $zero        

2a initsp: .fill 0xA00000
