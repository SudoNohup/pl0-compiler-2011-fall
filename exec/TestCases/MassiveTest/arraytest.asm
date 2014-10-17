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
sub	esp,96

;[]=	a	5	~t1
lea	eax,dword ptr [ebp-4]
mov	ecx,5
cmp	ecx,20
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
mov	dword ptr[ebp-84],eax

;:=	8		~t1
mov	eax,8
mov	esi,dword ptr [ebp-84]
mov	dword ptr [esi],eax

;[]=	a	3	~t2
lea	eax,dword ptr [ebp-4]
mov	ecx,3
cmp	ecx,20
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
mov	dword ptr[ebp-88],eax

;[]=	a	5	~t3
lea	eax,dword ptr [ebp-4]
mov	ecx,5
cmp	ecx,20
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
mov	dword ptr[ebp-92],eax

;:=	~t3		~t2
mov	esi,dword ptr [ebp-92]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-88]
mov	dword ptr [esi],eax

;[]=	a	3	~t4
lea	eax,dword ptr [ebp-4]
mov	ecx,3
cmp	ecx,20
jl	@array_flag7
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag7:
cmp	ecx,0
jge	@array_flag8
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag8:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-96],eax

;WriteExpression	~t4		
mov	esi,dword ptr [ebp-96]
mov	eax,dword ptr [esi]
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
