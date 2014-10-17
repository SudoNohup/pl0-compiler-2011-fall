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

;Begin	1		

id1_swap	PROC near32
push	ebp
mov	ebp,esp
sub	esp,16
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;:=	m		temp
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-12],eax

;:=	n		m
mov	esi,dword ptr [ebp-8]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-4]
mov	dword ptr [esi],eax

;:=	temp		n
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp-8]
mov	dword ptr [esi],eax

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_swap	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,16
push	ebx
push	esi
push	edi

;:=	1		a
mov	eax,1
mov	dword ptr [ebp-4],eax

;:=	2		b
mov	eax,2
mov	dword ptr [ebp-8],eax

;WriteExpression	a		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;WriteExpression	b		
mov	eax,dword ptr [ebp-8]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;Display	0	1	
push	ebp

;value=	a	m	1
lea	eax,dword ptr [ebp-4]
push	eax

;value=	b	n	1
lea	eax,dword ptr [ebp-8]
push	eax

;Call	1		
call	id1_swap
add	esp,12

;WriteExpression	a		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;WriteExpression	b		
mov	eax,dword ptr [ebp-8]
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
