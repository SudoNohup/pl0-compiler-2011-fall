	.386
	.model flat,stdcall

option	casemap:none
includelib	msvcrt.lib
printf	proto C:dword,:vararg
scanf	proto C:dword,:vararg
	.STACK	4096*16

	.DATA
inint 	DB "%d",0
outint 	DB "%d",0DH,0AH,0
inchar 	DB " %c",0
outchar 	DB "%c",0DH,0AH,0

	.CODE

;Begin	2		

id2_mod	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi

;/	x	y	#t1
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,16
mov	ebx,dword ptr [esi]
cdq
idiv	ebx
mov	dword ptr [ebp-4],eax

;*	#t1	y	#t2
mov	eax,dword ptr [ebp-4]
mov	esi,dword ptr [ebp+8]
sub	esi,16
mov	ebx,dword ptr [esi]
imul	eax,ebx
mov	dword ptr [ebp-8],eax

;-	x	#t2	#t3
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	eax,dword ptr [esi]
mov	ebx,dword ptr [ebp-8]
sub	eax,ebx
mov	dword ptr [ebp-12],eax

;:=	#t3		x
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	dword ptr [esi],eax

;End	2		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_mod	ENDP

;Begin	1		

id1_prime	PROC near32
push	ebp
mov	ebp,esp
sub	esp,28
push	ebx
push	esi
push	edi

;:=	true		f
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-8],eax

;:=	2		i
mov	eax,2
mov	dword ptr [ebp-4],eax

;-	m	1	#t4
mov	esi,dword ptr [ebp+8]
sub	esi,20
mov	eax,dword ptr [esi]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-16],eax

;SetFlag	flag_flag1		
flag_flag1:

;<=	i	#t4	#t5
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-16]
mov	dword ptr [ebp-20],1
cmp	eax,ebx
jle	judge_flag1
mov	dword ptr [ebp-20],0
judge_flag1:

;IfFalse	#t5	flag_flag2	
mov	eax,dword ptr [ebp-20]
cmp	eax,0
je	flag_flag2

;:=	m		x
mov	esi,dword ptr [ebp+8]
sub	esi,20
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	dword ptr [esi],eax

;:=	i		y
mov	eax,dword ptr [ebp-4]
mov	esi,dword ptr [ebp+8]
sub	esi,16
mov	dword ptr [esi],eax

;Display	1	2	
push	ebp
push	[ebp+8]

;Call	2		
call	id2_mod
add	esp,8

;=	x	0	#t6
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	eax,dword ptr [esi]
mov	ebx,0
mov	dword ptr [ebp-24],1
cmp	eax,ebx
je	judge_flag2
mov	dword ptr [ebp-24],0
judge_flag2:

;IfFalse	#t6	flag_flag3	
mov	eax,dword ptr [ebp-24]
cmp	eax,0
je	flag_flag3

;:=	false		f
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-8],eax

;Loop	flag_flag4		
jmp	flag_flag4

;SetFlag	flag_flag3		
flag_flag3:

;SetFlag	flag_flag4		
flag_flag4:

;+	i	1	i
mov	eax,dword ptr [ebp-4]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-4],eax

;Loop	flag_flag1		
jmp	flag_flag1

;SetFlag	flag_flag2		
flag_flag2:

;=	f	true	#t7
mov	eax,dword ptr [ebp-8]
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	ebx,dword ptr [esi]
mov	dword ptr [ebp-28],1
cmp	eax,ebx
je	judge_flag3
mov	dword ptr [ebp-28],0
judge_flag3:

;IfFalse	#t7	flag_flag5	
mov	eax,dword ptr [ebp-28]
cmp	eax,0
je	flag_flag5

;WriteExpression	m		
mov	esi,dword ptr [ebp+8]
sub	esi,20
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;:=	true		pf
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,28
mov	dword ptr [esi],eax

;Loop	flag_flag6		
jmp	flag_flag6

;SetFlag	flag_flag5		
flag_flag5:

;SetFlag	flag_flag6		
flag_flag6:

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_prime	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,44
push	ebx
push	esi
push	edi
mov	dword ptr [ebp-4],1
mov	dword ptr [ebp-8],0

;:=	false		pf
mov	eax,dword ptr [ebp-8]
mov	dword ptr [ebp-28],eax

;:=	100		n
mov	eax,100
mov	dword ptr [ebp-24],eax

;=	n	2	#t8
mov	eax,dword ptr [ebp-24]
mov	ebx,2
mov	dword ptr [ebp-36],1
cmp	eax,ebx
je	judge_flag4
mov	dword ptr [ebp-36],0
judge_flag4:

;IfFalse	#t8	flag_flag7	
mov	eax,dword ptr [ebp-36]
cmp	eax,0
je	flag_flag7

;:=	true		pf
mov	eax,dword ptr [ebp-4]
mov	dword ptr [ebp-28],eax

;WriteExpression	n		
mov	eax,dword ptr [ebp-24]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;Loop	flag_flag8		
jmp	flag_flag8

;SetFlag	flag_flag7		
flag_flag7:

;SetFlag	flag_flag8		
flag_flag8:

;:=	3		m
mov	eax,3
mov	dword ptr [ebp-20],eax

;SetFlag	flag_flag9		
flag_flag9:

;<=	m	n	#t9
mov	eax,dword ptr [ebp-20]
mov	ebx,dword ptr [ebp-24]
mov	dword ptr [ebp-40],1
cmp	eax,ebx
jle	judge_flag5
mov	dword ptr [ebp-40],0
judge_flag5:

;IfFalse	#t9	flag_flag10	
mov	eax,dword ptr [ebp-40]
cmp	eax,0
je	flag_flag10

;Display	0	1	
push	ebp

;Call	1		
call	id1_prime
add	esp,4

;+	m	1	m
mov	eax,dword ptr [ebp-20]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-20],eax

;Loop	flag_flag9		
jmp	flag_flag9

;SetFlag	flag_flag10		
flag_flag10:

;=	pf	false	#t10
mov	eax,dword ptr [ebp-28]
mov	ebx,dword ptr [ebp-8]
mov	dword ptr [ebp-44],1
cmp	eax,ebx
je	judge_flag6
mov	dword ptr [ebp-44],0
judge_flag6:

;IfFalse	#t10	flag_flag11	
mov	eax,dword ptr [ebp-44]
cmp	eax,0
je	flag_flag11

;WriteExpression	0		
mov	eax,0
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;Loop	flag_flag12		
jmp	flag_flag12

;SetFlag	flag_flag11		
flag_flag11:

;SetFlag	flag_flag12		
flag_flag12:

;End	0		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
