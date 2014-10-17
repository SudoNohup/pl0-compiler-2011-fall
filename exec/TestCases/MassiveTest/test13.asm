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
string1	DB "input x: ",0DH,0AH,0
string2	DB "input y: ",0DH,0AH,0
string3	DB "x mod y = ",0DH,0AH,0
string4	DB "choice 1 2 3: ",0DH,0AH,0
string5	DB "good ",0DH,0AH,0
string6	DB "better ",0DH,0AH,0
string7	DB "best ",0DH,0AH,0

	.CODE

id1_swap	PROC near32
push	ebp
mov	ebp,esp
sub	esp,4
push	ebx
push	esi
push	edi
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	eax,dword ptr [esi]
mov	dword ptr [ebp-4],eax
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp+8]
sub	esi,4
mov	dword ptr [esi],eax
mov	eax,dword ptr [ebp-4]
mov	esi,dword ptr [ebp+8]
sub	esi,8
mov	dword ptr [esi],eax
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_swap	ENDP

id2_mod	PROC near32
push	ebp
mov	ebp,esp
sub	esp,20
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	esi,dword ptr [ebp-8]
mov	ebx,dword ptr [esi]
cdq
idiv	ebx
mov	dword ptr [ebp-12],eax
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp-8]
mov	ebx,dword ptr [esi]
imul	eax,ebx
mov	dword ptr [ebp-16],eax
mov	esi,dword ptr [ebp-4]
mov	eax,dword ptr [esi]
mov	ebx,dword ptr [ebp-16]
sub	eax,ebx
mov	dword ptr [ebp-20],eax
mov	eax,dword ptr [ebp-20]
mov	esi,dword ptr [ebp-4]
mov	dword ptr [esi],eax
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_mod	ENDP

start:
push	ebp
mov	ebp,esp
sub	esp,56
push	ebx
push	esi
push	edi
mov	eax,3
mov	dword ptr [ebp-20],eax
@flag_flag1:
mov	eax,dword ptr [ebp-20]
mov	ebx,1
mov	dword ptr [ebp-40],1
cmp	eax,ebx
jge	@judge_flag1
mov	dword ptr [ebp-40],0
@judge_flag1:
mov	eax,dword ptr [ebp-40]
cmp	eax,0
je	@flag_flag2
lea	ecx,string1
push	ecx
call	printf
add	esp,4
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8
lea	ecx,string2
push	ecx
call	printf
add	esp,4
lea	eax,dword ptr [ebp-8]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8
push	ebp
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,dword ptr [ebp-8]
push	eax
call	id2_mod
add	esp,12
mov	eax,dword ptr [ebp-36]
mov	dword ptr [ebp-44],eax
mov	eax,dword ptr [ebp-44]
mov	dword ptr [ebp-4],eax
lea	ecx,string3
push	ecx
call	printf
add	esp,4
mov	eax,dword ptr [ebp-4]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8
lea	ecx,string4
push	ecx
call	printf
add	esp,4
lea	eax,dword ptr [ebp-12]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8
mov	eax,dword ptr [ebp-12]
mov	ebx,1
mov	dword ptr [ebp-48],1
cmp	eax,ebx
je	@judge_flag2
mov	dword ptr [ebp-48],0
@judge_flag2:
mov	eax,dword ptr [ebp-48]
cmp	eax,0
je	@flag_flag4
jmp	@flag_flag5
@flag_flag5:
lea	ecx,string5
push	ecx
call	printf
add	esp,4
jmp	@flag_flag3
@flag_flag4:
mov	eax,dword ptr [ebp-12]
mov	ebx,2
mov	dword ptr [ebp-52],1
cmp	eax,ebx
je	@judge_flag3
mov	dword ptr [ebp-52],0
@judge_flag3:
mov	eax,dword ptr [ebp-52]
cmp	eax,0
je	@flag_flag6
jmp	@flag_flag7
@flag_flag7:
lea	ecx,string6
push	ecx
call	printf
add	esp,4
jmp	@flag_flag3
@flag_flag6:
mov	eax,dword ptr [ebp-12]
mov	ebx,3
mov	dword ptr [ebp-56],1
cmp	eax,ebx
je	@judge_flag4
mov	dword ptr [ebp-56],0
@judge_flag4:
mov	eax,dword ptr [ebp-56]
cmp	eax,0
je	@flag_flag8
jmp	@flag_flag9
@flag_flag9:
lea	ecx,string7
push	ecx
call	printf
add	esp,4
jmp	@flag_flag3
@flag_flag8:
@flag_flag3:
mov	eax,dword ptr [ebp-20]
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-20],eax
jmp	@flag_flag1
@flag_flag2:
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
