.386
.model flat,stdcall
option casemap:none
include	C:\RadASM\Masm32\include\windows.inc
include	C:\RadASM\Masm32\include\kernel32.inc
include	C:\RadASM\Masm32\include\msvcrt.inc
includelib	C:\RadASM\Masm32\lib\msvcrt.lib
includelib	C:\RadASM\Masm32\lib\kernel32.lib
include	C:\RadASM\Masm32\macro\macros.asm
.data
.code
start:
	push	ebp
	mov	ebp,esp
	sub	esp,100h
	mov	eax,1
	mov	ebx,eax
	mov	eax,ebx
	cmp	eax,0
	jne	@label0_1
	mov	dword ptr [ebp-08h],1
	jmp	@label0_2
@label0_1:
	mov	dword ptr [ebp-08h],0
@label0_2:
	cmp	dword ptr [ebp-08h],0
	jz	@l2
	invoke	crt_printf,SADD("0")
	jmp	@l2
@l2:
	invoke	crt_printf,SADD("1")
@l2:
	add	esp,100h
	pop	ebp
	invoke	ExitProcess,NULL
end	start
