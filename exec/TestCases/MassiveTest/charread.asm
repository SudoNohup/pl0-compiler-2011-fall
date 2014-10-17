	.386
	.model flat,stdcall

option	casemap:none
includelib	C:\masm32\lib\msvcrt.lib
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
sub	esp,20

;Read	a		
lea	esi,dword ptr [ebp-4]
xor	eax,eax
mov dword ptr [esi],eax
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,inchar
push	eax
call	scanf
add	esp,8

;Read	b		
lea	esi,dword ptr [ebp-8]
xor	eax,eax
mov dword ptr [esi],eax
lea	eax,dword ptr [ebp-8]
push	eax
lea	eax,inchar
push	eax
call	scanf
add	esp,8

;Read	c		
lea	esi,dword ptr [ebp-12]
xor	eax,eax
mov dword ptr [esi],eax
lea	eax,dword ptr [ebp-12]
push	eax
lea	eax,inchar
push	eax
call	scanf
add	esp,8

;WriteExpression	a		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outchar
push	eax
call	printf
add	esp,8

;WriteExpression	b		
mov	eax,dword ptr [ebp-8]
push	eax
lea	eax,outchar
push	eax
call	printf
add	esp,8

;+	c	b	#t1
mov	eax,dword ptr [ebp-12]
mov	ebx,dword ptr [ebp-8]
add	eax,ebx
mov	dword ptr [ebp-16],eax

;WriteExpression	#t1		
mov	eax,dword ptr [ebp-16]
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
