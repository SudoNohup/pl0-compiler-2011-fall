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
sub	esp,56

;[]=	abc	3	~t1
lea	eax,dword ptr [ebp-4]
mov	ecx,3
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
mov	dword ptr[ebp-48],eax

;:=	3		~t1
mov	eax,3
mov	esi,dword ptr [ebp-48]
mov	dword ptr [esi],eax

;[]=	abc	3	~t2
lea	eax,dword ptr [ebp-4]
mov	ecx,3
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
mov	dword ptr[ebp-52],eax

;WriteExpression	~t2		
mov	esi,dword ptr [ebp-52]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	abc	3	~t3
lea	eax,dword ptr [ebp-4]
mov	ecx,3
cmp	ecx,10
jl	@array_flag5
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag5:
cmp	ecx,0
jge	@array_flag6
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag6:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-56],eax

;:=	~t3		a
mov	esi,dword ptr [ebp-56]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-44],eax

;WriteExpression	a		
mov	eax,dword ptr [ebp-44]
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
