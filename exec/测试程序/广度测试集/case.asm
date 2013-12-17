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
string1	DB "12",0DH,0AH,0
string2	DB "3",0DH,0AH,0

	.CODE

;Begin	0		

start:
push	ebp
mov	ebp,esp
sub	esp,16

;Read	choice		
lea	eax,dword ptr [ebp-4]
push	eax
lea	eax,inint
push	eax
call	scanf
add	esp,8

;=false	choice	1	@flag_flag3
mov	eax,dword ptr [ebp-4]
mov	ebx,1
cmp	eax,ebx
jne	@flag_flag3

;JmpTo	@flag_flag2		
jmp	@flag_flag2

;SetFlag	@flag_flag3		
@flag_flag3:

;=false	choice	2	@flag_flag4
mov	eax,dword ptr [ebp-4]
mov	ebx,2
cmp	eax,ebx
jne	@flag_flag4

;SetFlag	@flag_flag2		
@flag_flag2:

;WriteString	string1		
lea	ecx,string1
push	ecx
call	printf
add	esp,4

;JmpTo	@flag_flag6		
jmp	@flag_flag6

;SetFlag	@flag_flag4		
@flag_flag4:

;=false	choice	3	@flag_flag6
mov	eax,dword ptr [ebp-4]
mov	ebx,3
cmp	eax,ebx
jne	@flag_flag6

;WriteString	string2		
lea	ecx,string2
push	ecx
call	printf
add	esp,4

;SetFlag	@flag_flag6		
@flag_flag6:

;WriteExpression	choice		
mov	eax,dword ptr [ebp-4]
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
