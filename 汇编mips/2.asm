		.data
buf1:		.space 1000

buf2:		.space 4000
		
mess1:	.asciiz "input numbers:"
mess2:	.asciiz "\n the total numbers is:"
mess3:	.asciiz "\n the numbers you input is:"
mess4:	.asciiz "  "
mess5:	.asciiz "\n the numbers sorted is:"
		.macro exchange(%a,%b)
		move $at,%a
		move %a,%b
		move %b,$at
		.end_macro
		
		.text
main:		li $v0,8
		la $a0,buf1
		li $a1,1000
		syscall		#输入字符串
		jal gaink		#调用函数得到字符串的长度
		jal shift		#提取并转化为数字
		jal output1		#输出数字和个数
		jal sortarray	#排序
		jal output2		#输出排序后的数据
		li $v0,10
		syscall

gaink:		li $t1,0		#t1=0
		la $a1,buf1
loop1:		add $t2,$a1,$t1	#t2指向a[t1]
		lb $s1,0($t2)	#s1=a[t1]
		beqz $s1,back
		addiu $t1,$t1,1	#t1++
		j loop1		
back:		#li $v0,1		#检测字符串的长度是否有误
		#move $a0,$t1
		#syscall 
		jr $ra		

shift:		addi $t1,$t1,-1	#t1=t1-1  --i=len-1
		li $t3,0		#t3--j=0
		li $t4,1		#t4--t=1
		la $a2,buf2		#a2指向数组b
loop2:		add $t2,$a1,$t1	#t2指向a[i]
		lb $t5,0($t2)	#t5=a[i]
		blt $t5,48,next1
		bgt $t5,57,next1
		sll $t8,$t3,2
		add $t6,$a2,$t8	#t6指向b[j]
		lw $t7,0($t6)	#t7=b[j]
		addi $t7,$t5,-48	
		sw $t7,0($t6)	#b[j]=a[i]-'0'
loop3:		addi $s2,$t1,-1	#s2=i-1
		add $s3,$a1,$s2	#s3指向a[i-1]
		lb $s4,0($s3)	#s4=a[i-1]
		bltz $t1,next2	#循环条件
		blt  $s4,48,next2
		bgt  $s4,57,next2
		mul $t4,$t4,10	#t*=10
		addi $s5,$s4,-48
		mul $s5,$s5,$t4
		add $t7,$t7,$s5	#b[j]+=t*(a[i-1]-'0')
		sw $t7,0($t6)	#存入b[j]
		addi $t1,$t1,-1	#i--
		j loop3
next2:		addi $t3,$t3,1	#j++
		li $t4,1	#t=1
		bne $s4,45,next1  #如果为负数‘-’	
		mul $t7,$t7,-1	#乘以-1
		sw $t7,0($t6)
next1:		addi $t1,$t1,-1 			
		bgez $t1,loop2
		jr $ra

output1:	addi $t1,$t3,-1	#i=j-1
		li $v0,4
		la $a0,mess2
		syscall
		li $v0,1
		move $a0,$t3
		syscall		#输出数字个数
		li  $v0,4
		la $a0,mess3		
		syscall
loop4:		bltz $t1,next3
		sll $t4,$t1,2	#t4=i*4
		add $t6,$a2,$t4
		lw $t7,0($t6)	#t7=b[i]
		li $v0,1
		move $a0,$t7
		syscall
		li $v0,4
		la $a0,mess4
		syscall
		addi $t1,$t1,-1	#i--
		j loop4
next3:		jr $ra


sortarray:	li $t1,0		#t1--i=0
loop5:		bge $t1,$t3,next4
		li $t4,0		#t4--t=0
loop6:		sub $t5,$t3,$t1
		addi $t5,$t5,-1	#t5=j-i-1
		bge $t4,$t5,next5
		sll $t6,$t4,2	#t6=t*4
		add $t6,$t6,$a2	#t6指向b[t]
		lw $t7,0($t6)	#t7=b[t]	
		sll $t9,$t4,2
		addi $t9,$t9,4	
		add $t9,$t9,$a2	#t9指向b[t+1]
		lw  $t2,0($t9)	#t2=b[t+1]
		bge $t7,$t2,next6	
		exchange($t2,$t7)	#交换b[t]与b[t+1]
		sw $t2,0($t9)
		sw $t7,0($t6)

next6:		addi $t4,$t4,1	#t++
		j loop6
next5:		addi $t1,$t1,1	#i++
		j loop5
next4:		jr $ra


output2:	li $t1,0
		li $v0,4
		la $a0,mess5
		syscall
loop7:		bge $t1,$t3,next7
		sll $t6,$t1,2	#t6=i*4
		add $t6,$t6,$a2	#指向b[i]
		lw $t7,0($t6)	#t7=b[i]
		li $v0,1
		move $a0,$t7
		syscall
		li $v0,4
		la $a0,mess4
		syscall
		addiu $t1,$t1,1	#i++
		j loop7
next7:		jr $ra		
