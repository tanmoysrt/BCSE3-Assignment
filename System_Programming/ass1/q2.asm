.model small
.stack 100h 
.data
     msg1 db "Enter a character : $" 
     msg2 db "Result : $"
.code          
   mov ax, @data    
   mov ds, ax    
         
   lea dx, msg1         ; display input prompt
   mov ah, 09h  
   int 21h     
   
   mov ah, 01h          ; accept character from stdin
   int 21h
   
   ; Check whether the input character is already lowercase
   cmp al, 'A'
   JL SHOW
   
   cmp al, 'Z'
   JG SHOW
   
   ADD al, 32           ; convert to lowercase by adding 32

   SHOW:           
   
   mov bl, al           ; store the result in bl
   
   mov dl, 0dh          ; carriage return 
   mov ah, 02h
   int 21h
   
   mov dl, 0ah          ; line feed
   mov ah, 02h
   int 21h
   
   lea dx, msg2         ; display the msg2
   mov ah, 09h     
   int 21h     
   
   
   mov dl, bl           ; show the result
   mov ah, 02h
   int 21h              
   
   mov ah, 4ch
   int 21h
    
    
      
end
