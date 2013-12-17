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

;Begin	1		

id1_jiecheng	PROC near32
push	ebp
mov	ebp,esp
sub	esp,20
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-4],eax

;<=	i	1	#t1
mov	eax,dword ptr [ebp-4]
mov	ebx,1
mov	dword ptr [ebp-8],1
cmp	eax,ebx
jle	@judge_flag1
mov	dword ptr [ebp-8],0
@judge_flag1:

;IfFalse	#t1	@flag_flag1	
mov	eax,dword ptr [ebp-8]
cmp	eax,0
je	@flag_flag1

;:=	1		jiecheng
mov	eax,1
mov	esi,dword ptr [ebp+12]
sub	esi,8
mov	dword ptr [esi],eax

;JmpTo	@flag_flag2		
jmp	@flag_flag2

;SetFlag	@flag_flag1		
@flag_flag1:

;-	i	1	#t2
mov	eax,dword ptr [ebp-4]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-12],eax

;Display	1	1	
push	[ebp+12]

;value=	#t2	i	1
mov	eax,dword ptr [ebp-12]
push	eax

;CallValue	1		#t3
call	id1_jiecheng
add	esp,8
mov	esi,dword ptr [ebp+12]
sub	esi,8
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-16],eax

;*	i	#t3	#t4
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-16]
imul	eax,ebx
mov	dword ptr [ebp-20],eax

;:=	#t4		jiecheng
mov	eax,dword ptr [ebp-20]
mov	esi,dword ptr [ebp+12]
sub	esi,8
mov	dword ptr [esi],eax

;SetFlag	@flag_flag2		
@flag_flag2:

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_jiecheng	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;Read	x		
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;Display	0	1	
push	ebp

;value=	x	i	1
mov	eax,dword ptr [ebp-4]
push	eax

;CallValue	1		#t5
call	id1_jiecheng
add	esp,8
mov	eax,dword ptr [ebp-8]
mov	dword ptr [ebp-12],eax

;:=	#t5		x
mov	eax,dword ptr [ebp-12]
mov	dword ptr [ebp-4],eax

;WriteExpression	x		
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
