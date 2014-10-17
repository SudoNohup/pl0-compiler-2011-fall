	.386
	.model flat,stdcall

option	casemap:none
includelib	msvcrt.lib
printf	proto C:dword,:vararg
scanf	proto C:dword,:vararg
	.STACK	4096

	.DATA
inint 	DB "%d",0
outint 	DB "%d",0DH,0AH,0
inchar 	DB " %c",0
outchar 	DB "%c",0DH,0AH,0
string1	DB "input x: ",0DH,0AH,0
string2	DB "input y: ",0DH,0AH,0
string3	DB "x mod y = ",0DH,0AH,0
string4	DB "choice 1 2 3: ",0DH,0AH,0
string5	DB "good ",0DH,0AH,0
string6	DB "better ",0DH,0AH,0
string7	DB "best ",0DH,0AH,0

	.CODE

;Begin	1		

id1_swap	PROC near32
push	ebp
mov	ebp,esp
sub	esp,8
push	ebx
push	esi
push	edi

;=	x		temp
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-4],eax

;=	y		x
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	dword ptr [esi],eax

;=	temp		y
mov	eax,dword ptr [ebp-4]
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	dword ptr [esi],eax

;End	1		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_swap	ENDP

;Begin	2		

id2_mod	PROC near32
push	ebp
mov	ebp,esp
sub	esp,24
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax

;/	fArg1	fArg2	#t1
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-8]
mov	ebx,dword ptr [esi]
cdq
idiv	ebx
mov	dword ptr [ebp-12],eax

;*	#t1	fArg2	#t2
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp-8]
mov	ebx,dword ptr [esi]
imul	eax,ebx
mov	dword ptr [ebp-16],eax

;-	fArg1	#t2	#t3
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,dword ptr [ebp-16]
sub	eax,ebx
mov	dword ptr [ebp-20],eax

;=	#t3		fArg1
mov	eax,dword ptr [ebp-20]
mov	esi,dword ptr [ebp-4]
mov	dword ptr [esi],eax

;=	fArg1		mod
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+16]
sub	esi,36
mov	dword ptr [esi],eax

;End	2		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_mod	ENDP

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,60
push	ebx
push	esi
push	edi

;=	3		i
mov	eax,3
mov	dword ptr [ebp-20],eax

;SetFlag	flag_flag1		
flag_flag1:

;>=	i	1	#t4
mov	eax,dword ptr [ebp-20]
mov	ebx,1
mov	dword ptr [ebp-40],1
cmp	eax,ebx
jae	judge_flag1
mov	dword ptr [ebp-40],0
judge_flag1:

;IfFalse	#t4	flag_flag2	
mov	eax,dword ptr [ebp-40]
cmp	eax,0
je	flag_flag2

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;Read	x		
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;WriteString	string2		
lea	ecx,string2
push	ecx
call	printf
add	esp,4

;Read	y		
lea	eax,dword ptr [ebp-8]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;Display	0	2	
push	ebp

;value=	x	fArg1	2
lea	eax,dword ptr [ebp-4]
push	eax

;value=	y	fArg2	2
lea	eax,dword ptr [ebp-8]
push	eax

;CallValue	2		#t5
call	id2_mod
add	esp,12
mov	eax,dword ptr [ebp-36]
mov	dword ptr [ebp-44],eax

;=	#t5		x
mov	eax,dword ptr [ebp-44]
mov	dword ptr [ebp-4],eax

;WriteString	string3		
lea	ecx,string3
push	ecx
call	printf
add	esp,4

;WriteExpression	x		
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8

;WriteString	string4		
lea	ecx,string4
push	ecx
call	printf
add	esp,4

;Read	g		
lea	eax,dword ptr [ebp-12]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;==	g	1	#t6
mov	eax,dword ptr [ebp-12]
mov	ebx,1
mov	dword ptr [ebp-48],1
cmp	eax,ebx
je	judge_flag2
mov	dword ptr [ebp-48],0
judge_flag2:

;IfFalse	#t6	flag_flag4	
mov	eax,dword ptr [ebp-48]
cmp	eax,0
je	flag_flag4

;Loop	flag_flag5		
jmp	flag_flag5

;SetFlag	flag_flag5		
flag_flag5:

;WriteString	string5		
lea	ecx,string5
push	ecx
call	printf
add	esp,4

;Loop	flag_flag3		
jmp	flag_flag3

;SetFlag	flag_flag4		
flag_flag4:

;==	g	2	#t7
mov	eax,dword ptr [ebp-12]
mov	ebx,2
mov	dword ptr [ebp-52],1
cmp	eax,ebx
je	judge_flag3
mov	dword ptr [ebp-52],0
judge_flag3:

;IfFalse	#t7	flag_flag6	
mov	eax,dword ptr [ebp-52]
cmp	eax,0
je	flag_flag6

;Loop	flag_flag7		
jmp	flag_flag7

;SetFlag	flag_flag7		
flag_flag7:

;WriteString	string6		
lea	ecx,string6
push	ecx
call	printf
add	esp,4

;Loop	flag_flag3		
jmp	flag_flag3

;SetFlag	flag_flag6		
flag_flag6:

;==	g	3	#t8
mov	eax,dword ptr [ebp-12]
mov	ebx,3
mov	dword ptr [ebp-56],1
cmp	eax,ebx
je	judge_flag4
mov	dword ptr [ebp-56],0
judge_flag4:

;IfFalse	#t8	flag_flag8	
mov	eax,dword ptr [ebp-56]
cmp	eax,0
je	flag_flag8

;Loop	flag_flag9		
jmp	flag_flag9

;SetFlag	flag_flag9		
flag_flag9:

;WriteString	string7		
lea	ecx,string7
push	ecx
call	printf
add	esp,4

;Loop	flag_flag3		
jmp	flag_flag3

;SetFlag	flag_flag8		
flag_flag8:

;SetFlag	flag_flag3		
flag_flag3:

;-	i	1	i
mov	eax,dword ptr [ebp-20]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-20],eax

;Loop	flag_flag1		
jmp	flag_flag1

;SetFlag	flag_flag2		
flag_flag2:

;End	0		
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start