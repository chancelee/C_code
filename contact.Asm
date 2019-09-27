.386
.model flat, stdcall
option casemap:none

include 	msvcrt.inc
includelib	msvcrt.lib

.data
CONTACTSTRUCT	struct
	szName	BYTE	25 dup(0)
	szPhone	BYTE	12 dup(0)	
CONTACTSTRUCT ends
PCONTACTSTRUCT TYPEDEF PTR CONTACTSTRUCT

g_contact	CONTACTSTRUCT	100	dup(<>)
g_nCount	DWORD	0
g_nMax		DWORD	100
g_tempContact	CONTACTSTRUCT	<>

szInfo	db	'####################',0ah,\
			'1. 添加联系人',0ah,\
			'2. 删除联系人',0ah,\
			'3. 修改联系人',0ah,\
			'4. 查询联系人',0ah,\
			'5. 遍历联系人',0ah,\
			'0. 退出',0ah,\
			'####################',0ah,00h	

szInputOptFmt	db	'%d',00h
szInputStrFmt	db	'%s',00h
szName			db	'姓名: ',00h
szPhone			db	'手机号: ',00h
szOutFmt		db	'%s  %s',0ah,00h
szNotFound		db	'not found',0ah,00h
szSearchName	db	'查询姓名：',00h
szAltName		db	'修改姓名：',00h
szDelName		db	'删除姓名：',00h

szPause			db	'pause',00h
szReInput		db	'请重新输入',00h
szCls			db	'cls',00h

szFileName		db	'contact',00h
szOpenFileR		db	'r',00h
szOpenFileW		db	'w',00h



g_exit	DWORD	0
g_add	DWORD	1
g_del	DWORD	2
g_alt	DWORD	3
g_sel	DWORD	4
g_each	DWORD	5


.code
;>>>>>>>>>>>>>>>>>>>>>>>>
;读取文件
;>>>>>>>>>>>>>>>>>>>>>>>>
READ_FILE PROC
	LOCAL @fp:DWORD
	pushad

;	fp = fopen("contact", "r")
	push offset szOpenFileR
	push offset szFileName
	call crt_fopen
	add esp, 8
	
	mov @fp, eax
;	fread(&g_nCount, 4, 1, @fp)
	push @fp
	push 1
	push 4
	lea eax, [g_nCount]
	push eax
	call crt_fread
	add esp, 10h
;	fread(&g_contact, sizeof(CONTACTSTRUC), g_nCount, @fp)
	push @fp
	push g_nCount
	mov eax, sizeof(CONTACTSTRUCT)
	push eax
	lea eax, [g_contact]
	push eax
	call crt_fread
	add esp, 10h

;	fclose(@fp)
	push @fp
	call crt_fclose
	add esp, 4
	
	popad
	ret
READ_FILE endp
;>>>>>>>>>>>>>>>>>>>>>>>>
;写入文件
;>>>>>>>>>>>>>>>>>>>>>>>>
WRITE_FILE PROC
	LOCAL @fp:DWORD
	pushad

;	fp = fopen("contact", "w")
	push offset szOpenFileW
	push offset szFileName
	call crt_fopen
	add esp, 8
	
	mov @fp, eax
;	fwrite(&g_nCount, 4, 1, @fp)
	push @fp
	push 1
	push 4
	lea eax, [g_nCount]
	push eax
	call crt_fwrite
	add esp, 10h
;	fwrite(&g_contact, sizeof(CONTACTSTRUC), g_nCount, @fp)
	push @fp
	push g_nCount
	mov eax, sizeof(CONTACTSTRUCT)
	push eax
	lea eax, [g_contact]
	push eax
	call crt_fwrite
	add esp, 10h

;	fclose(@fp)
	push @fp
	call crt_fclose
	add esp, 4
	
	popad
	ret

WRITE_FILE endp

;>>>>>>>>>>>>>>>>>>>>>>>>
;增
;>>>>>>>>>>>>>>>>>>>>>>>>
ADD_USER proc
	enter 8,0 
	push eax
	push ebx
	push ecx
	push esi


;if(g_nCount <g_nMax)
;{
;	scanf("%s %s", &g_contact[g_nCount].szName, &g_contact[g_nCount].szPhone)
;	++g_nCount
;}
	mov eax, g_nMax
	mov ecx, g_nCount	;ecx: g_nCount
	cmp eax, ecx
	je @F
	lea esi, [g_contact]
	mov eax, sizeof(CONTACTSTRUCT)
	imul eax, ecx
	add esi, eax	; esi : index of new info
	
	;输入姓名
	push offset szName
	call crt_printf
	add esp, 4
	lea eax, [esi+CONTACTSTRUCT.szName]
	push eax
	push offset szInputStrFmt
	call crt_scanf
	add esp, 8
	call crt_getchar
	;输入电话号码
	push offset szPhone
	call crt_printf
	add esp, 4
	lea eax, [esi+CONTACTSTRUCT.szPhone]
	push eax
	push offset szInputStrFmt
	call crt_scanf
	add esp, 8
	call crt_getchar
	inc g_nCount
	
	call WRITE_FILE
@@:
	pop esi
	pop ecx
	pop ebx
	pop eax
	leave
	ret
ADD_USER endp

;>>>>>>>>>>>>>>>>>>>>>>>>
;删
;>>>>>>>>>>>>>>>>>>>>>>>>
DEL_USER proc
	LOCAL @target:PCONTACTSTRUCT
	LOCAL @nSizeStc:DWORD
	pushad
	
	call SEL_USER
	mov @target, eax
;	if(@target == -1)
;	{ 
;		printf("not found\n");
;		return;
;	}

	cmp @target, -1
	jne @F
	push offset szNotFound
	call crt_printf
	add esp, 4
	jmp END_DEL
@@:
;	else
;	{
;		prinf("%s  %s", g_contact[i].szName, g_contact[i].szPhone);
;		int nMov(ecx) = (&g_contact[g_nCount - 1] - @target);
;		memmov(&@target, @target + sizeof(CONTACTSTRUCT), nMov);
;		--g_nCount;
;		call WRITE_FILE;
;	}
	mov eax, @target	;eax : @target
	lea	ebx, [eax + CONTACTSTRUCT.szName]
	lea	edx, [eax + CONTACTSTRUCT.szPhone]
	push edx
	push ebx
	push offset szOutFmt
	call crt_printf
	add esp, 0ch
	
	mov @nSizeStc, sizeof(CONTACTSTRUCT)
	mov ecx, g_nCount
	dec ecx;	;	ebx : g_nCount - 1
	
	mov eax, @nSizeStc
	mul ecx	; ecx : @nSizeStc * (g_nCount - 1)
	lea ecx, [g_contact]
	add ecx, eax	; ecx : &g_contact[g_nCount - 1]
	
	mov eax, @target
	sub ecx, eax	; ecx = &g_contact[g_nCount - 1] - @target

	mov edi, @target
	mov esi, edi
	add esi, @nSizeStc
	rep movsb
	
	dec g_nCount
	
	call WRITE_FILE
END_DEL:
	popad
	ret
DEL_USER endp

;>>>>>>>>>>>>>>>>>>>>>>>>
;改
;>>>>>>>>>>>>>>>>>>>>>>>>
ALT_USER proc
	LOCAL @target:PCONTACTSTRUCT	; or :DWORD
	
	pushad

;	@target = SEL_USER()	
	call SEL_USER
	mov @target, eax
;	if(@target == -1)
;	{ 
;		printf("not found\n");
;		return;
;	}

	cmp @target, -1
	jne @F
	push offset szNotFound
	call crt_printf
	add esp, 4
	jmp END_ALT
@@:
;	else
;	{
;		prinf("%s  %s", g_contact[i].szName, g_contact[i].szPhone);
;		RtlZeroMemory();
	mov eax, @target	;eax : @target
	lea	ebx, [eax + CONTACTSTRUCT.szName]
	lea	edx, [eax + CONTACTSTRUCT.szPhone]
	push edx
	push ebx
	push offset szOutFmt
	call crt_printf
	add esp, 0ch
	
	mov edi, @target
	mov al,  0
	mov ecx, sizeof(CONTACTSTRUCT)
	rep stosb
;	mov ebx, sizeof(CONTACTSTRUCT)
;	push ebx
;	push eax
;	invoke RtlZeroMemory, eax, ebx
;	add esp, 8


;		printf("更改姓名");
;		scanf("%s", target.szName);
;		getchar();
;		printf("更改手机号");
;		scanf("%s", target.szPhone);
;		getchar();
;	}

	push offset szName
	call crt_printf
	add esp, 4
	
	mov eax, @target
	lea ebx, [eax+CONTACTSTRUCT.szName]
	push ebx
	push offset szInputStrFmt
	call crt_scanf
	add esp, 8
	call crt_getchar
;输入电话号码
	push offset szPhone
	call crt_printf
	add esp, 4
	
	mov eax, @target
	lea ebx, [eax+CONTACTSTRUCT.szPhone]
	push ebx
	push offset szInputStrFmt
	call crt_scanf
	add esp, 8
	call crt_getchar

;	保存到文件
	call WRITE_FILE
END_ALT:
	popad
	ret
ALT_USER endp

;>>>>>>>>>>>>>>>>>>>>>>>>
;查
;return  eax:target addr
;>>>>>>>>>>>>>>>>>>>>>>>>
SEL_USER proc
	LOCAL	@index:DWORD
	LOCAL	@stSize:DWORD
	
	push ebx
	push ecx
	push esi
	push edi
	
	
;	printf("查询姓名：");
;	scanf("%s", &g_tempContact.szName)
	push offset szSearchName
	call crt_printf
	add esp, 4
	
	lea esi, [g_tempContact.szName]
	push esi
	push offset szInputStrFmt
	call crt_scanf
	add esp, 8
	call crt_getchar
	
;	int i
;	for(i = 0; i <g_nCount; ++i)
;	{
;		if(!strcmp(g_tempContact.szName, g_contact[i].szName))
;		{
;			break;
;		}
;	}

	mov eax, sizeof(CONTACTSTRUCT)
	mov @stSize, eax
	
	mov @index, 0
	lea eax, [g_contact]
	jmp @F
	
FIND:
	mov esi, eax
	lea esi, [esi + CONTACTSTRUCT.szName]
	lea edi, [g_tempContact.szName]
	mov ecx, 6
	repe cmpsd
	je	FOUND
	
	mov ebx, @index
	inc ebx
	mov @index, ebx
	add eax, @stSize
@@:
	mov ebx, g_nCount
	cmp @index, ebx
	jb	FIND

	mov eax, -1
FOUND:
	pop edi
	pop esi
	pop ecx
	pop ebx
	ret
SEL_USER endp

;>>>>>>>>>>>>>>>>>>>>>>>>
;遍历
;>>>>>>>>>>>>>>>>>>>>>>>>
EACH_USER proc
	LOCAL @index:DWORD
	pushad

;ecx = 0
;esi = &g_contact
;while(ecx < g_nCount)
;{
;	printf(esi)
;	++ecx;
;	esi += sizeof(CONTACTSTRUCT	)
;}

	mov edi, sizeof(CONTACTSTRUCT)
	mov @index, 0
	lea esi, [g_contact]
LOOP_EACH:
	mov ecx, g_nCount
	cmp @index, ecx
	je @F
	
	lea ebx, [esi+CONTACTSTRUCT.szName]
	lea edx, [esi+CONTACTSTRUCT.szPhone]
	push edx
	push ebx
	push offset szOutFmt
	call crt_printf
	add esp, 0ch
	
	add esi, edi
	inc @index
	jmp LOOP_EACH
@@:
	popad
	ret
EACH_USER endp


;>>>>>>>>>>>>>>>>>>>>>>>>
;main
;>>>>>>>>>>>>>>>>>>>>>>>>
main proc
	LOCAL @opt:DWORD	
	
	mov @opt, 0
;	读取文件	
	call READ_FILE
OPT:
	push offset szPause
	call crt_system
	add esp, 4

	push offset szCls
	call crt_system
	add esp, 4
	
	push offset szInfo
	call crt_printf
	add esp, 4
	
	lea eax, [@opt]
	push eax
	push offset szInputOptFmt
	call crt_scanf
	add esp, 8
	call crt_getchar
	mov eax, @opt

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>add
	cmp eax, g_add
	jne	DEL
	call ADD_USER
	jmp OPT

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>del
DEL:
	cmp eax, g_del
	jne	ALT
	call DEL_USER
	jmp OPT
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>alt
ALT:
	cmp eax, g_alt
	jne	SELECT
	call ALT_USER
	jmp OPT
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>select
SELECT:
	cmp eax, g_sel
	jne	EACH
	call SEL_USER

;	if(ret == -1)
;	{ 
;		printf("not found\n");
;		continue;
;	}
;	else
;	{
;		prinf("%s  %s", g_contact[i].szName, g_contact[i].szPhone);
;	}
	cmp eax, -1
	jne @F
	push offset szNotFound
	call crt_printf
	add esp, 4
	jmp CONT
@@:
	lea	edx, [eax + CONTACTSTRUCT.szPhone]
	push edx
	push eax
	push offset szOutFmt
	call crt_printf
	add esp, 0ch
CONT:
	jmp OPT
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>遍历
EACH:
	cmp eax, g_each
	jne	EXIT
	call EACH_USER
	jmp OPT
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>exit
EXIT:		
	cmp eax, g_exit
	jne DEF
	jmp END_MAIN
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>default
DEF:		
	push offset szReInput
	call crt_printf
	add esp, 4
	jmp OPT
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>end main
END_MAIN:
	push offset szPause
	call crt_system
	add esp, 4
	
	ret
	
main endp
	
start:
	call main
	ret
end start
	
end