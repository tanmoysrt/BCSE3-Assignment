;Program to divide a 16 bit number by a 8 bit number.

printm macro mess
	lea dx,mess
	mov ah,09h
	int 21h
endm

;macro for hex input
hex_input macro
	local skip,input,letter,shift
	; output: bx
	xor bx,bx
	mov ah,01h
	int 21h
	cmp al,0dh
	je skip
	input:
		xor ah,ah
		cmp ax,'A'
		jge letter
		sub ax,'0'
		jmp shift
		letter:
			sub ax,55
		shift:	
			shl bx,1
			shl bx,1
			shl bx,1
			shl bx,1
		or bx,ax
		; take input
		mov ah,01h
		int 21h
		cmp al,0dh
		jne input
	skip:
endm

exitp macro
	mov ah,4ch
	int 21h
endm

;macro for hex_output
hex_output macro
	local output,display_loop,letter,line
	; input: bx	
	mov ah,02h
	mov cx,0
	output:
		mov dx,bx
		and dx,0fh
		cmp dx,10
		jge letter
		add dx,'0'
		jmp line
	letter:
			add dx,55
	line:
		push dx
		inc cx
		shr bx,1
		shr bx,1
		shr bx,1
		shr bx,1
	jnz output
	mov cx,cx
	display_loop:
		pop dx
		int 21h
	loop display_loop
endm

pushall macro
	push ax
	push bx
	push cx
	push dx
endm

popall macro
	pop dx
	pop cx
	pop bx
	pop ax
endm

.model small
.stack 100h
.data
	inpmsg1 db "16-bit number ? $"
	inpmsg2 db "8-bit number ? $"
	oupmsg1 db 10,13,"Quotient -> $"
	oupmsg2 db 10,13,"Remainder -> $"
	num1 dw ?
.code
	main proc
		mov ax,@data
		mov ds,ax
		;input
		printm inpmsg1
		hex_input
		mov num1,bx
		printm inpmsg2
		hex_input
		mov ax,num1
		xor dx,dx
		div bx
		;output
		mov bx,ax
		mov num1,dx
		printm oupmsg1
		pushall
		hex_output
		popall
		mov bx,num1
		printm oupmsg2
		pushall
		hex_output
		popall
		exitp
	main endp
end main