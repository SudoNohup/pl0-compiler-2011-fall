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

;Begin	1		

id1_hello	PROC near32
push	ebp
mov	ebp,esp
sub	esp,24

;[]=	e	8	~t1
mov	eax,dword ptr [ebp+8]
sub	eax,60
mov	ecx,8
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
mov	dword ptr[ebp-4],eax

;:=	4		~t1
mov	eax,4
mov	esi,dword ptr [ebp-4]
mov	dword ptr [esi],eax

;[]=	e	8	~t2
mov	eax,dword ptr [ebp+8]
sub	eax,60
mov	ecx,8
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
mov	dword ptr[ebp-8],eax

;WriteExpression	~t2		
mov	esi,dword ptr [ebp-8]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	2	~t3
mov	eax,dword ptr [ebp+8]
sub	eax,20
mov	ecx,2
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
mov	dword ptr[ebp-12],eax

;[]=	e	8	~t4
mov	eax,dword ptr [ebp+8]
sub	eax,60
mov	ecx,8
cmp	ecx,10
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
mov	dword ptr[ebp-16],eax

;:=	~t4		~t3
mov	esi,dword ptr [ebp-16]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-12]
mov	dword ptr [esi],eax

;[]=	a	2	~t5
mov	eax,dword ptr [ebp+8]
sub	eax,20
mov	ecx,2
cmp	ecx,10
jl	@array_flag9
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag9:
cmp	ecx,0
jge	@array_flag10
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag10:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-20],eax

;WriteExpression	~t5		
mov	esi,dword ptr [ebp-20]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	2	~t6
mov	eax,dword ptr [ebp+8]
sub	eax,20
mov	ecx,2
cmp	ecx,10
jl	@array_flag11
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag11:
cmp	ecx,0
jge	@array_flag12
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag12:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-24],eax

;:=	~t6		c
mov	esi,dword ptr [ebp-24]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	dword ptr [esi],eax

;WriteExpression	c		
mov	esi,dword ptr [ebp+8]
sub	esi,12
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;End	1		
mov	esp,ebp
pop	ebp
ret

id1_hello	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,100
mov	dword ptr [ebp-4],5

;Display	0	1	
push	ebp

;Call	1		
call	id1_hello
add	esp,4

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
