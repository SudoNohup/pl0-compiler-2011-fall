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

;Begin	1		

id1_swap	PROC near32
push	ebp
mov	ebp,esp
sub	esp,16
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;:=	x		temp
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-12],eax

;:=	y		x
mov	esi,dword ptr [ebp-8]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-4]
mov	dword ptr [esi],eax

;:=	temp		y
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp-8]
mov	dword ptr [esi],eax

;End	1		
mov	esp,ebp
pop	ebp
ret

id1_swap	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,132

;:=	4		n
mov	eax,4
mov	dword ptr [ebp-12],eax

;[]=	a	0	~t1
lea	eax,dword ptr [ebp-16]
mov	ecx,0
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
mov	dword ptr[ebp-60],eax

;:=	1		~t1
mov	eax,1
mov	esi,dword ptr [ebp-60]
mov	dword ptr [esi],eax

;[]=	a	1	~t2
lea	eax,dword ptr [ebp-16]
mov	ecx,1
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
mov	dword ptr[ebp-64],eax

;:=	3		~t2
mov	eax,3
mov	esi,dword ptr [ebp-64]
mov	dword ptr [esi],eax

;[]=	a	2	~t3
lea	eax,dword ptr [ebp-16]
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
mov	dword ptr[ebp-68],eax

;:=	5		~t3
mov	eax,5
mov	esi,dword ptr [ebp-68]
mov	dword ptr [esi],eax

;[]=	a	3	~t4
lea	eax,dword ptr [ebp-16]
mov	ecx,3
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
mov	dword ptr[ebp-72],eax

;:=	4		~t4
mov	eax,4
mov	esi,dword ptr [ebp-72]
mov	dword ptr [esi],eax

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-4],eax

;-	n	1	#t5
mov	eax,dword ptr [ebp-12]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-76],eax

;SetFlag	@flag_flag1		
@flag_flag1:

;<=false	i	#t5	@flag_flag2
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-76]
cmp	eax,ebx
jg	@flag_flag2

;+	i	1	#t7
mov	eax,dword ptr [ebp-4]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-84],eax

;:=	#t7		j
mov	eax,dword ptr [ebp-84]
mov	dword ptr [ebp-8],eax

;-	n	1	#t8
mov	eax,dword ptr [ebp-12]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-88],eax

;SetFlag	@flag_flag3		
@flag_flag3:

;<=false	j	#t8	@flag_flag4
mov	eax,dword ptr [ebp-8]
mov	ebx,dword ptr [ebp-88]
cmp	eax,ebx
jg	@flag_flag4

;[]=	a	i	~t10
lea	eax,dword ptr [ebp-16]
mov	ecx,dword ptr [ebp-4]
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
mov	dword ptr[ebp-96],eax

;[]=	a	j	~t12
lea	eax,dword ptr [ebp-16]
mov	ecx,dword ptr [ebp-8]
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
mov	dword ptr[ebp-104],eax

;>false	~t10	~t12	@flag_flag5
mov	esi,dword ptr [ebp-96]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-104]
mov	ebx,dword ptr [esi]
cmp	eax,ebx
jle	@flag_flag5

;[]=	a	i	~t13
lea	eax,dword ptr [ebp-16]
mov	ecx,dword ptr [ebp-4]
cmp	ecx,10
jl	@array_flag13
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag13:
cmp	ecx,0
jge	@array_flag14
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag14:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-108],eax

;Display	0	1	
push	ebp

;value=	~t13	x	1
mov	eax,dword ptr [ebp-108]
push	eax

;[]=	a	j	~t14
lea	eax,dword ptr [ebp-16]
mov	ecx,dword ptr [ebp-8]
cmp	ecx,10
jl	@array_flag15
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag15:
cmp	ecx,0
jge	@array_flag16
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag16:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-112],eax

;value=	~t14	y	1
mov	eax,dword ptr [ebp-112]
push	eax

;Call	1		
call	id1_swap
add	esp,12

;SetFlag	@flag_flag5		
@flag_flag5:

;+	j	1	j
mov	eax,dword ptr [ebp-8]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-8],eax

;JmpTo	@flag_flag3		
jmp	@flag_flag3

;SetFlag	@flag_flag4		
@flag_flag4:

;+	i	1	i
mov	eax,dword ptr [ebp-4]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-4],eax

;JmpTo	@flag_flag1		
jmp	@flag_flag1

;SetFlag	@flag_flag2		
@flag_flag2:

;[]=	a	0	~t15
lea	eax,dword ptr [ebp-16]
mov	ecx,0
cmp	ecx,10
jl	@array_flag17
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag17:
cmp	ecx,0
jge	@array_flag18
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag18:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-116],eax

;WriteExpression	~t15		
mov	esi,dword ptr [ebp-116]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	1	~t16
lea	eax,dword ptr [ebp-16]
mov	ecx,1
cmp	ecx,10
jl	@array_flag19
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag19:
cmp	ecx,0
jge	@array_flag20
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag20:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-120],eax

;WriteExpression	~t16		
mov	esi,dword ptr [ebp-120]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	2	~t17
lea	eax,dword ptr [ebp-16]
mov	ecx,2
cmp	ecx,10
jl	@array_flag21
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag21:
cmp	ecx,0
jge	@array_flag22
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag22:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-124],eax

;WriteExpression	~t17		
mov	esi,dword ptr [ebp-124]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	3	~t18
lea	eax,dword ptr [ebp-16]
mov	ecx,3
cmp	ecx,10
jl	@array_flag23
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag23:
cmp	ecx,0
jge	@array_flag24
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag24:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-128],eax

;WriteExpression	~t18		
mov	esi,dword ptr [ebp-128]
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
