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
.model small
.stack 100h
.data
	oldfilename db "OLD_FILE.txt", 0
	newfilename db "NEW_FILE.txt", 0
	oupmsg db 10,13,"File renamed $"
.code
	main proc
		mov ax, @data
		mov ds, ax
		mov es, ax
		lea dx,oldfilename
		lea di,newfilename
		mov ah,56h	;to rename file
		int 21h
		printm oupmsg
		exitp	;exit
	main endp
end main
