.model small
.stack 100h
.data     
    msg1 db "enter a character : $"
    msg2 db "entered character : $"
.code

; macro function to print string               
print macro msg                        
    push ax
    push dx            
    
    mov ah, 09h         ; print string
    lea dx, msg
    int 21h        
   
    pop dx
    pop ax
endm               

; macro function for new line
nextline macro  
    push ax
    push dx
    
    mov dl, 0dh          ; carriage return 
    mov ah, 02h
    int 21h
   
    mov dl, 0ah          ; line feed
    mov ah, 02h
    int 21h
    
    pop dx
    pop ax
endm

main proc   
    mov ax, @data
    mov ds, ax
    
    print msg1             ; print prompt
    
    mov ah, 01h            ; read character
    int 21h                      
    
    nextline               ; new line
    
    print msg2             ; print message2
    
    mov dl, al             ; print character
    mov ah, 02h
    int 21h
    
    mov ah,4ch             ; terminate programme
    int 21h        
main endp
end main
