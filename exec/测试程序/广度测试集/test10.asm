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

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,4
push	ebx
push	esi
push	edi

;:=	44		a
mov	eax,44
mov	dword ptr [ebp-4],eax

;IfFalse	1	@flag_flag1	
mov	eax,1
cmp	eax,0
je	@flag_flag1

;WriteExpression	a		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;JmpTo	@flag_flag2		
jmp	@flag_flag2

;SetFlag	@flag_flag1		
@flag_flag1:

;SetFlag	@flag_flag2		
@flag_flag2:

;End	0		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
