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
string1	DB "input x : ",0DH,0AH,0

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
sub	esp,36
push	ebx
push	esi
push	edi

;:=	true		f
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-8],eax

;-	m	1	#t4
mov	esi,dword ptr [ebp+8]
sub	esi,20
mov	eax,dword ptr [esi]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-20],eax

;:=	#t4		k
mov	eax,dword ptr [ebp-20]
mov	dword ptr [ebp-12],eax

;:=	3		i
mov	eax,3
mov	dword ptr [ebp-4],eax

;SetFlag	@flag_flag1		
@flag_flag1:

;<=	i	k	#t5
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-12]
mov	dword ptr [ebp-24],1
cmp	eax,ebx
jle	@judge_flag1
mov	dword ptr [ebp-24],0
@judge_flag1:

;IfFalse	#t5	@flag_flag2	
mov	eax,dword ptr [ebp-24]
cmp	eax,0
je	@flag_flag2

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
mov	dword ptr [ebp-28],1
cmp	eax,ebx
je	@judge_flag2
mov	dword ptr [ebp-28],0
@judge_flag2:

;IfFalse	#t6	@flag_flag3	
mov	eax,dword ptr [ebp-28]
cmp	eax,0
je	@flag_flag3

;:=	false		f
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-8],eax

;JmpTo	@flag_flag4		
jmp	@flag_flag4

;SetFlag	@flag_flag3		
@flag_flag3:

;SetFlag	@flag_flag4		
@flag_flag4:

;+	i	1	#t7
mov	eax,dword ptr [ebp-4]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-32],eax

;:=	#t7		i
mov	eax,dword ptr [ebp-32]
mov	dword ptr [ebp-4],eax

;+	i	1	i
mov	eax,dword ptr [ebp-4]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-4],eax

;JmpTo	@flag_flag1		
jmp	@flag_flag1

;SetFlag	@flag_flag2		
@flag_flag2:

;=	f	true	#t8
mov	eax,dword ptr [ebp-8]
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	ebx,dword ptr [esi]
mov	dword ptr [ebp-36],1
cmp	eax,ebx
je	@judge_flag3
mov	dword ptr [ebp-36],0
@judge_flag3:

;IfFalse	#t8	@flag_flag5	
mov	eax,dword ptr [ebp-36]
cmp	eax,0
je	@flag_flag5

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

;JmpTo	@flag_flag6		
jmp	@flag_flag6

;SetFlag	@flag_flag5		
@flag_flag5:

;SetFlag	@flag_flag6		
@flag_flag6:

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
sub	esp,56
push	ebx
push	esi
push	edi
mov	dword ptr [ebp-4],1
mov	dword ptr [ebp-8],0

;:=	false		pf
mov	eax,dword ptr [ebp-8]
mov	dword ptr [ebp-28],eax

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;Read	n		
lea	eax,dword ptr [ebp-24]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;=	n	2	#t9
mov	eax,dword ptr [ebp-24]
mov	ebx,2
mov	dword ptr [ebp-40],1
cmp	eax,ebx
je	@judge_flag4
mov	dword ptr [ebp-40],0
@judge_flag4:

;IfFalse	#t9	@flag_flag7	
mov	eax,dword ptr [ebp-40]
cmp	eax,0
je	@flag_flag7

;:=	true		pf
mov	eax,dword ptr [ebp-4]
mov	dword ptr [ebp-28],eax

;JmpTo	@flag_flag8		
jmp	@flag_flag8

;SetFlag	@flag_flag7		
@flag_flag7:

;SetFlag	@flag_flag8		
@flag_flag8:

;:=	2		t
mov	eax,2
mov	dword ptr [ebp-32],eax

;WriteExpression	t		
mov	eax,dword ptr [ebp-32]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;:=	3		m
mov	eax,3
mov	dword ptr [ebp-20],eax

;SetFlag	@flag_flag9		
@flag_flag9:

;<=	m	n	#t10
mov	eax,dword ptr [ebp-20]
mov	ebx,dword ptr [ebp-24]
mov	dword ptr [ebp-44],1
cmp	eax,ebx
jle	@judge_flag5
mov	dword ptr [ebp-44],0
@judge_flag5:

;IfFalse	#t10	@flag_flag10	
mov	eax,dword ptr [ebp-44]
cmp	eax,0
je	@flag_flag10

;Display	0	1	
push	ebp

;Call	1		
call	id1_prime
add	esp,4

;+	m	1	#t11
mov	eax,dword ptr [ebp-20]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-48],eax

;:=	#t11		m
mov	eax,dword ptr [ebp-48]
mov	dword ptr [ebp-20],eax

;+	m	1	m
mov	eax,dword ptr [ebp-20]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-20],eax

;JmpTo	@flag_flag9		
jmp	@flag_flag9

;SetFlag	@flag_flag10		
@flag_flag10:

;=	pf	false	#t12
mov	eax,dword ptr [ebp-28]
mov	ebx,dword ptr [ebp-8]
mov	dword ptr [ebp-52],1
cmp	eax,ebx
je	@judge_flag6
mov	dword ptr [ebp-52],0
@judge_flag6:

;IfFalse	#t12	@flag_flag11	
mov	eax,dword ptr [ebp-52]
cmp	eax,0
je	@flag_flag11

;:=	0		m
mov	eax,0
mov	dword ptr [ebp-20],eax

;WriteExpression	m		
mov	eax,dword ptr [ebp-20]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;JmpTo	@flag_flag12		
jmp	@flag_flag12

;SetFlag	@flag_flag11		
@flag_flag11:

;-	0	2	#t13
mov	eax,0
mov	ebx,2
sub	eax,ebx
mov	dword ptr [ebp-56],eax

;:=	#t13		m
mov	eax,dword ptr [ebp-56]
mov	dword ptr [ebp-20],eax

;WriteExpression	m		
mov	eax,dword ptr [ebp-20]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;SetFlag	@flag_flag12		
@flag_flag12:

;End	0		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
