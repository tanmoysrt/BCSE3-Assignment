;Program to implement selection and insertion sort
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
	
	oupmsg1 db "Selection Sort -> $"
	oupmsg2 db "Insertion Sort -> $"
	arr dw 50 dup(?)
	s dw ?
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
		call selection_sort 				; sort the array
		
		printm oupmsg1
		
		pushall
		mov cl, 0
		lea si,arr 				; si = offset address of arr
		@array_output:
			mov bx, word ptr[si]
			dec_output 			; print the number
			space
			inc si
			inc si 				; 2 inc because it is a word
			inc cl
			cmp cl, s
			jl @array_output
		popall
		new_line

		exitp
	main endp

	; Selection sort
	selection_sort proc
		lea si,arr 				; si = offset address of arr
		mov cx,s
		dec cx 					; cx = size - 1
		@sec_outer:
			mov dx,cx				; dx is the inner loop counter
			mov di,si
			inc di
			inc di 					; di points at the number at second index
			mov min_idx,si 			; min_index = si
			push si 				; save si
			@sec_inner:
				mov si,min_idx 			; si = min_index
				mov bx,word ptr[si] 	; bx = value at min_index
				cmp word ptr[di],bx 	; check if arr value at di >= arr value at min_idx
				jge @incr
				mov min_idx,di 			; if not, set min_idx to di
				
				@incr: 					; if so, set di to point to next word
				inc di
				inc di
				dec dx
			jnz @sec_inner

			pop si 						; retrive si
			mov di,min_idx 				; swap contents of word at si and word at min_idx
			mov bx,word ptr[di]
			xchg word ptr[si],bx
			mov word ptr[di],bx

			inc si 						; increment si to point to next word
			inc si
		loop @sec_outer
		ret
	selection_sort endp

	insertion_sort proc
	    pushall
	    mov cl, 1 
	    mov bx, offset arr 				; bx = offset address of arr
		@ins_outer:
			mov ch, 0
			mov di, cx 					; di is outer loop index
			shl di 						; multiply by 2 because it is a word
			mov dl, arr[di] 			; dl = arr[di]
			mov si, di 
			dec si
			dec si 						; si = di - 2
			@ins_inner:  				; si is inner loop index
				cmp si, 0 				; while (si >= 0)
				jl @ins_outer_update
			cmp arr[si], dl 			; while (arr[si] > dl)
			jbe @ins_outer_update
			mov ch, arr[si] 			; arr[di] = arr[si]
			mov arr[di], ch
			dec di
			dec di
			dec si
			dec si
			jmp @ins_inner
			@ins_outer_update:
				mov arr[si+1], dl 		; arr[si+1] = dl
		inc cl
		cmp cl, s 						; increment counter till cl = size
		jl @ins_outer
		popall
		ret
	insertion_sort endp
end main