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
string1	DB "x is 0",0DH,0AH,0
string2	DB "x is 1",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,12

;:=	1		x
mov	eax,1
mov	dword ptr [ebp-4],eax

;=false	x	0	@flag_flag1
mov	eax,dword ptr [ebp-4]
mov	ebx,0
cmp	eax,ebx
jne	@flag_flag1

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;JmpTo	@flag_flag3		
jmp	@flag_flag3

;SetFlag	@flag_flag1		
@flag_flag1:

;=false	x	1	@flag_flag3
mov	eax,dword ptr [ebp-4]
mov	ebx,1
cmp	eax,ebx
jne	@flag_flag3

;WriteString	string2		
lea	ecx,string2
push	ecx
call	printf
add	esp,4

;SetFlag	@flag_flag3		
@flag_flag3:

;End	0		
mov	esp,ebp
pop	ebp
ret

end	start
