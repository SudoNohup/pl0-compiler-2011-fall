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

id1_fun1	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;+	x	y	#t1
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-8]
add	eax,ebx
mov	dword ptr [ebp-12],eax

;:=	#t1		fun1
mov	eax,dword ptr [ebp-12]
mov	esi,fun1
mov	dword ptr [esi],eax

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_fun1	ENDP

;Begin	2		

id2_fun2	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;+	x	y	#t2
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-8]
add	eax,ebx
mov	dword ptr [ebp-12],eax

;:=	#t2		fun2
mov	eax,dword ptr [ebp-12]
mov	esi,fun2
mov	dword ptr [esi],eax

;End	2		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_fun2	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,28
push	ebx
push	esi
push	edi

;:=	1		a
mov	eax,1
mov	dword ptr [ebp-4],eax

;:=	2		b
mov	eax,2
mov	dword ptr [ebp-8],eax

;:=	3		c
mov	eax,3
mov	dword ptr [ebp-12],eax

;Display	0	0	

;Display	0	0	

;value=	a	a	0
lea	eax,dword ptr [ebp-4]
push	eax

;value=	b		0
push	eax

;CallValue	0		#t3
call	id0_Main
add	esp,0
mov	eax,Main
mov	dword ptr [ebp-24],eax

;value=	#t3	a	0
lea	eax,dword ptr [ebp-24]
push	eax

;value=	c		0
push	eax

;CallValue	0		#t4
call	id0_Main
add	esp,0
mov	eax,Main
mov	dword ptr [ebp-28],eax

;WriteExpression	#t4		
mov	eax,dword ptr [ebp-28]
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
