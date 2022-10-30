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

.model small
.stack 100h
.data
	msg db "Prime numbers from 1 to 100 -> $"
	num db ?
.code
	main proc 
		mov ax,@data
		mov ds,ax
		printm msg
		new_line 
		mov cl,02h
		start:	
			mov num,cl
			mov al,cl
			mov bl,01h
			mov bh,00h
			mov dx,0000h
			mov ah,00h

			;loop to check for prime no
			l1:
				div bl 				; al = quotient, ah = remainder
				cmp ah,00h      	; remainder is compared with 00h (ah)
				jne next
				inc bh          	; bh is incremented if the number is divisible by current value of bl
				next:
					cmp bh,02h 		; if bh = 02h, there is no need to proceed, it is not a prime
					je false
					inc bl          ; increment bl
					mov ax,0000h    ; to avoid divide overflow error
					mov dx,0000h    ; to avoid divide overflow error
					mov al,cl      	; move the test number to al
					cmp bl,cl      	; continue for x times, x = value of test number
					jne l1          ; jump to check again with incremented value of bl

			true: 
				mov ch,00h 
				mov bx,cx
				dec_output 			; display the number
				space
			
			false: 
				mov cl,num
				inc cl 				; proceed to the next number
				cmp cl,64h 			; till number <= 100
				jne start
		exitp
	main endp
end main