;Program to implement binary search
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
	inpmsg1 db "Enter size of array: $"
	inpmsg2 db "Enter elements of array in sorted order: $"
	inpmsg3 db 10,13,"Enter element to be searched: $"
	oupmsg1 db "Element found at index $"
	oupmsg2 db "Element not found $"
	arr dw 50 dup(?)
	s dw ?
	start dw ?
	stop dw ?
	min_idx dw ?
	temp dw ?
.code
	main proc
		mov ax,@data 			; initialize segment registers
		mov ds,ax
		
		printm inpmsg1			; accept size of array
		dec_input	
		mov s,bx				; s = size
		lea si,arr 				; si = offset address of arr
		mov cx,bx 				; cx = size
		
		printm inpmsg2
		new_line
		@array_input:
			pushall
			dec_input
			mov word ptr[si],bx ; copy the entered number into arr
			popall
			inc si 				; 2 inc because it is a word
			inc si
		loop @array_input
		call sort 				; sort the array
		
		printm inpmsg3
		dec_input 				; input element to search
		
		lea si,arr 				; si = offset address of arr
		mov cx,s 				; cx = size
		dec cx 					; cx = size - 1
		
		mov start,00h 			; start at index 0
		mov stop,cx 			; stop at index = size - 1
		
		@binary_search:
			lea si,arr 			; si = offset address of arr
			mov cx,stop
			add cx,start
			shr cx,1			; cx = middle index = (stop + start) / 2
			add si,cx			; si=si+cx
			add si,cx 			; 2 times because of word
			
			cmp bx,word ptr[si]    ; compare middle element to element to be found
			je @found			; if middle element then found
			jg @greater
			;if less
			@lesser:
				dec cx		
				mov stop,cx     ; stop = index - 1
				jmp @compare
			@greater:
				inc cx
				mov start,cx 	; start = index + 1
			@compare:
				mov cx,stop
				cmp cx,start
		jge @binary_search 	   	; continue till stop >= start
		
		printm oupmsg2 			; if not found
		jmp @exit
		
		@found: 				; if found
			printm oupmsg1
			mov bx,cx
			dec_output
		@exit:
			exitp
	main endp

	; Selection sort
	sort proc
		lea si,arr 				; si = offset address of arr
		mov cx,s
		dec cx 					; cx = size - 1
		@outer_loop:
			mov dx,cx				; dx is the inner loop counter
			mov di,si
			inc di
			inc di 					; di points at the number at second index
			mov min_idx,si 			; min_index = si
			push si 				; save si
			@inner_loop:
				mov si,min_idx 			; si = min_index
				mov bx,word ptr[si] 	; bx = value at min_index
				cmp word ptr[di],bx 	; check if arr value at di >= arr value at min_idx
				jge @incr
				mov min_idx,di 			; if not, set min_idx to di
				
				@incr: 					; if so, set di to point to next word
				inc di
				inc di
				dec dx
			jnz @inner_loop

			pop si 						; retrive si
			mov di,min_idx 				; swap contents of word at si and word at min_idx
			mov bx,word ptr[di]
			xchg word ptr[si],bx
			mov word ptr[di],bx

			inc si 						; increment si to point to next word
			inc si
		loop @outer_loop
		ret
	sort endp
end main