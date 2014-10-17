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

id1_quicksort	PROC near32
push	ebp
mov	ebp,esp
sub	esp,152
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;<false	m	n	@flag_flag1
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-8]
cmp	eax,ebx
jge	@flag_flag1

;-	m	1	#t2
mov	eax,dword ptr [ebp-4]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-44],eax

;:=	#t2		i
mov	eax,dword ptr [ebp-44]
mov	dword ptr [ebp-12],eax

;:=	n		j
mov	eax,dword ptr [ebp-8]
mov	dword ptr [ebp-16],eax

;[]=	a	n	#t3
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-8]
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
mov	dword ptr[ebp-48],eax

;:=	#t3		v
mov	esi,dword ptr [ebp-48]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-20],eax

;:=	1		t
mov	eax,1
mov	dword ptr [ebp-36],eax

;SetFlag	@flag_flag2		
@flag_flag2:

;<=false	t	10	@flag_flag3
mov	eax,dword ptr [ebp-36]
mov	ebx,10
cmp	eax,ebx
jg	@flag_flag3

;+	i	1	#t5
mov	eax,dword ptr [ebp-12]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-56],eax

;:=	#t5		i
mov	eax,dword ptr [ebp-56]
mov	dword ptr [ebp-12],eax

;[]=	a	i	#t6
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
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
mov	dword ptr[ebp-60],eax

;:=	#t6		tmpai
mov	esi,dword ptr [ebp-60]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-28],eax

;-	v	1	#t7
mov	eax,dword ptr [ebp-20]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-64],eax

;SetFlag	@flag_flag4		
@flag_flag4:

;<=false	tmpai	#t7	@flag_flag5
mov	eax,dword ptr [ebp-28]
mov	ebx,dword ptr [ebp-64]
cmp	eax,ebx
jg	@flag_flag5

;+	i	1	#t9
mov	eax,dword ptr [ebp-12]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-72],eax

;:=	#t9		i
mov	eax,dword ptr [ebp-72]
mov	dword ptr [ebp-12],eax

;[]=	a	i	#t10
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
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
mov	dword ptr[ebp-76],eax

;:=	#t10		tmpai
mov	esi,dword ptr [ebp-76]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-28],eax

;-	tmpai	1	#t11
mov	eax,dword ptr [ebp-28]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-80],eax

;:=	#t11		tmpai
mov	eax,dword ptr [ebp-80]
mov	dword ptr [ebp-28],eax

;+	tmpai	1	tmpai
mov	eax,dword ptr [ebp-28]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-28],eax

;JmpTo	@flag_flag4		
jmp	@flag_flag4

;SetFlag	@flag_flag5		
@flag_flag5:

;-	j	1	#t12
mov	eax,dword ptr [ebp-16]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-84],eax

;:=	#t12		j
mov	eax,dword ptr [ebp-84]
mov	dword ptr [ebp-16],eax

;[]=	a	j	#t13
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-16]
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
mov	dword ptr[ebp-88],eax

;:=	#t13		tmpaj
mov	esi,dword ptr [ebp-88]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-32],eax

;+	v	1	#t14
mov	eax,dword ptr [ebp-20]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-92],eax

;SetFlag	@flag_flag6		
@flag_flag6:

;>=false	tmpaj	#t14	@flag_flag7
mov	eax,dword ptr [ebp-32]
mov	ebx,dword ptr [ebp-92]
cmp	eax,ebx
jl	@flag_flag7

;-	j	1	#t16
mov	eax,dword ptr [ebp-16]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-100],eax

;:=	#t16		j
mov	eax,dword ptr [ebp-100]
mov	dword ptr [ebp-16],eax

;[]=	a	j	#t17
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-16]
cmp	ecx,20
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
mov	dword ptr[ebp-104],eax

;:=	#t17		tmpaj
mov	esi,dword ptr [ebp-104]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-32],eax

;-	tmpaj	1	#t18
mov	eax,dword ptr [ebp-32]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-108],eax

;:=	#t18		tmpaj
mov	eax,dword ptr [ebp-108]
mov	dword ptr [ebp-32],eax

;-	tmpaj	1	tmpaj
mov	eax,dword ptr [ebp-32]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-32],eax

;JmpTo	@flag_flag6		
jmp	@flag_flag6

;SetFlag	@flag_flag7		
@flag_flag7:

;>=false	i	j	@flag_flag8
mov	eax,dword ptr [ebp-12]
mov	ebx,dword ptr [ebp-16]
cmp	eax,ebx
jl	@flag_flag8

;:=	100		t
mov	eax,100
mov	dword ptr [ebp-36],eax

;JmpTo	@flag_flag9		
jmp	@flag_flag9

;SetFlag	@flag_flag8		
@flag_flag8:

;-	t	1	#t20
mov	eax,dword ptr [ebp-36]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-116],eax

;:=	#t20		t
mov	eax,dword ptr [ebp-116]
mov	dword ptr [ebp-36],eax

;SetFlag	@flag_flag9		
@flag_flag9:

;[]=	a	i	#t21
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
cmp	ecx,20
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
mov	dword ptr[ebp-120],eax

;:=	#t21		x
mov	esi,dword ptr [ebp-120]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-24],eax

;[]=	a	i	#t22
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
cmp	ecx,20
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
mov	dword ptr[ebp-124],eax

;[]=	a	j	#t23
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-16]
cmp	ecx,20
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
mov	dword ptr[ebp-128],eax

;:=	#t23		#t22
mov	esi,dword ptr [ebp-128]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-124]
mov	dword ptr [esi],eax

;[]=	a	j	#t24
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-16]
cmp	ecx,20
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
mov	dword ptr[ebp-132],eax

;:=	x		#t24
mov	eax,dword ptr [ebp-24]
mov	esi,dword ptr [ebp-132]
mov	dword ptr [esi],eax

;+	t	1	t
mov	eax,dword ptr [ebp-36]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-36],eax

;JmpTo	@flag_flag2		
jmp	@flag_flag2

;SetFlag	@flag_flag3		
@flag_flag3:

;[]=	a	i	#t25
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
cmp	ecx,20
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
mov	dword ptr[ebp-136],eax

;:=	#t25		x
mov	esi,dword ptr [ebp-136]
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-24],eax

;[]=	a	i	#t26
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-12]
cmp	ecx,20
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
mov	dword ptr[ebp-140],eax

;[]=	a	n	#t27
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-8]
cmp	ecx,20
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
mov	dword ptr[ebp-144],eax

;:=	#t27		#t26
mov	esi,dword ptr [ebp-144]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-140]
mov	dword ptr [esi],eax

;[]=	a	n	#t28
mov	eax,dword ptr [ebp+16]
sub	eax,4
mov	ecx,dword ptr [ebp-8]
cmp	ecx,20
jl	@array_flag25
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag25:
cmp	ecx,0
jge	@array_flag26
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag26:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-148],eax

;:=	x		#t28
mov	eax,dword ptr [ebp-24]
mov	esi,dword ptr [ebp-148]
mov	dword ptr [esi],eax

;Display	1	1	
push	[ebp+16]

;value=	m	m	1
mov	eax,dword ptr [ebp-4]
push	eax

;value=	j	n	1
mov	eax,dword ptr [ebp-16]
push	eax

;Call	1		
call	id1_quicksort
add	esp,12

;+	i	1	#t29
mov	eax,dword ptr [ebp-12]
mov	ebx,1
add	eax,ebx
mov	dword ptr [ebp-152],eax

;Display	1	1	
push	[ebp+16]

;value=	#t29	m	1
mov	eax,dword ptr [ebp-152]
push	eax

;value=	n	n	1
mov	eax,dword ptr [ebp-8]
push	eax

;Call	1		
call	id1_quicksort
add	esp,12

;SetFlag	@flag_flag1		
@flag_flag1:

;End	1		
mov	esp,ebp
pop	ebp
ret

id1_quicksort	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,128

;[]=	a	0	#t30
lea	eax,dword ptr [ebp-4]
mov	ecx,0
cmp	ecx,20
jl	@array_flag27
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag27:
cmp	ecx,0
jge	@array_flag28
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag28:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-88],eax

;:=	3		#t30
mov	eax,3
mov	esi,dword ptr [ebp-88]
mov	dword ptr [esi],eax

;[]=	a	1	#t31
lea	eax,dword ptr [ebp-4]
mov	ecx,1
cmp	ecx,20
jl	@array_flag29
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag29:
cmp	ecx,0
jge	@array_flag30
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag30:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-92],eax

;:=	1		#t31
mov	eax,1
mov	esi,dword ptr [ebp-92]
mov	dword ptr [esi],eax

;[]=	a	2	#t32
lea	eax,dword ptr [ebp-4]
mov	ecx,2
cmp	ecx,20
jl	@array_flag31
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag31:
cmp	ecx,0
jge	@array_flag32
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag32:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-96],eax

;:=	4		#t32
mov	eax,4
mov	esi,dword ptr [ebp-96]
mov	dword ptr [esi],eax

;[]=	a	3	#t33
lea	eax,dword ptr [ebp-4]
mov	ecx,3
cmp	ecx,20
jl	@array_flag33
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag33:
cmp	ecx,0
jge	@array_flag34
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag34:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-100],eax

;:=	5		#t33
mov	eax,5
mov	esi,dword ptr [ebp-100]
mov	dword ptr [esi],eax

;[]=	a	4	#t34
lea	eax,dword ptr [ebp-4]
mov	ecx,4
cmp	ecx,20
jl	@array_flag35
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag35:
cmp	ecx,0
jge	@array_flag36
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag36:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-104],eax

;:=	9		#t34
mov	eax,9
mov	esi,dword ptr [ebp-104]
mov	dword ptr [esi],eax

;[]=	a	5	#t35
lea	eax,dword ptr [ebp-4]
mov	ecx,5
cmp	ecx,20
jl	@array_flag37
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag37:
cmp	ecx,0
jge	@array_flag38
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag38:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-108],eax

;:=	0		#t35
mov	eax,0
mov	esi,dword ptr [ebp-108]
mov	dword ptr [esi],eax

;Display	0	1	
push	ebp

;value=	0	m	1
mov	eax,0
push	eax

;value=	5	n	1
mov	eax,5
push	eax

;Call	1		
call	id1_quicksort
add	esp,12

;[]=	a	0	#t36
lea	eax,dword ptr [ebp-4]
mov	ecx,0
cmp	ecx,20
jl	@array_flag39
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag39:
cmp	ecx,0
jge	@array_flag40
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag40:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-112],eax

;WriteExpression	#t36		
mov	esi,dword ptr [ebp-112]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	1	#t37
lea	eax,dword ptr [ebp-4]
mov	ecx,1
cmp	ecx,20
jl	@array_flag41
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag41:
cmp	ecx,0
jge	@array_flag42
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag42:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-116],eax

;WriteExpression	#t37		
mov	esi,dword ptr [ebp-116]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	2	#t38
lea	eax,dword ptr [ebp-4]
mov	ecx,2
cmp	ecx,20
jl	@array_flag43
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag43:
cmp	ecx,0
jge	@array_flag44
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag44:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-120],eax

;WriteExpression	#t38		
mov	esi,dword ptr [ebp-120]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	3	#t39
lea	eax,dword ptr [ebp-4]
mov	ecx,3
cmp	ecx,20
jl	@array_flag45
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag45:
cmp	ecx,0
jge	@array_flag46
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag46:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-124],eax

;WriteExpression	#t39		
mov	esi,dword ptr [ebp-124]
mov	eax,dword ptr [esi]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;[]=	a	4	#t40
lea	eax,dword ptr [ebp-4]
mov	ecx,4
cmp	ecx,20
jl	@array_flag47
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag47:
cmp	ecx,0
jge	@array_flag48
lea	ecx,arrayerror
push	ecx
call	printf
add	esp,4
mov	esp,ebp
pop	ebp
ret
@array_flag48:
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-128],eax

;WriteExpression	#t40		
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
