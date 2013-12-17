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
sub	esp,64

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

;+	3	4	#t2
mov	eax,3
mov	ebx,4
add	eax,ebx
mov	dword ptr [ebp-52],eax

;-	#t2	5	#t3
mov	eax,dword ptr [ebp-52]
mov	ebx,5
sub	eax,ebx
mov	dword ptr [ebp-56],eax

;:=	#t3		~t1
mov	eax,dword ptr [ebp-56]
mov	esi,dword ptr [ebp-48]
mov	dword ptr [esi],eax

;[]=	abc	3	~t4
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
mov	dword ptr[ebp-60],eax

;WriteExpression	~t4		
mov	esi,dword ptr [ebp-60]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	abc	3	~t5
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
mov	dword ptr[ebp-64],eax

;:=	~t5		a
mov	esi,dword ptr [ebp-64]
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
