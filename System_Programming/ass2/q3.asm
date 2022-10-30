;Program to print pairs of even numbers where the sum of the numbers in each pair is 100.
printm macro mess
	lea dx,mess
	mov ah,09h
	int 21h
endm

new_line macro
	mov ah,02h
	mov dl,0dh
	int 21h
	mov dl,0ah
	int 21h
endm

bracket macro char
	mov ah,02h
	mov dl,char
	int 21h
endm

; macro for decimal output
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

space macro
	mov ah,02h
	mov dl,' '
	int 21h
endm


.model small
.stack 100h
.data
	oupmsg db "Pairs of even numbers with sum 100: $"
	tempb dw ?
	tempc dw ?
.code
	main proc
		mov ax,@data
		mov ds,ax
		printm oupmsg
		new_line
		mov bx,0
		mov cx,100
		@print_loop:
			mov tempb,bx
			mov tempc,cx
			bracket '('
			dec_output
			space
			mov bx,tempc
			dec_output
			bracket ')'
			space
			mov bx,tempb
			mov cx,tempc
			inc bx
			inc bx
			dec cx
			dec cx
			cmp bx,50
		jle @print_loop
		new_line
		exitp
	main endp
end main