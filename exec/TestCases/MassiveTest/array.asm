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

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,116
push	ebx
push	esi
push	edi

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-84],eax

;SetFlag	@flag_flag1		
@flag_flag1:

;<=	i	9	#t1
mov	eax,dword ptr [ebp-84]
mov	ebx,9
mov	dword ptr [ebp-92],1
cmp	eax,ebx
jle	@judge_flag1
mov	dword ptr [ebp-92],0
@judge_flag1:

;IfFalse	#t1	@flag_flag2	
mov	eax,dword ptr [ebp-92]
cmp	eax,0
je	@flag_flag2

;[]=	a	i	#t2
lea	eax,dword ptr [ebp-4]
mov	ecx,dword ptr [ebp-84]
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-96],eax

;:=	i		#t2
mov	eax,dword ptr [ebp-84]
mov	esi,dword ptr [ebp-96]
mov	dword ptr [esi],eax

;:=	i		temp
mov	eax,dword ptr [ebp-84]
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

;<=	i	9	#t3
mov	eax,dword ptr [ebp-84]
mov	ebx,9
mov	dword ptr [ebp-100],1
cmp	eax,ebx
jle	@judge_flag2
mov	dword ptr [ebp-100],0
@judge_flag2:

;IfFalse	#t3	@flag_flag4	
mov	eax,dword ptr [ebp-100]
cmp	eax,0
je	@flag_flag4

;[]=	a	i	#t5
lea	eax,dword ptr [ebp-4]
mov	ecx,dword ptr [ebp-84]
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-108],eax

;[]=	b	#t5	#t4
lea	eax,dword ptr [ebp-44]
mov	esi,dword ptr [ebp-108]
mov	ecx,dword ptr [esi]
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-104],eax

;:=	i		#t4
mov	eax,dword ptr [ebp-84]
mov	esi,dword ptr [ebp-104]
mov	dword ptr [esi],eax

;[]=	b	i	#t6
lea	eax,dword ptr [ebp-44]
mov	ecx,dword ptr [ebp-84]
imul	ecx,4
sub	eax,ecx
mov	dword ptr[ebp-112],eax

;WriteExpression	#t6		
mov	esi,dword ptr [ebp-112]
mov	eax,dword ptr [esi]
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

;JmpTo	@flag_flag3		
jmp	@flag_flag3

;SetFlag	@flag_flag4		
@flag_flag4:

;:=	0		i
mov	eax,0
mov	dword ptr [ebp-84],eax

;SetFlag	@flag_flag5		
@flag_flag5:

;<=	i	9	#t7
mov	eax,dword ptr [ebp-84]
mov	ebx,9
mov	dword ptr [ebp-116],1
cmp	eax,ebx
jle	@judge_flag3
mov	dword ptr [ebp-116],0
@judge_flag3:

;IfFalse	#t7	@flag_flag6	
mov	eax,dword ptr [ebp-116]
cmp	eax,0
je	@flag_flag6

;:=	i		temp
mov	eax,dword ptr [ebp-84]
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
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
