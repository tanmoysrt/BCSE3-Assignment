TITLE	PROG1 {EXE}	Add two 16-bit hexadecimal numbers
;------------------------------------
	.MODEL	SMALL
	.STACK
;------------------------------------
	.DATA
INPMSG1	DB	"First Number? ", "$"
INPMSG2 DB	"Second Number? ", "$"
OUTMSGS	DB	"Sum -> ", "$"
OUTMSGD	DB	0DH, 0AH, "Difference -> ", "$"

NUM1	DW	?
NUM2	DW	?
RESULT	DW 	?
CARRY	DB	?
;------------------------------------
	.CODE
MAIN	PROC	FAR
	MOV	AX,@data		; Initialize segment registers
	MOV	DS,AX

	; Input first number
	LEA  	DX,INPMSG1
	CALL	PROMPT			; Display prompt
	CALL	INPT			; Request input
	MOV	NUM1,AX
	
	; Input second number
	LEA  	DX,INPMSG2
	CALL	PROMPT			; Display prompt
	CALL	INPT			; Request input
	MOV	NUM2,AX

	; Addition
	MOV	CARRY,00H
	MOV	AX,NUM1			; Add first number
	ADD 	AX,NUM2			; Add second number
	JNC	STORE1			; If carry found
	INC	CARRY			; Increment carry
STORE1:
	MOV	RESULT,AX		; Store the result
	LEA	DX,OUTMSGS
	CALL	OUTPT			; Display sum

	; Subtraction
	MOV	CARRY,00H
	MOV	AX,NUM1			; Add first number
	SUB 	AX,NUM2			; Add second number
	MOV	RESULT,AX		; Store the result
	LEA	DX,OUTMSGD
	CALL	OUTPT			; Display sum
		
TATA:
	MOV	AH,4CH			; Terminate
	INT 	21H

MAIN	ENDP

;	Display input prompt
;	--------------------
PROMPT	PROC	NEAR
	MOV	AH,09H
	INT 	21H
	RET
PROMPT	ENDP

;	Accept input of number
;	----------------------
INPT	PROC	NEAR
	PUSH 	DX

	MOV 	DX,0000H
	MOV	CL,4
GETCH:
	MOV 	AH,01H			; Request character input
    	INT 	21H
	CMP 	AL,0DH			; Check if Enter
	JE 	DONE
	CMP 	AL,0AH 			; Check if LineFeed
	JE 	DONE

	SHL 	DX,CL 			; Left shift 4 bits
	SUB 	AL,48 			; Find the numerical value from character
	CMP 	AL,9
	JLE 	ISNUM   		; Check if 0-9 or A-F
	SUB 	AL,7  			; for A-F

ISNUM:
	OR 	DL,AL			; Store the character
	JMP 	GETCH			; Loop to take next character
DONE:
	MOV 	AX,DX			; Store the number
	POP 	DX
	RET

INPT 	ENDP

;	Display Sum
;	-----------
OUTPT 	PROC	NEAR
	MOV	AH,09H			; Display output prompt
	INT 	21H
	MOV	DL, CARRY
	CMP	DL, 00H			; Check if carry is non-zero
	JE 	DISPSUM
	MOV	AH, 02H 		; Display carry
	ADD	DL, 48
	INT 	21H

DISPSUM:
	MOV 	CL,4

    	MOV	DX,RESULT
    	MOV 	DL,DH 			; Take first 4 bits
    	SHR 	DL,CL
    	CALL	DISPCH

    	MOV 	DX,RESULT
    	MOV 	DL,DH 			; The next 4 bits
    	CALL	DISPCH

    	MOV 	DX,RESULT
    	SHR 	DL,CL 			; The next 4 bits
    	CALL	DISPCH

    	MOV 	DX,RESULT 		; The last 4 bits
    	CALL	DISPCH
    	RET
OUTPT 	ENDP

;	Display hex-equivalent of decimal character
;	-------------------------------------------
DISPCH 	PROC 	NEAR
	AND 	DL,0FH
    	CMP 	DL,0AH
    	JL 	ISNUM2
    	ADD 	DL,7
ISNUM2:
        ADD 	DL,48
    	MOV 	AH,02H
    	INT 	21H
	RET
DISPCH 	ENDP

END 	MAIN