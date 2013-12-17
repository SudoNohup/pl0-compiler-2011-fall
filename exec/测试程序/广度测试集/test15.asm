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
string1	DB "a[11]ÊÇ",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,48
push	ebx
push	esi
push	edi

;[]=	a	11	#t1
lea	eax,dword ptr [ebp-4]
mov	ecx,11
cmp	ecx,10
jl	@array_flag1
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag1:
cmp	ecx,0
jge	@array_flag2
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag2:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-44],eax

;:=	1		#t1
mov	eax,1
mov	esi,dword ptr [ebp-44]
mov	dword ptr [esi],eax

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;[]=	a	11	#t2
lea	eax,dword ptr [ebp-4]
mov	ecx,11
cmp	ecx,10
jl	@array_flag3
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag3:
cmp	ecx,0
jge	@array_flag4
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag4:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-48],eax

;WriteExpression	#t2		
mov	esi,dword ptr [ebp-48]
mov	eax,dword ptr [esi]
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
