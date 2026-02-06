       .data
prime: 	 	.space 14000
moreout: 	.asciiz"Out of range"
words1:  	 .asciiz "\n input a number:"
words2: 	 .asciiz "\n next prime is:"
words3: 	 .asciiz "\n prime numbers is:"
enter:  	 .asciiz"\n"
		 .align 2
        	 .globl main
        	 .text
main:       	 la $a2,prime   #a2指向素数表
        	li $v1,2
         	sh $v1,0($a2)	#a[0]=2
        	li $v1,3
         	sh $v1,2($a2)	#a[1]=3
		jal createprime  #创建素数表函数
input:     	li $v0,4
           	la $a0,words1
        	syscall
      	 	li $v0,5
      		syscall
     		move $a3,$v0   #a3=n
     		bgtu $a3,2147483647,more
     	 	bleu $a3,1,end   #判断是否大于1
      		jal nextprime      #下一个素数
      		j input
end:  		li $v0 10
      		syscall
more:		li $v0,4
		la $a0,moreout
		syscall
		j input    
createprime:  li $t1,5   #t1--i
              li $t2,1   #t2--j             
loop1:        li $t3,3   #t3--k
              bgeu $t1,65536,ret1  #i>=65538跳出函数
loop2:        mult $t3,$t3  
              mflo $t4  #t4=k^2          
              bgtu $t4,$t1,exitloop2 #k^2>i跳出内循环
              rem $t5,$t1,$t3    #t5=i%k
              beqz $t5,exitloop2 #t5=0就break
              addi $t3,$t3,2     #k+=2
              j loop2           #跳回内循环
             
 ret1:       li $v0,4
             la $a0,words3
             syscall        
            
             li $v0,1
             move $a0,$t2
             syscall
             jr $ra  
 exitloop2:  
              bleu $t4,$t1,next
	      addi $t2,$t2,1  #j++
              sll $t6,$t2,1   #t6=j*2
              add $t6,$a2,$t6 #t6指向prime[j]位置
              sh  $t1,0($t6)   #prime[j]=i            		  
 next:        addi $t1,$t1,2  #i+=2
              j loop1         #跳到外循环

nextprime:    
              #sll $t6,$t2,1   #t6=j*2
              #add $t6,$a2,$t6 #t6指向prime[j]位置
              #lhu  $t7,0($t6)  #t7=prime[j]
              bgtu $a3,65521,else #比较n与prime[j]
              li  $t8,0
loop3:        bgtu $t8,$t2,end1
              sll $t9,$t8,1  #t9=t*2
              add $t9,$a2,$t9 #t9指向prime[t]位置
              lh  $s1,0($t9)  #s1=prime[t]
              bgt $a3,$s1,addtt
              li $v0,4
              la $a0,words2
              syscall
              li $v0,1       #打印n
               move $a0,$s1
              syscall
              li $v0,4       #打印空格
              la $a0,enter
              syscall
              jr $ra         #打印后返回
addtt:        addi $t8,$t8,1  #t++   
              j loop3          

else:         li  $t8,0     #t=0
	      move $t7,$a3  #t7=n
  loop4:      bgtu $t8,$t2,output2
              sll $t9,$t8,1  #t9=t*2
              add $t9,$a2,$t9 #t9指向prime[t]位置
              lh  $s1,0($t9)  #s1=prime[t]
             # rem $s1,$a0,$s1  #n%prime[t]
              div $t7,$s1
              mfhi $s1
              bnez $s1,addt
              li $t8,-1        #t=0
              addi $t7,$t7,1  #q++
addt:         addi $t8,$t8,1  #t++
              j loop4         #跳回循环

output2:      li $v0,4
              la $a0,words2
              syscall
              li $v0,1    #输出n的值
              move $a0,$t7
              syscall 
              li $v0,4    #打印空格
              la $a0,enter
              syscall
              jr $ra   
end1:         j output2