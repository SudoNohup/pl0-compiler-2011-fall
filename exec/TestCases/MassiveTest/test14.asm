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

id1_fun1	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-8]
add	eax,ebx
mov	dword ptr [ebp-12],eax
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp+16]
sub	esi,16
mov	dword ptr [esi],eax
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id1_fun1	ENDP

id2_fun2	PROC near32
push	ebp
mov	ebp,esp
sub	esp,12
push	ebx
push	esi
push	edi
mov	eax,dword ptr [ebp+12]
mov	dword ptr [ebp-4],eax
mov	eax,dword ptr [ebp+8]
mov	dword ptr [ebp-8],eax
mov	eax,dword ptr [ebp-4]
mov	ebx,dword ptr [ebp-8]
add	eax,ebx
mov	dword ptr [ebp-12],eax
mov	eax,dword ptr [ebp-12]
mov	esi,dword ptr [ebp+16]
sub	esi,20
mov	dword ptr [esi],eax
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

id2_fun2	ENDP

start:
push	ebp
mov	ebp,esp
sub	esp,28
push	ebx
push	esi
push	edi
mov	dword ptr [ebp-4],11
mov	eax,1
mov	dword ptr [ebp-8],eax
mov	eax,2
mov	dword ptr [ebp-12],eax
mov	eax,3
mov	dword ptr [ebp-4],eax
push	ebp
mov	eax,dword ptr [ebp-8]
push	eax
mov	eax,dword ptr [ebp-12]
push	eax
call	id2_fun2
add	esp,12
mov	eax,dword ptr [ebp-20]
mov	dword ptr [ebp-24],eax
push	ebp
mov	eax,dword ptr [ebp-24]
push	eax
mov	eax,dword ptr [ebp-4]
push	eax
call	id1_fun1
add	esp,12
mov	eax,dword ptr [ebp-16]
mov	dword ptr [ebp-28],eax
mov	eax,dword ptr [ebp-28]
push	eax
lea	eax,outint
push	eax
call	printf
add	esp,8
pop	edi
pop	esi
pop	ebx
mov	esp,ebp
pop	ebp
ret

end	start
