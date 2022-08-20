.model small
.stack 100h
.data
.code

main proc
    mov cx, 26
    mov al, 41h
    label:    
    mov dl, al
    mov ah, 02h
    int 21h
    inc al
    loop label
    
    mov ah, 4ch
    int 21h
    
main endp
end main
