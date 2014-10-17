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
sub	esp,124

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-84],eax

;SetFlag	@flag_flag1		
@flag_flag1:

;<=false	i	9	@flag_flag2
mov	eax,dword ptr [ebp-84]
mov	ebx,9
cmp	eax,ebx
jg	@flag_flag2

;[]=	a	i	~t2
lea	eax,dword ptr [ebp-4]
mov	ecx,dword ptr [ebp-84]
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
mov	dword ptr[ebp-96],eax

;:=	i		~t2
mov	eax,dword ptr [ebp-84]
mov	esi,dword ptr [ebp-96]
mov	dword ptr [esi],eax

;[]=	a	i	~t3
lea	eax,dword ptr [ebp-4]
mov	ecx,dword ptr [ebp-84]
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
mov	dword ptr[ebp-100],eax

;:=	~t3		temp
mov	esi,dword ptr [ebp-100]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-88],eax

;WriteExpression	temp		
mov	eax,dword ptr [ebp-88]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;+	i	1	i
mov	eax,dword ptr [ebp-84]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-84],eax

;JmpTo	@flag_flag1		
jmp	@flag_flag1

;SetFlag	@flag_flag2		
@flag_flag2:

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-84],eax

;SetFlag	@flag_flag3		
@flag_flag3:

;<=false	i	9	@flag_flag4
mov	eax,dword ptr [ebp-84]
mov	ebx,9
cmp	eax,ebx
jg	@flag_flag4

;[]=	a	i	~t6
lea	eax,dword ptr [ebp-4]
mov	ecx,dword ptr [ebp-84]
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
mov	dword ptr[ebp-112],eax

;[]=	b	~t6	~t5
lea	eax,dword ptr [ebp-44]
mov	esi,dword ptr [ebp-112]
mov	ecx,dword ptr [esi]
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
mov	dword ptr[ebp-108],eax

;-	10	i	#t7
mov	eax,10
mov	ebx,dword ptr [ebp-84]
sub	eax,ebx
mov	dword ptr [ebp-116],eax

;:=	#t7		~t5
mov	eax,dword ptr [ebp-116]
mov	esi,dword ptr [ebp-108]
mov	dword ptr [esi],eax

;+	i	1	i
mov	eax,dword ptr [ebp-84]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-84],eax

;JmpTo	@flag_flag3		
jmp	@flag_flag3

;SetFlag	@flag_flag4		
@flag_flag4:

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-84],eax

;SetFlag	@flag_flag5		
@flag_flag5:

;<=false	i	9	@flag_flag6
mov	eax,dword ptr [ebp-84]
mov	ebx,9
cmp	eax,ebx
jg	@flag_flag6

;[]=	b	i	~t9
lea	eax,dword ptr [ebp-44]
mov	ecx,dword ptr [ebp-84]
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
mov	dword ptr[ebp-124],eax

;:=	~t9		temp
mov	esi,dword ptr [ebp-124]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-88],eax

;WriteExpression	temp		
mov	eax,dword ptr [ebp-88]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;+	i	1	i
mov	eax,dword ptr [ebp-84]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-84],eax

;JmpTo	@flag_flag5		
jmp	@flag_flag5

;SetFlag	@flag_flag6		
@flag_flag6:

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
