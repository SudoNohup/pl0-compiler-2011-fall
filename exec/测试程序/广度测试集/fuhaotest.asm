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
sub	esp,20

;:=	9		y
mov	eax,9
mov	dword ptr [ebp-8],eax

;-	0	5	#t1
mov	eax,0
mov	ebx,5
sub	eax,ebx
mov	dword ptr [ebp-12],eax

;+	#t1	8	#t2
mov	eax,dword ptr [ebp-12]
mov	ebx,8
add	eax,ebx
mov	dword ptr [ebp-16],eax

;-	#t2	y	#t3
mov	eax,dword ptr [ebp-16]
mov	ebx,dword ptr [ebp-8]
sub	eax,ebx
mov	dword ptr [ebp-20],eax

;:=	#t3		x
mov	eax,dword ptr [ebp-20]
mov	dword ptr [ebp-4],eax

;WriteExpression	x		
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
