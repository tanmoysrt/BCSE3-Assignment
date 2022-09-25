.model small
.stack 100h
.data
      msg1 db "enter a character [press enter to exit] : $"
      msg2 db "program terminated $"
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
 
 labelZ:
 print msg1
 mov ah, 01h
 int 21h
 nextline
 cmp al, 13
 nop     
 jne labelZ     
 
 nextline
 print msg2
 
 mov ah,4ch
 int 21h
main endp
end main