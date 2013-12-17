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
string1	DB "yes",0DH,0AH,0
string2	DB "no",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi

;-	0	1	#t2
mov	eax,0
mov	ebx,1
sub	eax,ebx
mov	dword ptr [ebp-8],eax

;>	1	#t2	#t1
mov	eax,1
mov	ebx,dword ptr [ebp-8]
mov	dword ptr [ebp-4],1
cmp	eax,ebx
jg	judge_flag1
mov	dword ptr [ebp-4],0
judge_flag1:

;IfFalse	#t1	flag_flag1	
mov	eax,dword ptr [ebp-4]
cmp	eax,0
je	flag_flag1

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;Loop	flag_flag2		
jmp	flag_flag2

;SetFlag	flag_flag1		
flag_flag1:

;WriteString	string2		
lea	ecx,string2
push	ecx
call	printf
add	esp,4

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
