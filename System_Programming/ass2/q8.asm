;Program to identify the GCD and LCM of three numbers
new_line macro
	mov ah,02h
	mov dl,0dh
	int 21h
	mov dl,0ah
	int 21h
endm

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

.model small
.stack 100h
.data
	inpmsg1 db "First number ? $"
	inpmsg2 db "Second number ? $"
	inpmsg3 db "Third number ? $"
	oupmsg1 db 10,13,"GCD -> $"
	oupmsg2 db 10,13,"LCM -> $"
	num1 dw ?
	num2 dw ?
	num3 dw ?
	gcdn dw ?
	lcmn dw ?
.code
	;procedure to find gcd of two numbers
	gcd proc
	; input: bx,ax
	; output: gcd
	; Assumption: ax is greater than bx
	  up: 
	  	  cmp ax,bx        ;compare the two numbers.
		  je exit          ;if equal, go to exit label.
		  jb excg          ;if first number is below than second,
					   ;go to excg label.
	  up1: 
	  	   mov dx,0        ;initialize the dx.
		   div bx          ;divide the first number by second number.
		   cmp dx,0        ;compare remainder is zero or not.
		   je exit         ;if zero, jump to exit label.
		   mov ax,dx       ;if non-zero, move remainder to ax.
		   jmp up
		             		;jump to up label.
	  excg:
	  	   xchg ax,bx     ;exchange the remainder and quotient.
		   jmp up1        ;jump to up1.
	  exit:
	  	   mov gcdn,bx     ;store the result in gcd.
		   ret
	gcd endp

	lcm proc
	; input: bx,ax
	; output: dx
		xor dx,dx
		mul bx	;product of numbers
		div gcdn	;by gcd of numbers
		mov lcmn,ax
		ret
	lcm endp

	main proc	
		mov ax,@data
		mov ds,ax
		;input
		; first number
		printm inpmsg1		
		dec_input
		mov num1,bx
		;second number
		printm inpmsg2
		dec_input
		mov num2,bx
		;third number
		printm inpmsg3
		dec_input
		mov num3,bx			
		; finding GCD
		;find GCD of first two
		mov ax,num1
		mov bx,num2
		call gcd
		; GCD of GCD and third 
		mov ax,num3
		mov bx,gcdn
		call gcd
		;output
		printm oupmsg1
		mov bx,gcdn
		dec_output
		; finding LCM
		; find gcd of two
		mov ax,num1
		mov bx,num2
		call gcd
		; find lcm
		mov ax,num1
		mov bx,num2
		call lcm
		; find gcd of two
		mov ax,lcmn
		mov bx,num3
		call gcd
		; find lcm
		mov ax,lcmn
		mov bx,num3
		call lcm
		;output
		printm oupmsg2
		mov bx,lcmn
		dec_output
		exitp
	main endp
end main