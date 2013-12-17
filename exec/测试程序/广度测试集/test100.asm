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
arrayerror 	DB "array out of range",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,16

;/	5	0	#t1
mov	eax,5
mov	ebx,0
cdq
idiv	ebx
mov	dword ptr [ebp-12],eax

;+	#t1	2	#t2
mov	eax,dword ptr [ebp-12]
mov	ebx,2
add	eax,ebx
mov	dword ptr [ebp-16],eax

;:=	#t2		a
mov	eax,dword ptr [ebp-16]
mov	dword ptr [ebp-4],eax

;WriteExpression	a		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
