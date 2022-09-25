.MODEL SMALL
.STACK 100H
.DATA
var1 db 'q'
nl db 0ah
s db 20h
.CODE
MAIN PROC FAR
    MOV AX,@DATA
    MOV DS,AX   
    mov cl, 00h
    mov ch, 0ffh
    mov ah, 1
loop1:
    int 21h
    cmp al, var1
    je exit
    cmp ch, al
    jc next1
    mov ch, al
next1:
    cmp al, cl
    jc next2
    mov cl, al
next2:
    jmp loop1
exit:
    mov dl, nl
    mov ah,2
    int 21h
    mov dl, ch
    mov ah, 2
    int 21h
    mov dl, s
    int 21h
    mov dl, cl
    int 21h                        
    MOV AH,4CH
    INT 21H

MAIN ENDP

READINT PROC          
    mov ah,1
    int 21h
    sub al, 30h
    mov bh, al
    mov ah, 1
    int 21h
    sub al, 30h
    mov ch, al
    mov al, bh 
    mov bl, 10h
    mul bl 
    add al, ch   
READINT ENDP

END MAIN