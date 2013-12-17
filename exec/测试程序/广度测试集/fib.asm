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
string1	DB "Please enter:n",0DH,0AH,0
string2	DB "Fib(n) is: ",0DH,0AH,0

	.CODE

;Begin	1		

id1_fib	PROC near32
push	ebp
mov	ebp,esp
sub	esp,32
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-4],eax

;=	k	0	#t1
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,0
mov	dword ptr [ebp-8],1
cmp	eax,ebx
je	@judge_flag1
mov	dword ptr [ebp-8],0
@judge_flag1:

;IfFalse	#t1	@flag_flag1	
mov	eax,dword ptr [ebp-8]
cmp	eax,0
je	@flag_flag1

;:=	0		fib
mov	eax,0
mov	esi,dword ptr [ebp+12]
sub	esi,12
mov	dword ptr [esi],eax

;JmpTo	@flag_flag2		
jmp	@flag_flag2

;SetFlag	@flag_flag1		
@flag_flag1:

;=	k	1	#t2
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,1
mov	dword ptr [ebp-12],1
cmp	eax,ebx
je	@judge_flag2
mov	dword ptr [ebp-12],0
@judge_flag2:

;IfFalse	#t2	@flag_flag3	
mov	eax,dword ptr [ebp-12]
cmp	eax,0
je	@flag_flag3

;:=	1		fib
mov	eax,1
mov	esi,dword ptr [ebp+12]
sub	esi,12
mov	dword ptr [esi],eax

;JmpTo	@flag_flag4		
jmp	@flag_flag4

;SetFlag	@flag_flag3		
@flag_flag3:

;Display	1	1	
push	[ebp+12]

;-	k	1	#t3
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-16],eax

;value=	#t3	k	1
lea	eax,dword ptr [ebp-16]
push	eax

;CallValue	1		#t4
call	id1_fib
add	esp,8
mov	esi,dword ptr [ebp+12]
sub	esi,12
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-20],eax

;Display	1	1	
push	[ebp+12]

;-	k	2	#t5
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,2
sub	eax,ebx
mov	dword ptr [ebp-24],eax

;value=	#t5	k	1
lea	eax,dword ptr [ebp-24]
push	eax

;CallValue	1		#t6
call	id1_fib
add	esp,8
mov	esi,dword ptr [ebp+12]
sub	esi,12
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-28],eax

;+	#t4	#t6	#t7
mov	eax,dword ptr [ebp-20]
mov	ebx,dword ptr [ebp-28]
add	eax,ebx
mov	dword ptr [ebp-32],eax

;:=	#t7		fib
mov	eax,dword ptr [ebp-32]
mov	esi,dword ptr [ebp+12]
sub	esi,12
mov	dword ptr [esi],eax

;SetFlag	@flag_flag4		
@flag_flag4:

;SetFlag	@flag_flag2		
@flag_flag2:

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_fib	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,16
push	ebx
push	esi
push	edi

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;Read	n		
lea	eax,dword ptr [ebp-8]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;WriteString	string2		
lea	ecx,string2
push	ecx
call	printf
add	esp,4

;Display	0	1	
push	ebp

;value=	n	k	1
lea	eax,dword ptr [ebp-8]
push	eax

;CallValue	1		#t8
call	id1_fib
add	esp,8
mov	eax,dword ptr [ebp-12]
mov	dword ptr [ebp-16],eax

;:=	#t8		result
mov	eax,dword ptr [ebp-16]
mov	dword ptr [ebp-4],eax

;WriteExpression	result		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;End	0		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
