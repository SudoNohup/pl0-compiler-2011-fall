	.386
	.model flat,stdcall

option	casemap:none
includelib	msvcrt.lib
printf	proto C:dword,:vararg
scanf	proto C:dword,:vararg
	.STACK	4096

	.DATA
inint 	DB "%d",0
outint 	DB "%d",0DH,0AH,0
inchar 	DB " %c",0
outchar 	DB "%c",0DH,0AH,0

	.CODE

;Begin	2		

id2_proc2	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi

;:=	c		d
mov	eax,dword ptr [ebp-4]
mov	dword ptr [ebp-8],eax

;End	2		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_proc2	ENDP

;Begin	1		

id1_proc1	PROC near32
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

;Display	1	2	
push	ebp
push	[ebp+16]

;Call	2		
call	id2_proc2
add	esp,8

;:=	3		proc1
mov	eax,3
mov	esi,dword ptr [ebp+16]
sub	esi,16
mov	dword ptr [esi],eax

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_proc1	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,32
push	ebx
push	esi
push	edi
mov	dword ptr [ebp-4],114

;Display	0	1	
push	ebp

;value=	c	b	1
lea	eax,dword ptr [ebp-12]
push	eax

;value=	m	a	1
lea	eax,dword ptr [ebp-4]
push	eax

;CallValue	1		#t1
call	id1_proc1
add	esp,12
mov	eax,dword ptr [ebp-16]
mov	dword ptr [ebp-20],eax

;-	1	#t1	#t2
mov	eax,1
mov	ebx,dword ptr [ebp-20]
sub	eax,ebx
mov	dword ptr [ebp-24],eax

;+	#t2	3	#t3
mov	eax,dword ptr [ebp-24]
mov	ebx,3
add	eax,ebx
mov	dword ptr [ebp-28],eax

;:=	#t3		james
mov	eax,dword ptr [ebp-28]
mov	dword ptr [ebp-8],eax

;WriteExpression	m		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outchar
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
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
