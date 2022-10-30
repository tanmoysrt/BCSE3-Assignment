;Program to multiply two numbers
printm macro mess
	lea dx,mess
	mov ah,09h
	int 21h
endm

dec_input macro
	local input,skip
	; output: bx
	xor bx,bx
	mov ah,01h
	int 21h
	;if \r
	cmp al,0dh
	je skip
	input:
		and ax,000fh
		push ax
		; bx=bx*10+ax
		mov ax,10
		mul bx
		mov bx,ax
		pop ax
		add bx,ax
		; take input
		mov ah,01h
		int 21h
		cmp al,0dh
		jne input
	skip:
endm

dec_output macro
	local start,repeat,display
	start:                        ; jump label
		mov ax, bx                     ; set ax=bx
		xor cx, cx                     ; clear cx
		mov bx, 10                     ; set bx=10
	repeat:                       ; loop label
		xor dx, dx                   ; clear dx
		div bx                       ; divide ax by bx
		push dx                      ; push dx onto the stack
		inc cx                       ; increment cx
		or ax, ax                    ; take or of ax with ax
		jne repeat                    ; jump to label repeat if zf=0
		mov ah, 2                      ; set output function
	display:                      ; loop label
		pop dx                       ; pop a value from stack to dx
		or dl, 30h                   ; convert decimal to ascii code
		int 21h                      ; print a character
		loop display
endm

exitp macro
	mov ah,4ch
	int 21h
endm

.model small
.stack 100h
.data
	inpmsg1 db "First number ? $"
	inpmsg2 db "Second number ? $"
	oupmsg db "Product -> $"
	num1 dw ?
	num2 dw ?
.code
	main proc
		mov ax,@data
		mov ds,ax
		xor bh,bh
		;input prompt
		printm inpmsg1
		dec_input
		mov num1,bx
		printm inpmsg2
		dec_input
		mov num2,bx
		printm oupmsg
		mov ax,num1
		mul num2
		mov bx,dx
		push ax
		dec_output
		pop ax		
		mov bx,ax
		dec_output
		exitp
	main endp
end main