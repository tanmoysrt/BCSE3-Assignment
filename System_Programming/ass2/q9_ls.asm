;Program to Implement Linear search.
new_line macro
	mov ah,02h
	mov dl,0dh
	int 21h
	mov dl,0ah
	int 21h
endm

;macro to print space
space macro
	mov ah,02h
	mov dl,' '
	int 21h
endm

;macro to print a message
printm macro mess
	lea dx,mess
	mov ah,09h
	int 21h
endm

;macro to exit
exitp macro
	mov ah,4ch
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

; macro for decimal input
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

; macro for decimal output
dec_output macro
	local start,repeat,display
	start:                        ; jump label
		mov ax, bx                   ; set ax=bx
		xor cx, cx                   ; clear cx
		mov bx, 10                   ; set bx=10
	repeat:                       ; loop label
		xor dx, dx                   ; clear dx
		div bx                       ; divide ax by bx
		push dx                      ; push dx onto the stack
		inc cx                       ; increment cx
		or ax, ax                    ; take or of ax with ax
		jne repeat                   ; jump to label repeat if zf=0
		mov ah, 2                    ; set output function
	display:                      ; loop label
		pop dx                       ; pop a value from stack to dx
		or dl, 30h                   ; convert decimal to ascii code
		int 21h                      ; print a character
		loop display
endm
.model small
.stack 100h
.data
	inpmsg1 db "Enter size of array: $"
	inpmsg2 db "Enter elements of array: $"
	inpmsg3 db 10,13,"Enter element to be searched: $"
	oupmsg1 db "Element found at index $"
	oupmsg2 db "Element not found $"
	arr dw 50 dup(?)
	s dw ?
.code
	main proc
		mov ax,@data			   	; initialize segment registers
		mov ds,ax
		
		printm inpmsg1
		dec_input 					; input size
		
		printm inpmsg2
		new_line
		mov s,bx 					; s = size
		
		lea si,arr 					; si = offset address of arr
		mov cx,bx 					; cx = size
		
		@array_input:
			pushall
			dec_input
			mov word ptr[si],bx 	; accept array elements
			popall
			inc si 					; increment to point to the next word
			inc si
		loop @array_input
		
		printm inpmsg3 
		dec_input 					; input element to be searched
		lea si,arr 					; si = offset address of arr
		mov cx,s 					; cx = size
		
		@linear_search:
			cmp bx,word ptr[si]
			je @found
			inc si
			inc si
		loop @linear_search
		
		printm oupmsg2 				; if not found
		jmp @exit
		
		@found: 					; if found
			printm oupmsg1
			mov bx,s
			sub bx,cx 				; get the index from beginning
			dec_output
		@exit:
			exitp
	main endp
end main