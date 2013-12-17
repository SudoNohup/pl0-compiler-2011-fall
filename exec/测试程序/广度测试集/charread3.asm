	.386
	.model flat,stdcall

option	casemap:none
includelib	E:\masm32\lib\msvcrt.lib
printf	proto C:dword,:vararg
scanf	proto C:dword,:vararg
	.STACK	4096*16

	.DATA
inint 	DB "%d",0
outint 	DB "%d",0DH,0AH,0
inchar 	DB " %c",0
outchar 	DB "%c",0DH,0AH,0
arrayerror 	DB "array out of range",0DH,0AH,0
string1	DB "invalid",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,40
mov	dword ptr [ebp-4],97
mov	dword ptr [ebp-8],109

;Read	c1		
lea	esi,dword ptr [ebp-12]
xor	eax,eax
mov dword ptr [esi],eax
lea	eax,dword ptr [ebp-12]
push	eax
lea	eax,inchar
push	eax
call	scanf
add	esp,8

;Read	c2		
lea	esi,dword ptr [ebp-16]
xor	eax,eax
mov dword ptr [esi],eax
lea	eax,dword ptr [ebp-16]
push	eax
lea	eax,inchar
push	eax
call	scanf
add	esp,8

;WriteExpression	c1		
mov	eax,dword ptr [ebp-12]
push	eax
lea	eax,outchar
push	eax
call	printf
add	esp,8

;WriteExpression	c2		
mov	eax,dword ptr [ebp-16]
push	eax
lea	eax,outchar
push	eax
call	printf
add	esp,8

;>false	c1	a	@flag_flag1
mov	eax,dword ptr [ebp-12]
mov	ebx,dword ptr [ebp-4]
cmp	eax,ebx
jle	@flag_flag1

;<false	c1	m	@flag_flag2
mov	eax,dword ptr [ebp-12]
mov	ebx,dword ptr [ebp-8]
cmp	eax,ebx
jge	@flag_flag2

;-	c1	a	#t3
mov	eax,dword ptr [ebp-12]
mov	ebx,dword ptr [ebp-4]
sub	eax,ebx
mov	dword ptr [ebp-32],eax

;+	#t3	97	#t4
mov	eax,dword ptr [ebp-32]
mov	ebx,97
add	eax,ebx
mov	dword ptr [ebp-36],eax

;:=	#t4		tt
mov	eax,dword ptr [ebp-36]
mov	dword ptr [ebp-20],eax

;WriteExpression	tt		
mov	eax,dword ptr [ebp-20]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;JmpTo	@flag_flag3		
jmp	@flag_flag3

;SetFlag	@flag_flag2		
@flag_flag2:

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;SetFlag	@flag_flag3		
@flag_flag3:

;SetFlag	@flag_flag1		
@flag_flag1:

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
