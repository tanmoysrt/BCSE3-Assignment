.model tiny
.stack 100h 
.data
     name1 db "Name : Tannmoy Sarkar$" 
     progTitle db "Title : Q1.asm$"
.code          
   mov ax, @data    
   mov ds, ax    
         
   lea dx, name1        ; display name
   mov ah, 09h  
   int 21h
   
   mov dl, 0dh          ; carriage return 
   mov ah, 02h
   int 21h
   
   mov dl, 0ah          ; line feed
   mov ah, 02h
   int 21h
   
   lea dx, progTitle    ; programme title
   mov ah, 09h     
   int 21h
   
   mov ah, 4ch
   int 21h
    
      
end
