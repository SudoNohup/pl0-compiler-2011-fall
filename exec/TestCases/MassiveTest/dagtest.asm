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
sub	esp,32

;-	0	c	#t1
mov	eax,0
mov	ebx,dword ptr [ebp-12]
sub	eax,ebx
mov	dword ptr [ebp-16],eax

;*	b	#t1	#t2
mov	eax,dword ptr [ebp-8]
mov	ebx,dword ptr [ebp-16]
imul	eax,ebx
mov	dword ptr [ebp-20],eax

;-	0	c	#t3
mov	eax,0
mov	ebx,dword ptr [ebp-12]
sub	eax,ebx
mov	dword ptr [ebp-24],eax

;*	b	#t3	#t4
mov	eax,dword ptr [ebp-8]
mov	ebx,dword ptr [ebp-24]
imul	eax,ebx
mov	dword ptr [ebp-28],eax

;+	#t2	#t4	#t5
mov	eax,dword ptr [ebp-20]
mov	ebx,dword ptr [ebp-28]
add	eax,ebx
mov	dword ptr [ebp-32],eax

;:=	#t5		a
mov	eax,dword ptr [ebp-32]
mov	dword ptr [ebp-4],eax

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
