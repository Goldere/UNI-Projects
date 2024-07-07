"""
Hirsipuupeli Kuvaus:

Pelaaja yrittää arvata jonkin sanoista, jotka ovat sijoittettu listaan
self.__words_list. Pelaajan tarkoituksena on klikata jonkin kirjaimen
vastaavaa nappia käyttöliittymässä ja täten tehdä arvauksen.

Pelaaja voi arvata väärin 4 kertaa, jonka jälkeen viides virheellinen arvaus lopettaa pelin.
Jos pelaaja onnistuu muodostamaan arvattavan sanan, peli päättyy, jättäen viestin: "You Won!"
Pelin voi pättää painalla "quit" painiketta.

Pelaaja voi aloittaa pelin alusta (MILLOIN VAIN) painamalla "replay" painiketta. Tämä palauttaa pelin takaisin
sen lähtötilanteeseen, MUTTA arvatta sana vaihtuu. Sana arvotaan self.__word_list listasta.
Arvatta sana ei voi tulla arvattavaksi kaksi kertaa peräkkäin.

Projektissa tähdätään kehittyneeseen versioon Hirsipuupelistä, jossa on mukana virtuaalinäppäimisöltä.
"""

from tkinter import *
import random

class Userinterface:

    def __init__(self):
        # Initialize the mainwindow
        self.__mainwindow = Tk()
        self.__mainwindow.configure(background="deepskyblue")

        # List of words that can be guessed.
        self.__words_list = ["GYM", "UNIVERSITY", "PYTHON"] # UPPER CASE ONLY

        self.__word_to_guess = random.choice(self.__words_list)
        self.__guess_word_status = len(self.__word_to_guess) * "_"
        self.__stick_figure_image = PhotoImage(file="stick_figure.png")

        # Set the initial number of guesses.
        self.__guesses_amount = 5

        # Create labels and buttons for different parts of the game.
        self.__stick_figure_label = Label(self.__mainwindow, image=self.__stick_figure_image, background="deepskyblue")
        self.__game_state = Label(self.__mainwindow, background="darkgoldenrod1",
                                  text=f"You can be wrong {self.__guesses_amount} times", font=("Helvetica", 19))
        self.__hang_man_text_label = Label(self.__mainwindow, text="HANGMAN", font=("Helvetica", 25),
                                     background="darkorange1", foreground="azure1")
        self.__current_word_text_label = Label(self.__mainwindow, text=len(self.__word_to_guess)  * " _",
                                 font=("Comic Sans MS", 30), background="deepskyblue")
        self.__first_part = Label(self.__mainwindow, background="deepskyblue")
        self.__second_part = Label(self.__mainwindow, background="deepskyblue", height=20)
        self.__third_part = Label(self.__mainwindow, background="deepskyblue")
        self.__fourth_part = Label(self.__mainwindow, background="deepskyblue")
        self.__empty_row = Label(self.__mainwindow, background="deepskyblue")

        self.__quit_button = Button(self.__mainwindow, text="QUIT", command=self.quit, font=("Comic Sans MS", 15),
                                     background="crimson", foreground="azure1", borderwidth=5)
        self.__reset_button = Button(self.__mainwindow, text="REPLAY", command=self.replay, font=("Comic Sans MS", 15),
                                     background="chartreuse3", foreground="azure1", borderwidth=5)

        # Define a list for the buttons, so we can later loop through them.
        self.__buttons = []

        # Define the keyboard layout.
        keyboard_layout = [
            ["A", "B", "C", "D", "E", "F", "G",],
            ["H", "I", "J", "K", "L", "M", "N",],
            ["O", "P", "Q", "R", "S", "T", "U",],
            ["V", "W", "X", "Y", "Z", "Å", "Ä",],
            ["Ö"],
        ]

        # Creates the keyboard into the UI.
        for row, keys in enumerate(keyboard_layout):
            for column, key in enumerate(keys):
                button = Button(self.__mainwindow, text=key, width=8, height=3, background="darkorange",
                                   command=lambda k=key: self.guess_letter(k))
                button.grid(row=row+1, column=column, padx=2, pady=2)
                # Adds the button to the list.
                self.__buttons.append(button)

        # Place UI elements in the grid layout.
        self.__quit_button.grid(row=0, column=5, sticky=W + E + N + S, columnspan=2)
        self.__hang_man_text_label.grid(row=0, column=0, sticky=W + E + N + S, columnspan=5)
        self.__current_word_text_label.grid(row=7, columnspan=5, column=1, sticky=W+E)
        self.__reset_button.grid(row=5, column=5, sticky=W+E+N+S, columnspan=2)
        self.__game_state.grid(row=5, columnspan=4, column=1, sticky=W+N+S+E)
        self.__empty_row.grid(row=8)
        self.__third_part.grid(row=9, column=1, columnspan=5, sticky=W + N + S + E)
        self.__second_part.grid(row=10, rowspan=4, column=1,sticky=W + N + S + E)
        self.__first_part.grid(row=14, columnspan=3, column=0, sticky=W+N+S+E)
        self.__fourth_part.grid(row=10, columnspan=1, rowspan=1, column=5, sticky=W + N + S + E)
        self.__stick_figure_label.grid(row=13, column=5, columnspan=1)

        self.__mainwindow.mainloop()


    def replay(self):
        # Resets the entire ui starting the game from the beginning.
        self.__guesses_amount = 5

        # Avoids the same word from appearing twice in a row.
        new_word = random.choice(self.__words_list)
        while self.__word_to_guess == new_word:
            new_word = random.choice(self.__words_list)
        self.__word_to_guess = new_word
        self.__guess_word_status = len(self.__word_to_guess) * "_"

        self.__game_state.configure(text=f"You can be wrong {self.__guesses_amount} times")
        self.__first_part.configure(background="deepskyblue")
        self.__second_part.configure(background="deepskyblue")
        self.__third_part.configure(background="deepskyblue")
        self.__fourth_part.configure(background="deepskyblue")
        self.__stick_figure_label.grid(row=13, column=5, columnspan=1)

        # Activates the buttons again.
        for button in self.__buttons:
            button.configure(background="darkorange", state="active")
            self.__current_word_text_label.configure(text=len(self.__word_to_guess)  * " _")

    def check_game_status(self):
        self.__game_state.configure(text=f"You can be wrong {self.__guesses_amount} times")

        # Ends the game when the guess limit is reached.
        if self.__guesses_amount == 0:
            self.__stick_figure_label.grid(row=11, column=5, columnspan=1)
            for button in self.__buttons:
                button.configure(state="disabled")
            self.__game_state.configure(text="Game Over!")

        elif self.__guesses_amount == 4:
            self.__first_part.configure(background="black")
        elif self.__guesses_amount == 3:
            self.__second_part.configure(background="black")
        elif self.__guesses_amount == 2:
            self.__third_part.configure(background="black")
        elif self.__guesses_amount == 1:
            self.__fourth_part.configure(background="black")
            self.__game_state.configure(text=f"You can be wrong {self.__guesses_amount} time")

        # Ends the game when the player fully guesses the word.
        if self.__guess_word_status == self.__word_to_guess:
            self.__game_state.configure(text="You Won!")
            # Disables the buttons so the player can't keep playing.
            for button in self.__buttons:
                button.configure(state="disabled")


    def guess_letter(self, key):
        for button in self.__buttons:
            # Checks if the guessed key is in the word that the user is trying to guess
            if button.cget("text") == key and key in self.__word_to_guess:
                button.configure(background="chartreuse1", state="disabled")

                # Rewrites the current state of the word that is being guessed and updates it to the label.
                index_to_change = 0
                string_list = list(self.__guess_word_status)
                for index, letter in enumerate(self.__word_to_guess):
                    if key == letter:
                        index_to_change = index
                        string_list[index_to_change] = key
                        self.__guess_word_status = "".join(string_list)
                        self.__current_word_text_label.configure(text=" ".join(string_list))

                self.check_game_status()

            # Disables the clicked button and decreases the amount of guesses when the guess is wrong
            elif button.cget("text") == key:
                self.__guesses_amount -= 1
                button.configure(background="firebrick", state="disabled")
                self.check_game_status()

    def quit(self):
        # Ends the execution of the program.
        self.__mainwindow.destroy()

def main():
    ui = Userinterface()

if __name__ == "__main__":
    main()
