;Program to Print Fibonacci series up to 10 terms
printm macro mess
	lea dx,mess
	mov ah,09h
	int 21h
endm
exitp macro
	mov ah,4ch
	int 21h
endm

new_line macro
	mov ah,02h
	mov dl,0dh
	int 21h
	mov dl,0ah
	int 21h
endm

space macro
	mov ah,02h
	mov dl,' '
	int 21h
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



.model small
.stack 100h
.data
	msg db "First 10 terms of fibonacci series -> $"
	f1 dw 1
	f2 dw 1
	f3 dw ?
.code
	main proc
		mov ax,@data
		mov ds,ax
		mov bx,1
		mov dx,1
		printm msg
		pushall
		dec_output
		space
		popall
		pushall
		dec_output
		space
		popall
		mov cx,8
		@loop_fibo:
			mov f1,bx	
			mov f2,dx
			add bx,dx
			mov f3,bx	;f3=f1+f2
			pushall
			dec_output
			space
			popall
			mov bx,f2	;f1=f2
			mov dx,f3	;f2=f3
		loop @loop_fibo
		exitp
	main endp
end main
