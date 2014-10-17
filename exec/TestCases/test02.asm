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

;Begin	2		

id2_proc2	PROC near32
push	ebp
mov	ebp,esp
sub	esp,8

;:=	m		d
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-4],eax

;:=	2		c
mov	eax,2
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	dword ptr [esi],eax

;WriteExpression	d		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;End	2		
mov	esp,ebp
pop	ebp
ret

id2_proc2	ENDP

;Begin	1		

id1_proc1	PROC near32
push	ebp
mov	ebp,esp
sub	esp,16
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;Display	1	2	
push	ebp
push	[ebp+16]

;Call	2		
call	id2_proc2
add	esp,8

;:=	3		proc1
mov	eax,3
mov	esi,dword ptr [ebp+16]
sub	esi,20
mov	dword ptr [esi],eax

;:=	4		b
mov	eax,4
mov	esi,dword ptr [ebp-8]
mov	dword ptr [esi],eax

;End	1		
mov	esp,ebp
pop	ebp
ret

id1_proc1	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,36
mov	dword ptr [ebp-4],114

;:=	1		c
mov	eax,1
mov	dword ptr [ebp-12],eax

;Display	0	1	
push	ebp

;value=	c	a	1
lea	eax,dword ptr [ebp-12]
push	eax

;value=	e	b	1
lea	eax,dword ptr [ebp-16]
push	eax

;CallValue	1		#t1
call	id1_proc1
add	esp,12
mov	eax,dword ptr [ebp-20]
mov	dword ptr [ebp-24],eax

;-	1	#t1	#t2
mov	eax,1
mov	ebx,dword ptr [ebp-24]
sub	eax,ebx
mov	dword ptr [ebp-28],eax

;+	#t2	3	#t3
mov	eax,dword ptr [ebp-28]
mov	ebx,3
add	eax,ebx
mov	dword ptr [ebp-32],eax

;:=	#t3		james
mov	eax,dword ptr [ebp-32]
mov	dword ptr [ebp-8],eax

;WriteExpression	m		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outchar
push	eax
call	printf
add	esp,8

;WriteExpression	c		
mov	eax,dword ptr [ebp-12]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;WriteExpression	e		
mov	eax,dword ptr [ebp-16]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;WriteExpression	james		
mov	eax,dword ptr [ebp-8]
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
