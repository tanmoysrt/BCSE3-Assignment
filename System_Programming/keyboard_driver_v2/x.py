from tkinter import *
from tkinter import font
from tkinter.font import Font

win= Tk()
win.geometry("700x700")

fams = font.families()
print('Lohit Bengali' in fams)

# Define a function to display the message
def key_press(e):
    print(e)
    label.config(text="Welcome to TutorialsPoint")

def key_released(e):
   label.config(text="\u0981")
# Create a label widget to add some text
label= Label(win, text= "", font= Font(family="Lohit Bengali", size=12))
label.pack(pady= 50)

# Bind the Mouse button event
win.bind('<KeyPress>',key_press)
win.bind('<KeyRelease>',key_released )
win.mainloop()